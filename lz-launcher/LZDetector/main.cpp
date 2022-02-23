#include "MainWindow.h"

#include <QApplication>
#include "PluginManager.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    PluginManager::manager()->start();
    Plugin::InitConnections(PluginManager::manager());
    Plugin::MainWindowPlugin()->showWindow(true);

    //MainWindow w;
    //w.show();
    int ret = a.exec();

    PluginManager::manager()->stop();

    return ret;
}
