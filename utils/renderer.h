#ifndef RENDERER_H
#define RENDERER_H

#include <inja/inja.hpp>

class QFileInfo;
class QString;
class QIODevice;
class QJsonObject;

class Renderer
{
public:
    static bool canRender(const QFileInfo &file);
    static bool render(const QString &source, const QString &target, const QJsonObject &data);
    static bool render(QIODevice *source, QIODevice *target, const QJsonObject &data);

private:
    static void prepareEnvironment();

    static inja::Environment s_env;
};

#endif // RENDERER_H
