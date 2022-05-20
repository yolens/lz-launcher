#ifndef MAINWINDOWPLUGIN_H
#define MAINWINDOWPLUGIN_H

#include "IPlugin.h"
#include "IMainWindow.h"
#include "MainWindow.h"

class MainWindowPlugin : public QObject, public IMainWindow, public IPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "LZ.IMainWindow/1.0" FILE "MainWindow.json")
    Q_INTERFACES(IMainWindow IPlugin)

public:
    explicit MainWindowPlugin(QObject *parent = nullptr);

public:
    //IPlugin
    virtual QObject* instance() override;
    virtual QUuid pluginUuid() const override;
    virtual IPlugin::Type pluginType() const override;
    virtual void pluginInfo(IPluginInfo *pluginInfo) override;
    virtual bool initConnections(IPluginManager *pluginManager, int& initOrder) override;
    virtual bool initObjects() override;
    virtual bool initSettings() override;
    virtual bool startPlugin() override;
    virtual bool stopPlugin() override;

    //IMainWindow
    virtual void showWindow(const bool show) override;

private:
    MainWindow *m_pMainWindow = nullptr;
};

#endif // MAINWINDOWPLUGIN_H
