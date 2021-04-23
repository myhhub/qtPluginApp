#include "plugina.h"
#include <QtDebug>

PluginA::PluginA(QObject *parent) :
    QObject(parent)
{
}

QString PluginA::output(const QString &message)
{
    qDebug() << message + "插件A加载成功";
    return message;
}
