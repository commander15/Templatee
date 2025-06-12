#include "application.h"

#include <QtCore/qstandardpaths.h>
#include <QtCore/qversionnumber.h>
#include <QtCore/qdiriterator.h>

#include "command/command.h"
#include "command/rendercommand.h"
#include "project/createproject.h"

Application::Application(int &argc, char **argv)
    : QCoreApplication(argc, argv)
    , m_templateDir(QStandardPaths::writableLocation(QStandardPaths::TemplatesLocation) + "/Templatee")
{
}

int Application::exec()
{
    // Template directory handling

    if (!m_templateDir.exists())
        m_templateDir.mkpath(".");

    if (m_templateDir.isEmpty())
        copyTemplates();

    QFile file(m_templateDir.filePath("version.txt"));
    file.open(QIODevice::ReadOnly);

    const QVersionNumber appVersion = QVersionNumber::fromString(applicationVersion());
    const QVersionNumber temVersion = QVersionNumber::fromString(file.isOpen() ? file.readAll().trimmed() : "");
    if (appVersion > temVersion)
        copyTemplates();

    // Commands handling

    QStringList arguments = this->arguments();
    QString commandName = (arguments.size() > 1 ? arguments.takeAt(1) : "");
    //arguments[0] = arguments.at(0) + ' ' + commandName;

    Command *cmd = nullptr;

    if (commandName == "render")
        cmd = new RenderCommand();
    else if (commandName == "create-project")
        cmd = new CreateProject();
    else {
        cmd = nullptr;

        const QStringList commands = { "render", "create-project" };

        QTextStream out(stdout);
        out << "Templatee - the ultimate template rendering tool for developers\n" << Qt::endl;
        out << "supported commands:" << Qt::endl;

        for (const QString &command : commands)
            out << "-> " << command << '\n';
        out << Qt::endl;

        out << "template directory: " << m_templateDir.absolutePath() << Qt::endl;
        out << "version " << applicationVersion() << Qt::endl;
    }

    if (cmd) {
        cmd->init();
        cmd->setArguments(arguments);
        int code = cmd->exec();
        delete cmd;
        return code;
    }

    return 0;
}

void Application::copyTemplates()
{
    QTextStream out(stdout);

    out << "Copying templates on " << m_templateDir.absolutePath() << " ..." << Qt::endl;

    QString sourceDir(applicationDirPath() + "/../share/Templatee/templates");
    QDir targetDir = m_templateDir;

    QDirIterator it(sourceDir, QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);
    while (it.hasNext()) {
        QFileInfo info = it.nextFileInfo();
        QString relPath = QDir(sourceDir).relativeFilePath(info.filePath());
        QString destPath = targetDir.filePath(relPath);

        if (info.isDir()) {
            QDir().mkpath(destPath);
        } else {
            QDir().mkpath(QFileInfo(destPath).path());

            if (QFile::exists(destPath))
                QFile::remove(destPath);

            QFile::copy(info.filePath(), destPath);
        }
    }

    QFile version(targetDir.filePath("version.txt"));
    if (version.open(QIODevice::WriteOnly)) {
        version.write(applicationVersion().toLatin1());
        version.flush();
        version.close();
    }
}
