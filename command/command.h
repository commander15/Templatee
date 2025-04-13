#ifndef COMMAND_H
#define COMMAND_H

#include <QtCore/qcommandlineparser.h>
#include <QtCore/qcommandlineoption.h>

class AbstractCommand
{
public:
    virtual ~AbstractCommand() = default;

    virtual void init() {}
    virtual int exec() { return run(); }

protected:
    virtual int run() = 0;

    virtual bool hasOption(const QCommandLineOption &option) const = 0;
    virtual QString optionValue(const QCommandLineOption &option) const = 0;
    virtual QStringList optionValues(const QCommandLineOption &option) const = 0;
};

class Command : public AbstractCommand
{
public:
    enum CommandError {
        NoError = 0,
        CustomCommandError = 100
    };

    explicit Command();
    virtual ~Command() = default;

    void init() override;

    QStringList arguments() const;
    void setArguments(const QStringList &arguments);

    int exec() override;

protected:
    virtual void initParser(QCommandLineParser *parser) = 0;

    bool hasOption(const QCommandLineOption &option) const override;
    QString optionValue(const QCommandLineOption &option) const override;
    QStringList optionValues(const QCommandLineOption &option) const override;

    QTextStream outputStream;
    QTextStream errorStream;
    QTextStream inputStream;

    QCommandLineParser parser;

private:
    QStringList m_arguments;
    QList<AbstractCommand *> m_subCommands;

    friend class SubCommand;
};

#endif // COMMAND_H
