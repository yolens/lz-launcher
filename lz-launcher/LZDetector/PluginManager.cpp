#include "PluginManager.h"
#include <QUuid>
#include <QDir>
#include <QLibrary>


const QString PLUGINS_DIR = "./plugins";

PluginManager::PluginManager(QObject *parent)
    : QObject{parent}
{

}

PluginManager* PluginManager::manager()
{
    static PluginManager ins;
    return &ins;
}

void PluginManager::start()
{
    loadPlugins();
    initPlugins();
    startPlugins();
}
void PluginManager::stop()
{
    unloadPlugins();
}

QObject* PluginManager::instance()
{
    return this;
}

QString PluginManager::version()
{
    return "0.0.0.1";
}

IPlugin* PluginManager::pluginInstance(const QUuid &uid)
{
    return m_pluginMap.contains(uid.toString())
           ? m_pluginMap.value(uid.toString()).plugin
           : nullptr;
}

const IPlugin::IPluginInfo* PluginManager::pluginInfo(const QUuid &uid)
{
    return m_pluginMap.contains(uid.toString())
           ? m_pluginMap.value(uid.toString()).info
           : nullptr;
}

bool PluginManager::loadPlugins()
{
    findFiles(PLUGINS_DIR);
    return !m_pluginMap.isEmpty();
}

bool PluginManager::initPlugins()
{
    m_releaseItems.clear();
    QMultiMap<int, IPlugin*> pluginOrder;
    QHash<QString, PluginItem>::iterator it = m_pluginMap.begin();
    for(; it != m_pluginMap.end();)
    {
        int initOrder = 500;
        IPlugin* plugin = it.value().plugin;
        if(plugin->initConnections(this, initOrder))
        {
            pluginOrder.insertMulti(initOrder, plugin);
            ++it;
        }
        else
        {
            PluginItem pluginItem = it.value();
            m_releaseItems.push_back(pluginItem);
            it = m_pluginMap.erase(it);
        }
    }//end while

    foreach(IPlugin* plugin, pluginOrder)
        plugin->initObjects();
    foreach(IPlugin* plugin, pluginOrder)
        plugin->initSettings();

    return true;
}

bool PluginManager::startPlugins()
{
    bool allStarted = true;
    foreach(const PluginItem &pluginItem, m_pluginMap)
    {
        bool started = pluginItem.plugin->startPlugin();
        allStarted = allStarted && started;
    }

    return allStarted;
}

void PluginManager::unloadPlugins()
{
    foreach(const QUuid &uid, m_pluginMap.keys())
        unloadPlugin(uid);

    foreach (PluginItem pluginItem, m_releaseItems) {
        delete pluginItem.info;
        delete pluginItem.loader;
    }
    m_releaseItems.clear();
}

bool PluginManager::findFiles(const QString& path)
{
    QDir pluginsDir(path);
    pluginsDir.setFilter(QDir::Dirs | QDir::Files);
    pluginsDir.setSorting(QDir::DirsFirst);//文件夹排在前面
    QFileInfoList fileInfoList = pluginsDir.entryInfoList();
    foreach (QFileInfo fileInfo, fileInfoList )
    {
        if (fileInfo.fileName() == "." || fileInfo.fileName() == "..")
            continue;
        if (fileInfo.isDir())
        {
            if (!findFiles(fileInfo.filePath()))
                return false;
            //continue;
        }
        else
        {
            QString file = fileInfo.absoluteFilePath();
            addPlugin(file);
        }
    }
    return true;
}

void PluginManager::addPlugin(const QString& file)
{
    if(QLibrary::isLibrary(file))
    {
        QPluginLoader* loader = new QPluginLoader(file, this);
        if(loader->load())
        {
            IPlugin* plugin = qobject_cast<IPlugin*>(loader->instance());
            if(plugin)
            {
                plugin->instance()->setParent(loader);
                QUuid uuid = plugin->pluginUuid();

                if(!m_pluginMap.contains(uuid.toString()))
                {
                    PluginItem pluginItem;
                    pluginItem.plugin = plugin;
                    pluginItem.loader = loader;
                    pluginItem.info = new IPlugin::IPluginInfo;

                    plugin->pluginInfo(pluginItem.info);
                    m_pluginMap.insert(uuid.toString(), pluginItem);
                }
                else
                {
                    delete loader;
                }
            }
            else
            {
                delete loader;
            }

        }
        else
        {
            delete loader;
        }
    }
}

void PluginManager::unloadPlugin(const QUuid& uid, const QString& error)
{
    if (m_pluginMap.contains(uid.toString()))
    {
        removePluginItem(uid, error);
    }
}

void PluginManager::removePluginItem(const QUuid& uid, const QString& error)
{
    if (m_pluginMap.contains(uid.toString()))
    {
        PluginItem pluginItem = m_pluginMap.take(uid.toString());
        if (!error.isEmpty())
        {

        }
        pluginItem.plugin->stopPlugin();
        m_releaseItems.push_back(pluginItem);
    }
}
