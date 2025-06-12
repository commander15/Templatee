#ifndef {{ Project.macro }}_DEBUG_P_H
#define {{ Project.macro }}_DEBUG_P_H

#include <QtCore/qloggingcategory.h>

#define {{ Project.micro }}Debug()    qCDebug({{ Project.micro }}).noquote().nospace()
#define {{ Project.micro }}Info()     qCInfo({{ Project.micro }}).noquote().nospace()
#define {{ Project.micro }}Warning()  qCWarning({{ Project.micro }}).noquote().nospace()
#define {{ Project.micro }}Critical() qCCritical({{ Project.micro }}).noquote().nospace()

Q_DECLARE_LOGGING_CATEGORY({{ Project.micro }})

#endif // {{ Project.macro }}_DEBUG_P_H
