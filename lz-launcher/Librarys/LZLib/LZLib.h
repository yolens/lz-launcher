#ifndef LZLIB_H
#define LZLIB_H

#include "LZLib_global.h"
#include "LDB.h"
#include <QColor>
#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif

const QColor Chart_Circuit_Input_Color = QColor(0,0,0);
const QColor Chart_Circuit_Output_Color = QColor(255,0,0);
const QColor Chart_Value_Input_Color = QColor(0,0,255);
const QColor Chart_Value_Output_Color = QColor(0,255,255);

const QString Mime_Node_Scene = "Node/Scene";
const QString Mime_Node_Chart = "Node/Chart";
const QString Mime_Node_Point = "Node/Point";

class LZLIB_EXPORT LZLib
{
private:
    LZLib();

public:
    static LZLib* instance();

    QVariant toLonglong(const LOrder::ByteType type, const QVariant& value);
    QVariant fromLonglong(const LOrder::ByteType type, const QVariant& value);
};

#include "../../LZDetector/Interface/IPlugin.h"
#define ALL_STATIC 0
#define EXPORT 0

#if EXPORT
#  define PLUGIN_HELPER_EXPORT Q_DECL_EXPORT
#else
#  define PLUGIN_HELPER_EXPORT
#endif

#define Plugin PluginHelper
#define Ins Plugin::Instance()

template <typename T>
void AssignPointer(T*& pointer, IPluginManager* pluginManager, const QUuid& uid) {
    if (!pluginManager)
        return;

    if (pluginManager->pluginInstance(uid))
        pointer = qobject_cast<T*>(pluginManager->pluginInstance(uid)->instance());
}

class LZLIB_EXPORT PluginHelper
{
public:
    static PluginHelper* Instance() {
        static PluginHelper plugin_helper;
        return &plugin_helper;
    }

#if ALL_STATIC
public:
    static bool InitConnections(IPluginManager* pluginManager) {
        if (!pluginManager)
            return false;

        AssignPointer(main_window_,     pluginManager, MAINWINDOW_UUID);
        AssignPointer(data_center_,     pluginManager, DATACENTER_UUID);

        return true;
    }

public:
    static IMainWindow*     MainWindowPlugin()      { return main_window_; }
    static IDataCenter*     DataCenterPlugin()      { return data_center_; }

private:
    static IMainWindow*     main_window_;
    static IDataCenter*     data_center_;
#else
public:
    static bool InitConnections(IPluginManager* pluginManager) {
        if (!pluginManager)
            return false;
        Ins->manager_ = pluginManager;
        AssignPointer(Ins->main_window_,        pluginManager, MAINWINDOW_UUID);
        AssignPointer(Ins->data_center_,        pluginManager, DATACENTER_UUID);

        return true;
    }

public:
    static IPluginManager*  Manager()               { return Ins->manager_; }
    static IMainWindow*     MainWindowPlugin()      { return Ins->main_window_; }
    static IDataCenter*     DataCenterPlugin()      { return Ins->data_center_; }

private:
    IPluginManager  *manager_       = nullptr;
    IMainWindow     *main_window_   = nullptr;
    IDataCenter     *data_center_   = nullptr;
#endif
};



#endif // LZLIB_H
