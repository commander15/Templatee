#include "createproject.h"

#include <utils/jsonutils.h>

CreateProject::CreateProject()
{
}

void CreateProject::initParser(QCommandLineParser *parser)
{
    RenderCommand::initParser(parser);

    parser->addPositionalArgument("name", "the project name");
}

bool CreateProject::fixData(QString *context, QJsonObject *templateData, const QJsonObject &input)
{
    *context = "Project";

    QJsonObject project;

    // Project name, description and url
    {
        const QStringList args = parser.positionalArguments();
        QString name = !args.isEmpty() ? args.first() : input.value("_name").toString();
        if (name.isEmpty()) {
            errorStream << "error: project name not provided" << Qt::endl;
            return false;
        }

        project.insert("name", name);
        project.insert("macro", name.toUpper());
        project.insert("micro", name.toLower());

        QString desc = input.value("description").toString();
        project.insert("description", !desc.isEmpty() ? desc : name);

        QString url = input.value("url").toString();
        project.insert("url", url);
    }

    // Project version
    {
        if (input.contains("version"))
            project.insert("version", JsonUtils::versionObject(input.value("version").toString()));
        else
            project.insert("version", JsonUtils::versionObject("1.0.0"));
    }

    // Project languages
    {
        QJsonArray languages;

        const QJsonArray languageArray = input.value("languages").toArray();
        for (const QJsonValue &value : languageArray) {
            QJsonObject language = value.toObject();

            const QString name = language.value("name").toString();
            if (name.isEmpty())
                continue;

            languages.append(name);

            language.remove("name");
            templateData->insert(name, language);
        }

        project.insert("languages", languages);
    }

    // Project locales
    {
        QJsonArray localeArray = input.value("locales").toArray();
        if (localeArray.isEmpty())
            localeArray.append("en");
        project.insert("locales", localeArray);
    }

    // Project dependencies
    {
        QJsonArray dependencyArray;
        const QJsonArray dependencies = input.value("dependencies").toArray();

        for (const QJsonValue &value : dependencies) {
            QJsonObject dependency = value.toObject();

            const QString name = dependency.value("name").toString();
            if (name.isEmpty())
                continue;

            if (dependency.contains("version"))
                dependency.insert("version", JsonUtils::versionObject(dependency.value("version").toString()));

            dependencyArray.append(dependency);

            dependency.remove("name");
            templateData->insert(name, dependency);
        }

        project.insert("dependencies", dependencyArray);
    }

    // Extras
    {
        const QJsonArray extras = input.value("extras").toArray();
        for (const QJsonValue &value : extras) {
            const QJsonObject extra = value.toObject();

            const QString name = extra.value("name").toString();
            const QString field = extra.value("field").toString();
            if (field.isEmpty() || !input.contains(field))
                continue;

            templateData->insert(!name.isEmpty() ? name : field, input.value(field));
        }
    }

    templateData->insert("Project", project);
    return true;
}
