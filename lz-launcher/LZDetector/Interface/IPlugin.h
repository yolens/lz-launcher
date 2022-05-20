#ifndef IPLUGIN_H
#define IPLUGIN_H

#include <QObject>
#include <QUrl>
#include <QUuid>
#include "IMainWindow.h"
#include "IDataCenter.h"

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

    enum Type {
        Coms = 0,           //通信插件
        Main = 1000,       //主插件
    };
public:
    virtual QObject* instance() = 0;
    virtual QUuid pluginUuid() const = 0;
    virtual IPlugin::Type pluginType() const = 0;
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
    virtual QList<IPlugin*> getPluginsByType(IPlugin::Type type) = 0;
};


Q_DECLARE_INTERFACE(IPlugin,"LZ.IPlugin/1.0")
Q_DECLARE_INTERFACE(IPluginManager,"LZ.IPluginManager/1.0")

#endif // IPLUGIN_H
