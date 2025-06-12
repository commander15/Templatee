#ifndef APPLICATION_H
#define APPLICATION_H

#include <QCoreApplication>
#include <QDir>

class Application : public QCoreApplication
{
    Q_OBJECT

public:
    explicit Application(int &argc, char **argv);
    ~Application() = default;

    int exec();

private:
    void copyTemplates();

    QDir m_templateDir;
};

#endif // APPLICATION_H
