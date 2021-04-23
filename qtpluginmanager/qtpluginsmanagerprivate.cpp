#include "qtpluginsmanagerprivate.h"
#include <QVariantMap>
#include <QDebug>
//#include <QtWidgets/QMessageBox>

bool QtPluginsManagerPrivate::check(const QString &filepath)
{
    for(QVariant item : m_dependencies.value(filepath))
    {
        QVariantMap map = item.toMap();
        //依赖的插件名称、版本、路径
        QVariant name = map.value("name");
        QVariant version = map.value("version");
        QString path = m_names.key(name);

        /********** 检测插件是否依赖于其他插件 **********/
        // 先检测插件名称
        if(!m_names.values().contains(name))
        {
            QString strcons = "Missing dependency: "+ name.toString()+" for plugin "+path;
            qDebug()<<Q_FUNC_INFO<<strcons;
            //QMessageBox::warning(nullptr, ("Plugins Loader Error"), strcons, QMessageBox::Ok);
            return false;
        }
        //再检测插件版本
        if(m_versions.value(path) != version)
        {
            QString strcons = "Version mismatch: " + name.toString() +" version "+m_versions.value(m_names.key(name)).toString()+
                              " but " + version.toString() + " required for plugin "+path;
            qDebug()<<Q_FUNC_INFO<<strcons;
            //QMessageBox::warning(nullptr, "Plugins Loader Error", strcons, QMessageBox::Ok);
            return false;
        }
        //最后检测被依赖的插件是否还依赖其他的插件
        if(!check(path))
        {
            QString strcons = "Corrupted dependency: "+name.toString()+" for plugin "+path;
            qDebug()<<Q_FUNC_INFO<<strcons;
            //QMessageBox::warning(nullptr, "Plugins Loader Error", strcons, QMessageBox::Ok);
            return false;
        }
    }

    return true;
}
