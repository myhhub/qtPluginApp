
#include "widget.h"
#include <QApplication>
#include <QDir>
#include <QPluginLoader>
#include <QObject>
#include <QJsonObject>
#include <QJsonArray>
#include "interfaceplugin.h"
#include "../qtpluginmanager/qtpluginmanager.h"
#include "../qtpluginmanager/plugininterface.h"
#include "../GenericPlugin1/genericplugin1.h"
#include "monitor.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);



    //加载exe所在目录下  plugin文件夹的所有插件
    QDir path = QDir(qApp->applicationDirPath());
    path.cd("plugins");
    foreach (QFileInfo info, path.entryInfoList(QDir::Files | QDir::NoDotAndDotDot))
    {
        QPluginLoader pluginLoader(info.absoluteFilePath());
        QObject *plugin = pluginLoader.instance();
        if (plugin)
        {
            // 获取元数据（名称、版本、依赖）
            QJsonObject json = pluginLoader.metaData().value("MetaData").toObject();
            qDebug() << "********** MetaData **********";
            qDebug() << json.value("author").toVariant();
            qDebug() << json.value("date").toVariant();
            qDebug() << json.value("name").toVariant();
            qDebug() << json.value("version").toVariant();
            qDebug() << json.value("dependencies").toArray().toVariantList();

            //访问感兴趣的接口
            InterfacePlugin *app = qobject_cast<InterfacePlugin*>(plugin);
            if (app)
            {
                app->output("i am a plugin");
            }

        }
    }


    /////////----------------------

    QtPluginsManager::instance()->loadAllPlugins();//插件管理器 加载所有插件
    auto plugins=QtPluginsManager::instance()->allPlugins();
    foreach(auto plugin,plugins)
    {
        PluginInterface *app = qobject_cast<PluginInterface*>(plugin->instance());
        if (app)
        {
            QMap<QString,QVariant> data;
            data.insert("0001",10000);
            app->setInitData(data);
        }
    }


    QtPluginsManager::instance()->unloadAllPlugins();


//    Widget w;
//    w.show();

//    return a.exec();
}
