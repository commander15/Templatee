#include <QtQml/qqmlextensionplugin.h>
#include <QtQml/qqmlengine.h>

#include <{{ Project.name }}/{{ Project.micro }}.h>

using namespace {{ Project.name }};

class {{ Project.name }}QmlPlugin : public QQmlExtensionPlugin
{
    Q_OBJECT

public:
    void registerTypes(const char *uri) override
    {
        // @uri {{ Project.name }}
        int major = 2, minor = 0;

        // Register your types here !
    }
};

#include "plugin.moc"

