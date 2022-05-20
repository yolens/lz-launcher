#ifndef PLUGINMANAGER_H
#define PLUGINMANAGER_H

#include <QHash>
#include <QPluginLoader>
#include "Interface/IPlugin.h"

class PluginManager : public QObject, public IPluginManager
{
    Q_OBJECT
    Q_INTERFACES(IPluginManager);
public:
    struct PluginItem {
        IPlugin *plugin;
        IPlugin::IPluginInfo *info;
        QPluginLoader *loader;
    };
public:
    static PluginManager* manager();
    void init();
    void start();
    void stop();
public:
    virtual QObject* instance() override;
    virtual QString version() override;
    virtual IPlugin* pluginInstance(const QUuid &uid) override;
    virtual const IPlugin::IPluginInfo* pluginInfo(const QUuid &uid) override;
    virtual QList<IPlugin*> getPluginsByType(IPlugin::Type type) override;

private:
    explicit PluginManager(QObject *parent = nullptr);

    bool loadPlugins();
    bool initPlugins();
    bool startPlugins();
    void unloadPlugins();

    bool findFiles(const QString& path);
    void addPlugin(const QString& file);
    void unloadPlugin(const QUuid& uid, const QString& error = "");
    void removePluginItem(const QUuid& uid, const QString& error);
signals:

private:
    QHash<QString, PluginItem> m_pluginMap;
    QList<PluginItem> m_releaseItems;
};

#endif // PLUGINMANAGER_H
