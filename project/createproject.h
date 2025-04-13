#ifndef CREATEPROJECT_H
#define CREATEPROJECT_H

#include <command/rendercommand.h>

class CreateProject : public RenderCommand
{
public:
    CreateProject();

protected:
    void initParser(QCommandLineParser *parser) override;

    bool fixData(QString *context, QJsonObject *templateData, const QJsonObject &input) override;
};

#endif // CREATEPROJECT_H
