QT -= gui
QT += widgets

TEMPLATE = lib
DEFINES += LZCONTROL_LIBRARY

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
INCLUDEPATH += ../LZLib
INCLUDEPATH += ../../LZDetector/Interface
COPY_PWD = $$replace(PWD, /, \\)
COPY_OUT_PWD = $$replace(OUT_PWD, /, \\)
CONFIG(debug, debug|release) {
    win32{
        QMAKE_POST_LINK += mkdir $$COPY_PWD\..\..\..\rundir\debug &
        QMAKE_POST_LINK += Copy $$COPY_OUT_PWD\debug\*.dll $$COPY_PWD\..\..\..\rundir\debug

        LIBS += -L$$COPY_OUT_PWD/../LZLib/debug -lLZLib
    }
} else {
    win32{
        QMAKE_POST_LINK += mkdir $$COPY_PWD\..\..\..\rundir\release &
        QMAKE_POST_LINK += Copy $$COPY_OUT_PWD\release\*.dll $$COPY_PWD\..\..\..\rundir\release

        LIBS += -L$$COPY_OUT_PWD/../LZLib/release -lLZLib
    }
}

SOURCES += \
    Delegate/ComboBoxDelegate.cpp \
    LOrderModel.cpp \
    LTreeItem.cpp \
    LTreeMode.cpp \
    LZControl.cpp \
    LZGraphicsView/Commands.cpp \
    LZGraphicsView/DetectWorker.cpp \
    LZGraphicsView/Items/Finish.cpp \
    LZGraphicsView/Items/Item.cpp \
    LZGraphicsView/Items/Line.cpp \
    LZGraphicsView/Items/Node.cpp \
    LZGraphicsView/Items/Start.cpp \
    LZGraphicsView/Items/Virtual.cpp \
    LZGraphicsView/LZGraphicsScene.cpp \
    LZGraphicsView/LZGraphicsView.cpp \
    LZGraphicsView/LZWindow.cpp

HEADERS += \
    Delegate/ComboBoxDelegate.h \
    LOrderModel.h \
    LTreeItem.h \
    LTreeMode.h \
    LZControl_global.h \
    LZControl.h \
    LZGraphicsView/Commands.h \
    LZGraphicsView/DetectWorker.h \
    LZGraphicsView/Items/Finish.h \
    LZGraphicsView/Items/Item.h \
    LZGraphicsView/Items/Line.h \
    LZGraphicsView/Items/Node.h \
    LZGraphicsView/Items/Start.h \
    LZGraphicsView/Items/Virtual.h \
    LZGraphicsView/LZGraphicsScene.h \
    LZGraphicsView/LZGraphicsView.h \
    LZGraphicsView/LZWindow.h

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target

FORMS += \
    LZGraphicsView/LZWindow.ui
