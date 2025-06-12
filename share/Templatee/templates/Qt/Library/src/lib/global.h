#ifndef {{ Project.macro }}_GLOBAL_H
#define {{ Project.macro }}_GLOBAL_H

#include <{{ Project.name }}/config.h>

#ifdef {{ Project.macro }}_SHARED
#   ifdef {{ Project.macro }}_BUILD
#       define {{ Project.macro }}_EXPORT Q_DECL_EXPORT
#   else
#       define {{ Project.macro }}_EXPORT Q_DECL_IMPORT
#   endif
#else
#   define {{ Project.macro }}_EXPORT
#endif

#define {{ Project.macro }}_Q(ClassName) ClassName *q = static_cast<ClassName *>(q_ptr)
#define {{ Project.macro }}_D(ClassName) \
    ClassName##Private *d = static_cast<ClassName##Private *>(qGetPtrHelper(d_ptr))

namespace {{ Project.name }} {
  // Populate !
}

#endif // {{ Project.macro }}_GLOBAL_H
