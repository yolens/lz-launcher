#ifndef LZCONTROL_GLOBAL_H
#define LZCONTROL_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(LZCONTROL_LIBRARY)
#  define LZCONTROL_EXPORT Q_DECL_EXPORT
#else
#  define LZCONTROL_EXPORT Q_DECL_IMPORT
#endif

#endif // LZCONTROL_GLOBAL_H
