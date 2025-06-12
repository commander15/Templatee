#include <gtest/gtest.h>

#include <QtCore/qcoreapplication.h>
#include <QtCore/qloggingcategory.h>

int main(int argc, char *argv[])
{
    testing::InitGoogleTest(&argc, argv);

    QCoreApplication app(argc, argv);
    QLoggingCategory::setFilterRules("{{ Project.micro }}.warning=true");

    return RUN_ALL_TESTS();
}
