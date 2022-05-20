#include "MainWindow.h"

#include <QApplication>
#include "PluginManager.h"
#include "LDB.h"
#include "LZLib.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    LDB::init();
    PluginManager::manager()->init();
    Plugin::InitConnections(PluginManager::manager());
    PluginManager::manager()->start();
    Plugin::MainWindowPlugin()->showWindow(true);

    //MainWindow w;
    //w.show();
    int ret = a.exec();

    PluginManager::manager()->stop();

    return ret;
}
