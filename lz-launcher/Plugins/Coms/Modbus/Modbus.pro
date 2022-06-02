QT += gui widgets sql serialbus
qtConfig(modbus-serialport): QT += serialport

TEMPLATE = lib
CONFIG += plugin

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
INCLUDEPATH += ../../../LZDetector/Interface
INCLUDEPATH += ../../../Librarys/LZControl
INCLUDEPATH += ../../../Librarys/LZLib
COPY_PWD = $$replace(PWD, /, \\)
COPY_OUT_PWD = $$replace(OUT_PWD, /, \\)
CONFIG(debug, debug|release) {
    win32{
        QMAKE_POST_LINK += mkdir $$COPY_PWD\..\..\..\..\rundir\debug\plugins\Coms &
        QMAKE_POST_LINK += Copy $$COPY_OUT_PWD\debug\*.dll $$COPY_PWD\..\..\..\..\rundir\debug\plugins\Coms

        LIBS += -L$$COPY_OUT_PWD/../../../Librarys/LZLib/debug -lLZLib
        LIBS += -L$$COPY_OUT_PWD/../../../Librarys/LZControl/debug -lLZControl
    }
} else {
    win32{
        QMAKE_POST_LINK += mkdir $$COPY_PWD\..\..\..\..\rundir\release\plugins\Coms &
        QMAKE_POST_LINK += Copy $$COPY_OUT_PWD\release\*.dll $$COPY_PWD\..\..\..\..\rundir\release\plugins\Coms

        LIBS += -L$$COPY_OUT_PWD/../../../Librarys/LZLib/release -lLZLib
        LIBS += -L$$COPY_OUT_PWD/../../../Librarys/LZControl/release -lLZControl
    }
}

SOURCES += \
    Device/DeviceCom.cpp \
    Device/DeviceView.cpp \
    ModbusData.cpp \
    ModbusPlugin.cpp \
    Order/OrderCom.cpp \
    Order/OrderModel.cpp \
    Order/OrderView.cpp

HEADERS += \
    Device/DeviceCom.h \
    Device/DeviceView.h \
    ModbusData.h \
    ModbusPlugin.h \
    Order/OrderCom.h \
    Order/OrderModel.h \
    Order/OrderView.h

DISTFILES += Modbus.json

# Default rules for deployment.
unix {
    target.path = $$[QT_INSTALL_PLUGINS]/generic
}
!isEmpty(target.path): INSTALLS += target

FORMS += \
    Device/DeviceCom.ui \
    Device/DeviceView.ui \
    Order/OrderView.ui
