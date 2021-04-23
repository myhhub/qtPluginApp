#include "qtpluginmanager.h"
#include "qtpluginsmanagerprivate.h"
#include "plugininterface.h"
#include <QDir>
#include <QCoreApplication>
#include <QJsonArray>
#include <QDebug>

QtPluginsManager* QtPluginsManager::m_instance=nullptr;

QtPluginsManager::QtPluginsManager()
{
    d = new QtPluginsManagerPrivate();
}

QtPluginsManager::~QtPluginsManager()
{
    if(d)
        delete d;
}

void QtPluginsManager::loadAllPlugins()
{
    QDir pluginsdir = QDir(qApp->applicationDirPath());
    pluginsdir.cd("plugins");

    QFileInfoList pluginsInfo = pluginsdir.entryInfoList(QDir::Files | QDir::NoDotAndDotDot);
    //初始化插件中的元数据
    for(QFileInfo fileinfo : pluginsInfo)
        scanMetaData(fileinfo.absoluteFilePath());

    //加载插件
    for(QFileInfo fileinfo : pluginsInfo)
        loadPlugin(fileinfo.absoluteFilePath());
}

void QtPluginsManager::scanMetaData(const QString &filepath)
{
    //判断是否为库（后缀有效性）
    if(!QLibrary::isLibrary(filepath))
        return ;
    //获取元数据
    QPluginLoader *loader = new QPluginLoader(filepath);
    QJsonObject json = loader->metaData().value("MetaData").toObject();

    QVariant var = json.value("name").toVariant();
    d->m_names.insert(filepath, json.value("name").toVariant());
    d->m_versions.insert(filepath, json.value("version").toVariant());
    d->m_dependencies.insert(filepath, json.value("dependencies").toArray().toVariantList());

    delete loader;
    loader = nullptr;
}

void QtPluginsManager::loadPlugin(const QString &filepath)
{
    if(!QLibrary::isLibrary(filepath))
        return;

    //检测依赖
    if(!d->check(filepath))
        return;

    //加载插件
    QPluginLoader *loader = new QPluginLoader(filepath);
    if(loader->load())
    {
        // 如果继承自 Plugin，则认为是自己的插件（防止外部插件注入）。
        PluginInterface *plugin = qobject_cast<PluginInterface *>(loader->instance());
        if(plugin)
        {
            d->m_loaders.insert(filepath, loader);
            plugin->connectTosendMsgToManager(this, SLOT(recMsgFromManager(PluginMetaData&)), true);
        }
        else
        {
            delete loader;
            loader = nullptr;
        }
    }
}

void QtPluginsManager::unloadAllPlugins()
{
    for(QString filepath : d->m_loaders.keys())
        unloadPlugin(filepath);
}

void QtPluginsManager::unloadPlugin(const QString &filepath)
{
    QPluginLoader *loader = d->m_loaders.value(filepath);
    //卸载插件，并从内部数据结构中移除
    if(loader->unload())
    {
        d->m_loaders.remove(filepath);
        delete loader;
        loader = nullptr;
    }
}

QList<QPluginLoader *> QtPluginsManager::allPlugins()
{
    return d->m_loaders.values();
}

QPluginLoader* QtPluginsManager::getPlugin(const QString &name)
{
    return d->m_loaders.value(d->m_names.key(name));
}

QVariant QtPluginsManager::getPluginName(QPluginLoader *loader)
{
    if(loader)
        return d->m_names.value(d->m_loaders.key(loader));
    else
        return "";
}

void QtPluginsManager::recMsgFromManager(PluginMetaData &msg)
{
     qDebug()  <<"QtPluginsManager::recMsgFromManager..."<< msg.dest;
     auto loader = getPlugin(msg.dest);
     if(loader)
     {
         auto plugin = qobject_cast<PluginInterface*>(loader->instance());;
         if(plugin)
         {
             plugin->recMsgFromManager(msg);
         }
     }
}
