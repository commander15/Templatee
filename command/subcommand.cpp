#include "subcommand.h"

SubCommand::SubCommand(Command *command)
    : command(command)
    , outputStream(command->outputStream)
    , errorStream(command->errorStream)
    , inputStream(command->inputStream)
    , parser(command->parser)
{
}

SubCommand::~SubCommand()
{
}

void SubCommand::init()
{
    initParser(&parser);
}

int SubCommand::exec()
{
    return run();
}

void SubCommand::initParser(QCommandLineParser *parser)
{
}

bool SubCommand::hasOption(const QCommandLineOption &option) const
{
    return parser.isSet(option);
}

QString SubCommand::optionValue(const QCommandLineOption &option) const
{
    return parser.value(option);
}

QStringList SubCommand::optionValues(const QCommandLineOption &option) const
{
    return parser.values(option);
}
