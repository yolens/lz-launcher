#ifndef LZLIB_GLOBAL_H
#define LZLIB_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(LZLIB_LIBRARY)
#  define LZLIB_EXPORT Q_DECL_EXPORT
#else
#  define LZLIB_EXPORT Q_DECL_IMPORT
#endif

#endif // LZLIB_GLOBAL_H
