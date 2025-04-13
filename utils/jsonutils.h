#ifndef JSONUTILS_H
#define JSONUTILS_H

#include <QtCore/qjsondocument.h>
#include <QtCore/qjsonobject.h>
#include <QtCore/qjsonvalue.h>
#include <QtCore/qjsonarray.h>

#include <inja/json.hpp>

class QJsonParseError;

class JsonUtils
{
public:
    enum MergePriority {
        FirstPriority,
        SecondPriority
    };

    static QJsonObject mergeObjects(const QJsonObject &o1, const QJsonObject &o2, MergePriority priority = FirstPriority);
    static QJsonArray mergeArrays(const QJsonArray &a1, const QJsonArray &a2, MergePriority priority = FirstPriority);

    static QJsonObject readJsonObject(const QString &fileName, QJsonParseError *error);
    static QJsonObject readJsonObject(QIODevice *device, QJsonParseError *error);

    static bool writeJsonObject(const QString &fileName, const QJsonObject &object);
    static bool writeJsonObject(QIODevice *device, const QJsonObject &object);

    static QJsonObject versionObject(const QString &version);

    static inja::json injaJsonFromJsonValue(const QJsonValue &value);
};

#endif // JSONUTILS_H
