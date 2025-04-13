#include "command.h"

Command::Command()
    : outputStream(stdout)
    , errorStream(stderr)
    , inputStream(stdin)
{
}

void Command::init()
{
    parser.setApplicationDescription("templatee");
    parser.addHelpOption();
    parser.addVersionOption();

    initParser(&parser);
}

QStringList Command::arguments() const
{
    return m_arguments;
}

void Command::setArguments(const QStringList &arguments)
{
    m_arguments = arguments;
}

int Command::exec()
{
    parser.process(m_arguments);
    return run();
}

bool Command::hasOption(const QCommandLineOption &option) const
{
    return parser.isSet(option);
}

QString Command::optionValue(const QCommandLineOption &option) const
{
    return parser.value(option);
}

QStringList Command::optionValues(const QCommandLineOption &option) const
{
    return parser.values(option);
}
