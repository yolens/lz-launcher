#include "MainWindowPlugin.h"

MainWindowPlugin::MainWindowPlugin(QObject *parent)
{
    m_pMainWindow = new MainWindow();
    m_pMainWindow->hide();
}

QObject* MainWindowPlugin::instance()
{
    return this;
}
QUuid MainWindowPlugin::pluginUuid() const
{
    return MAINWINDOW_UUID;
}
void MainWindowPlugin::pluginInfo(IPluginInfo *pluginInfo)
{
    pluginInfo->author = "yolens";
    pluginInfo->name = "MainWindowPlugin";
    pluginInfo->version = "1.0.0";
}
bool MainWindowPlugin::initConnections(IPluginManager *pluginManager, int& initOrder)
{
    Q_UNUSED(pluginManager);
    initOrder = 1;
    return true;
}
bool MainWindowPlugin::initObjects()
{
    return true;
}
bool MainWindowPlugin::initSettings()
{
    return true;
}
bool MainWindowPlugin::startPlugin()
{
    return true;
}
bool MainWindowPlugin::stopPlugin()
{
    return true;
}

void MainWindowPlugin::showWindow(const bool show)
{
    m_pMainWindow->setVisible(show);
}
