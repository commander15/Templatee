#ifndef SUBCOMMAND_H
#define SUBCOMMAND_H

#include "command.h"

class SubCommand : public AbstractCommand
{
public:
    explicit SubCommand(Command *command);
    virtual ~SubCommand();

    void init() override;
    int exec() override;

protected:
    void initParser(QCommandLineParser *parser);

    bool hasOption(const QCommandLineOption &option) const override;
    QString optionValue(const QCommandLineOption &option) const override;
    QStringList optionValues(const QCommandLineOption &option) const override;

private:
    Command *command;

    QTextStream &outputStream;
    QTextStream &errorStream;
    QTextStream &inputStream;

    QCommandLineParser &parser;

    friend class SubCommand;
};

#endif // SUBCOMMAND_H
