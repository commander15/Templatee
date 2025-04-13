#ifndef APPLICATION_H
#define APPLICATION_H

#include <QCoreApplication>

class Application : public QCoreApplication
{
    Q_OBJECT

public:
    explicit Application(int &argc, char **argv);
    ~Application() = default;

    int exec();
};

#endif // APPLICATION_H
