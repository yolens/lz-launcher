QT -= gui
QT += widgets

TEMPLATE = lib
DEFINES += LZCONTROL_LIBRARY

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
COPY_PWD = $$replace(PWD, /, \\)
COPY_OUT_PWD = $$replace(OUT_PWD, /, \\)
CONFIG(debug, debug|release) {
    win32{
        QMAKE_POST_LINK += mkdir $$COPY_PWD\..\..\..\rundir\debug &
        QMAKE_POST_LINK += Copy $$COPY_OUT_PWD\debug\*.dll $$COPY_PWD\..\..\..\rundir\debug

    }
} else {
    win32{
        QMAKE_POST_LINK += mkdir $$COPY_PWD\..\..\..\rundir\release &
        QMAKE_POST_LINK += Copy $$COPY_OUT_PWD\release\*.dll $$COPY_PWD\..\..\..\rundir\release

    }
}

SOURCES += \
    LZControl.cpp \
    LZGraphicsView/Items/Item.cpp \
    LZGraphicsView/Items/Line.cpp \
    LZGraphicsView/Items/Node.cpp \
    LZGraphicsView/LZGraphicsScene.cpp \
    LZGraphicsView/LZGraphicsView.cpp

HEADERS += \
    LZControl_global.h \
    LZControl.h \
    LZGraphicsView/Items/Item.h \
    LZGraphicsView/Items/Line.h \
    LZGraphicsView/Items/Node.h \
    LZGraphicsView/LZGraphicsScene.h \
    LZGraphicsView/LZGraphicsView.h

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target
