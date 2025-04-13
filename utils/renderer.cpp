#include "renderer.h"

#include "jsonutils.h"

#include <QtCore/qmimedatabase.h>
#include <QtCore/qfileinfo.h>
#include <QtCore/qfile.h>
#include <QtCore/qiodevice.h>

bool Renderer::canRender(const QFileInfo &file)
{
    qint64 size = file.size() / 1024 / 1024;
    if (size > 5)
        return false;

    static const QMimeDatabase mimeDb;
    const QMimeType type = mimeDb.mimeTypeForFile(file);
    return type.inherits("text/plain");
}

bool Renderer::render(const QString &source, const QString &target, const QJsonObject &data)
{
    QFile sourceFile(source);

    if (!sourceFile.open(QIODevice::ReadOnly)) {
        return false;
    }

    QFile targetFile(target);
    if (!targetFile.open(QIODevice::WriteOnly)) {
        return false;
    }

    return render(&sourceFile, &targetFile, data);
}

bool Renderer::render(QIODevice *source, QIODevice *target, const QJsonObject &data)
{
    if (!source->isOpen() || source->atEnd())
        return false;

    if (!target->isOpen())
        return false;

    const QByteArray input = source->readAll();
    if (input.isEmpty())
        return true;

    inja::json json = JsonUtils::injaJsonFromJsonValue(data);
    prepareEnvironment();

    const QByteArray output = QByteArray::fromStdString(s_env.render(input.toStdString(), json));
    target->write(output);
    return true;
}

void Renderer::prepareEnvironment()
{
    static bool ready(false);
    if (ready)
        return;

    s_env.add_callback("versionobject", 1, [](inja::Arguments &args) {
        std::string str = *args.at(0);
        const QJsonObject ver = JsonUtils::versionObject(QString::fromStdString(str));
        return JsonUtils::injaJsonFromJsonValue(ver);
    });

    s_env.add_callback("dependencymodules", 3, [](inja::Arguments &args) {
        std::string dep = *args.at(0);
        std::string sep = *args.at(1);
        std::vector<inja::json> modules = *args.at(2);

        for (inja::json &module : modules)
            module = dep + sep + std::string(module);

        return modules;
    });

    ready = true;
}

inja::Environment Renderer::s_env;
