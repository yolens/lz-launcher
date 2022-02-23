QT += gui
QT += widgets

TEMPLATE = lib
CONFIG += plugin

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
INCLUDEPATH += ../../LZDetector/Interface
INCLUDEPATH += ../../Librarys/LZControl
COPY_PWD = $$replace(PWD, /, \\)
COPY_OUT_PWD = $$replace(OUT_PWD, /, \\)
CONFIG(debug, debug|release) {
    win32{
        QMAKE_POST_LINK += mkdir $$COPY_PWD\..\..\..\rundir\debug\plugins &
        QMAKE_POST_LINK += Copy $$COPY_OUT_PWD\debug\*.dll $$COPY_PWD\..\..\..\rundir\debug\plugins

        LIBS += -L$$COPY_OUT_PWD/../../Librarys/LZControl/debug -lLZControl
    }
} else {
    win32{
        QMAKE_POST_LINK += mkdir $$COPY_PWD\..\..\..\rundir\release\plugins &
        QMAKE_POST_LINK += Copy $$COPY_OUT_PWD\release\*.dll $$COPY_PWD\..\..\..\rundir\release\plugins

        LIBS += -L$$COPY_OUT_PWD/../../Librarys/LZControl/release -lLZControl
    }
}


SOURCES += \
    MainWindow.cpp \
    MainWindowPlugin.cpp

HEADERS += \
    MainWindow.h \
    MainWindowPlugin.h

DISTFILES += MainWindow.json

# Default rules for deployment.
unix {
    target.path = $$[QT_INSTALL_PLUGINS]/generic
}
!isEmpty(target.path): INSTALLS += target

FORMS += \
    MainWindow.ui
