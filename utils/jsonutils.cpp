#include "jsonutils.h"

#include <QtCore/qfile.h>
#include <QtCore/qjsondocument.h>
#include <QtCore/qiodevice.h>
#include <QtCore/qversionnumber.h>

QJsonObject JsonUtils::mergeObjects(const QJsonObject &o1, const QJsonObject &o2, MergePriority priority)
{
    QJsonObject merged;

    QStringList keys = o1.keys() + o2.keys();
    keys.removeDuplicates();

    for (const QString &key : std::as_const(keys)) {
        if (o1.contains(key) && !o2.contains(key))
            merged.insert(key, o1.value(key));
        else if (o2.contains(key) && !o1.contains(key))
            merged.insert(key, o2.value(key));
        else {
            const QJsonValue v1 = o1[key];
            const QJsonValue v2 = o2[key];

            if (v1.isObject() && v2.isObject())
                merged.insert(key, mergeObjects(v1.toObject(), v2.toObject()));
            else if (v1.isArray() && v2.isArray())
                merged.insert(key, mergeArrays(v1.toArray(), v2.toArray(), priority));
            else
                merged.insert(key, priority == FirstPriority ? v1 : v2);
        }
    }

    return merged;
}

QJsonArray JsonUtils::mergeArrays(const QJsonArray &a1, const QJsonArray &a2, MergePriority priority)
{
    QJsonArray merged;

    int size = std::max(a1.size(), a2.size());
    for (int i(0); i < size; ++i) {
        if (i < a1.size() && i < a2.size()) {
            const QJsonValue v1 = a1.at(i);
            const QJsonValue v2 = a2.at(i);

            if (v1.isObject() && v2.isObject())
                merged.append(mergeObjects(v1.toObject(), v2.toObject(), priority));
            else if (v1.isArray() && v2.isArray())
                merged.append(mergeArrays(v1.toArray(), v2.toArray(), priority));
            else if (!a1.contains(v2) && !a2.contains(v1)) {
                merged.append(v1);
                merged.append(v2);
            } else {
                merged.append(v1);
            }
        } else if (i < a1.size()) {
            merged.append(a1.at(i));
        } else if (i < a2.size()) {
            merged.append(a2.at(i));
        }
    }

    return merged;
}

QJsonObject JsonUtils::readJsonObject(const QString &fileName, QJsonParseError *error)
{
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        //
        return QJsonObject();
    }

    return readJsonObject(&file, error);
}

QJsonObject JsonUtils::readJsonObject(QIODevice *device, QJsonParseError *error)
{
    const QByteArray data = device->readAll();
    return QJsonDocument::fromJson(data, error).object();
}

bool JsonUtils::writeJsonObject(const QString &fileName, const QJsonObject &object)
{
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly)) {
        //
        return false;
    }

    return writeJsonObject(&file, object);
}

bool JsonUtils::writeJsonObject(QIODevice *device, const QJsonObject &object)
{
    const QByteArray data = QJsonDocument(object).toJson();
    return device->write(data) > 0;
}

QJsonObject JsonUtils::versionObject(const QString &version)
{
    QVersionNumber versionNumber = QVersionNumber::fromString(version);

    QJsonObject versionObject;
    versionObject.insert("major", versionNumber.majorVersion());
    versionObject.insert("minor", versionNumber.minorVersion());
    versionObject.insert("patch", versionNumber.microVersion());
    versionObject.insert("string", versionNumber.toString());
    return versionObject;
}

inja::json JsonUtils::injaJsonFromJsonValue(const QJsonValue &value)
{
    if (value.isBool()) {
        return value.toBool();
    }

    if (value.isDouble()) {
        if (value.toDouble() == value.toInteger())
            return value.toInteger();
        return value.toDouble();
    }

    if (value.isString()) {
        return value.toString().toStdString();
    }

    if (value.isNull()) {
        return nullptr;
    }

    if (value.isObject()) {
        inja::json jsonObject = inja::json::object();
        const QJsonObject object = value.toObject();
        for (const QString &key : object.keys())
            jsonObject[key.toStdString()] = injaJsonFromJsonValue(object.value(key));
        return jsonObject;
    }

    if (value.isArray()) {
        inja::json jsonArray = inja::json::array();
        for (const QJsonValue &item : value.toArray())
            jsonArray.push_back(injaJsonFromJsonValue(item));
        return jsonArray;
    }

    return {}; // Fallback (should never reach here)
}
