#include "rendercommand.h"

#include <utils/jsonutils.h>
#include <utils/renderer.h>

#include <QtCore/qdir.h>
#include <QtCore/qdiriterator.h>
#include <QtCore/qfile.h>
#include <QtCore/qfileinfo.h>
#include <QtCore/qmimedatabase.h>
#include <QtCore/qmimetype.h>
#include <QtCore/qstandardpaths.h>

RenderCommand::RenderCommand()
    : m_templateOption("template", "the template directory", "template dir")
    , m_dataOption("data", "the data json input", "json data file")
    , m_outputOption("output", "the output directory", "output dir")
    , m_generateJsonOption("json", "the preprocessed json file", "json file")
{
}

void RenderCommand::initParser(QCommandLineParser *parser)
{
    parser->addOptions({ m_templateOption, m_dataOption, m_outputOption, m_generateJsonOption });
}

int RenderCommand::run()
{
    const QStringList arguments = parser.positionalArguments();

    if (!parser.isSet(m_templateOption)) {
        errorStream << "template not provided" << Qt::endl;
        return TemplateNotProvidedError;
    }

    QDir templateDir = this->templateDir(parser.value(m_templateOption));
    if (!templateDir.exists()) {
        errorStream << "invalid template provided" << Qt::endl;
        return InvalidTemplateError;
    }

    QDir outputDir(parser.value(m_outputOption));
    if (!outputDir.exists())
        outputDir.mkpath(".");

    QFile dataFile(this->dataFile(parser.value(m_dataOption), (!arguments.isEmpty() ? arguments.first() : QString())));

    if (!dataFile.exists()) {
        errorStream << "the data file doesn't exists" << Qt::endl;
        return -1;
    }

    if (!dataFile.open(QIODevice::ReadOnly)) {
        errorStream << "the data file can't be opened, check for permissions" << Qt::endl;
        return -1;
    }

    QJsonParseError parseError;
    QJsonObject inputData = JsonUtils::readJsonObject(&dataFile, &parseError);

    if (parseError.error != QJsonParseError::NoError) {
        errorStream << parseError.errorString() << Qt::endl;
        return -1;
    }

    if (templateDir.exists("Templatee.json")) {
        QJsonObject extraData = JsonUtils::readJsonObject(templateDir.filePath("Templatee.json"), &parseError);
        if (!extraData.isEmpty())
            inputData = JsonUtils::mergeObjects(inputData, extraData, JsonUtils::FirstPriority);
    }

    QString templateContext = "Template";

    QJsonObject templateData;
    if (!fixData(&templateContext, &templateData, inputData)) {
        return -1;
    }

    QJsonObject templatee;
    templatee.insert("input", inputData);
    templateData.insert("Templatee", templatee);

    if (true) {
        const QString contextName = templateData.value(templateContext).toObject().value("name").toString();
        outputDir.mkdir(contextName);
        outputDir.cd(contextName);
    }

    const QString jsonOutput = parser.value(m_generateJsonOption);
    if (!jsonOutput.isEmpty()) {
        QFile f(jsonOutput);

        if (!f.open(QIODevice::WriteOnly)) {
            errorStream << f.errorString() << Qt::endl;
            return -1;
        }

        if (!JsonUtils::writeJsonObject(&f, templateData)) {
            return -1;
        }

        f.flush();
        f.close();
    }

    return render(templateDir, outputDir, templateData);
}

bool RenderCommand::fixData(QString *context, QJsonObject *templateData, const QJsonObject &inputData)
{
    if (inputData.contains("templatee")) {
        const QJsonObject data = inputData.value("templatee").toObject();
        if (data.contains("context"))
            *context = data.value("context").toString();
    }

    *templateData = inputData;
    return true;
}

int RenderCommand::render(const QDir templateDir, const QDir &outputDir, const QJsonObject &templateData)
{
    auto outputName = [&templateDir, &outputDir, &templateData](const QString &source) {
        QString o = outputDir.filePath(templateDir.relativeFilePath(source));

        static const QRegularExpression exp("^.+@(.+)@.+$");

        QRegularExpressionMatch match = exp.match(o);
        if (match.hasMatch()) {
            const QString variable = match.captured(1);
            QVariant value;

            if (variable.contains('.')) {
                QStringList path = variable.split('.', Qt::SkipEmptyParts);
                QString field = path.takeLast();

                QJsonObject currentObject = templateData;
                for (const QString &currentPath : std::as_const(path))
                    currentObject = currentObject.value(currentPath).toObject();

                value = currentObject.value(field).toVariant();
            } else {
                value = templateData.value(variable).toVariant();
            }

            if (value.isValid())
                o.replace('@' + variable + '@', value.toString());
        }

        return o;
    };

    QDirIterator it(templateDir, QDirIterator::FollowSymlinks|QDirIterator::Subdirectories);
    while (it.hasNext()) {
        const QFileInfo sourceInfo = it.nextFileInfo();

        if (sourceInfo.fileName() == "Templatee.json") {
            continue;
        }

        if (sourceInfo.isDir() && !sourceInfo.dir().isEmpty()) {
            continue;
        }

        if (!sourceInfo.isFile()) {
            continue;
        }

        const QString relativePath = templateDir.relativeFilePath(sourceInfo.filePath());
        outputStream << "processing: " << relativePath << Qt::endl;

        if (relativePath.contains('/'))
            outputDir.mkpath(relativePath.section('/', 0, -2));

        const QString source = sourceInfo.absoluteFilePath();
        const QString target = outputName(source);

        if (!Renderer::canRender(sourceInfo)) {
            if (QFile::exists(target) && !QFile::remove(target)) {
                errorStream << "can't replace target file " << relativePath << Qt::endl;
            } else if (!QFile::copy(source, target)) {
                errorStream << "Can't copy file " << relativePath << Qt::endl;
            }

            continue;
        }

        try {
            Renderer::render(source, target, templateData);
        } catch (const inja::InjaError &e) {
            errorStream << "Can't render file '" << relativePath << "', ";
            errorStream << QString::fromStdString(e.message) << " at " << e.location.line << ':' << e.location.column << Qt::endl;
            return -1;
        }
    }
    return NoError;
}

QString RenderCommand::dataFile(const QString &path, const QString &name)
{
    QFileInfo info(path);

    if (!info.exists())
        return QString();

    if (info.isDir()) {
        QDir dir(info.absoluteFilePath());
        return dir.absoluteFilePath(name + ".json");
    }

    return info.absoluteFilePath();
}

QDir RenderCommand::templateDir(const QString &path)
{
#ifdef TEMPLATES_DIR
    QDir templatesDir(TEMPLATES_DIR);
#else
    QDir templatesDir(QStandardPaths::writableLocation(QStandardPaths::TemplatesLocation) + "/Templatee");
#endif

    QDir dir(QDir::isAbsolutePath(path) ? path : templatesDir.filePath(path));
    dir.setFilter(QDir::Dirs|QDir::Files|QDir::Hidden);
    return dir;
}
