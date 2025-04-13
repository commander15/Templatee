#include "application.h"

#include "command/command.h"
#include "command/rendercommand.h"
#include "project/createproject.h"

Application::Application(int &argc, char **argv)
    : QCoreApplication(argc, argv)
{
}

int Application::exec()
{
    QStringList arguments = this->arguments();
    QString commandName = (arguments.size() > 1 ? arguments.takeAt(1) : "");
    //arguments[0] = arguments.at(0) + ' ' + commandName;

    Command *cmd = nullptr;

    if (commandName == "render")
        cmd = new RenderCommand();
    else if (commandName == "create-project")
        cmd = new CreateProject();
    else
        cmd = nullptr; // ToDo: add a default command

    if (cmd) {
        cmd->init();
        cmd->setArguments(arguments);
        int code = cmd->exec();
        delete cmd;
        return code;
    }

    return 255;
}
