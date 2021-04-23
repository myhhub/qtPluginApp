#ifndef INTERFACEPLUGIN_H
#define INTERFACEPLUGIN_H

#include <QString>
#include <QtPlugin>

//定义接口
class InterfacePlugin
{
public:
    virtual ~InterfacePlugin() {}
    virtual QString output(const QString &message) = 0;
};

#define InterfacePlugin_iid "Test.Plugin.InterfacePlugin"   // 唯一标识符

Q_DECLARE_INTERFACE(InterfacePlugin, InterfacePlugin_iid)

#endif
