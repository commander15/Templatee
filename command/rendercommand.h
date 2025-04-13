#ifndef RENDERCOMMAND_H
#define RENDERCOMMAND_H

#include "command.h"

#include <inja/inja.hpp>

class QDir;

class RenderCommand : public Command
{
public:
    enum RenderError {
        TemplateNotProvidedError = 50,
        InvalidTemplateError,
        CustomRenderError = CustomCommandError
    };

    RenderCommand();
    virtual ~RenderCommand() = default;

protected:
    void initParser(QCommandLineParser *parser) override;
    int run() override;

    virtual bool fixData(QString *context, QJsonObject *templateData, const QJsonObject &inputData);
    virtual int render(const QDir templateDir, const QDir &outputDir, const QJsonObject &templateData);

    static QString dataFile(const QString &path, const QString &name);
    static QDir templateDir(const QString &path);

private:
    QCommandLineOption m_templateOption;
    QCommandLineOption m_dataOption;
    QCommandLineOption m_outputOption;
    QCommandLineOption m_generateJsonOption;
};

#endif // RENDERCOMMAND_H
