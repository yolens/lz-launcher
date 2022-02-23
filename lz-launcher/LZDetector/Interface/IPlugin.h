#ifndef IPLUGIN_H
#define IPLUGIN_H

#include <QObject>
#include <QUrl>
#include <QUuid>
#include "IMainWindow.h"

class IPluginManager;
class IPlugin
{
public:
    typedef struct IPluginInfo_
    {
        QString name;
        QString description;
        QString version;
        QString author;
        QUrl homePage;
    }IPluginInfo;
public:
    virtual QObject* instance() = 0;
    virtual QUuid pluginUuid() const = 0;
    virtual void pluginInfo(IPluginInfo *pluginInfo) = 0;
    virtual bool initConnections(IPluginManager *pluginManager, int& initOrder) = 0;
    virtual bool initObjects() = 0;
    virtual bool initSettings() = 0;
    virtual bool startPlugin() = 0;
    virtual bool stopPlugin() = 0;
};
class IPluginManager
{
public:
    virtual QObject* instance() = 0;
    virtual QString version() = 0;
    virtual IPlugin* pluginInstance(const QUuid &uid) = 0;
    virtual const IPlugin::IPluginInfo* pluginInfo(const QUuid &uid) = 0;
};

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

class PLUGIN_HELPER_EXPORT PluginHelper
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

        return true;
    }

public:
    static IMainWindow*     MainWindowPlugin()      { return main_window_; }

private:
    static IMainWindow*     main_window_;
#else
public:
    static bool InitConnections(IPluginManager* pluginManager) {
        if (!pluginManager)
            return false;

        AssignPointer(Ins->main_window_,        pluginManager, MAINWINDOW_UUID);

        return true;
    }

public:
    static IMainWindow*     MainWindowPlugin()      { return Ins->main_window_; }

private:
    IMainWindow*    main_window_    = nullptr;
#endif
};

#if EXPORT
Q_DECLARE_INTERFACE(PluginHelper,"BSL.PluginHelper/1.0")
#endif

Q_DECLARE_INTERFACE(IPlugin,"LZ.IPlugin/1.0")
Q_DECLARE_INTERFACE(IPluginManager,"LZ.IPluginManager/1.0")

#endif // IPLUGIN_H
