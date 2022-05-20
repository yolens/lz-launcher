QT += gui

TEMPLATE = lib
CONFIG += plugin

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
INCLUDEPATH += ../../../Librarys/LZLib
INCLUDEPATH += ../../../Librarys/LZControl
INCLUDEPATH += ../../../LZDetector/Interface
COPY_PWD = $$replace(PWD, /, \\)
COPY_OUT_PWD = $$replace(OUT_PWD, /, \\)
CONFIG(debug, debug|release) {
    win32{
        QMAKE_POST_LINK += mkdir $$COPY_PWD\..\..\..\..\rundir\debug\plugins\orders &
        QMAKE_POST_LINK += Copy $$COPY_OUT_PWD\debug\*.dll $$COPY_PWD\..\..\..\..\rundir\debug\plugins\orders

        LIBS += -L$$COPY_OUT_PWD/../../../Librarys/LZControl/debug -lLZControl
        LIBS += -L$$COPY_OUT_PWD/../../../Librarys/LZLib/debug -lLZLib
    }
} else {
    win32{
        QMAKE_POST_LINK += mkdir $$COPY_PWD\..\..\..\..\rundir\release\plugins\orders &
        QMAKE_POST_LINK += Copy $$COPY_OUT_PWD\release\*.dll $$COPY_PWD\..\..\..\..\rundir\release\plugins\orders

        LIBS += -L$$COPY_OUT_PWD/../../../Librarys/LZControl/release -lLZControl
        LIBS += -L$$COPY_OUT_PWD/../../../Librarys/LZLib/release -lLZLib
    }
}

SOURCES += \
    ModbusRtuPlugin.cpp

HEADERS += \
    ModbusRtuPlugin.h

DISTFILES += ModbusRtu.json

# Default rules for deployment.
unix {
    target.path = $$[QT_INSTALL_PLUGINS]/generic
}
!isEmpty(target.path): INSTALLS += target
