#ifndef {{ Project.macro }}_WIDGETS_GLOBAL_H
#define {{ Project.macro }}_WIDGETS_GLOBAL_H

#include <{{ Project.name }}/global.h>
#include <{{ Project.name }}Widgets/config.h>

#ifdef {{ Project.macro }}_SHARED
#   ifdef {{ Project.macro }}_WIDGETS_BUILD
#       define {{ Project.macro }}_WIDGETS_EXPORT Q_DECL_EXPORT
#   else
#       define {{ Project.macro }}_WIDGETS_EXPORT Q_DECL_IMPORT
#   endif
#else
#   define {{ Project.macro }}_WIDGETS_EXPORT
#endif

#endif // {{ Project.macro }}_WIDGETS_GLOBAL_H
