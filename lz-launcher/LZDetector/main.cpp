#include "MainWindow.h"

#include <QApplication>
#include "PluginManager.h"
#include "LDB.h"
#include "LZLib.h"
#include <QTextCodec>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForLocale(codec);

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
