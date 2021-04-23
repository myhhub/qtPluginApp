#ifndef GENERICPLUGIN2_H
#define GENERICPLUGIN2_H

#include <QObject>
#include <QtPlugin>
#include "../qtpluginmanager/pluginmetadata.h"
#include "../qtpluginmanager/plugininterface.h"

class GenericPlugin2 : public QObject, public PluginInterface
{
    // programmer.json 插件的信息描述类
    Q_OBJECT
    Q_PLUGIN_METADATA(IID PluginInterface_iid FILE "programmer.json") // QT5.0 引入
    Q_INTERFACES(PluginInterface)
public:
    GenericPlugin2() {}
    ~GenericPlugin2() {}

    void setInitData(QMap<QString,QVariant> &data) Q_DECL_OVERRIDE {
        qDebug() << "GenericPlugin2 setInitDat..." << data;
        map=&data;
    }

    QMap<QString,QVariant>& getResultData() Q_DECL_OVERRIDE {
        qDebug() << "GenericPlugin2 getResultData...";
        return *map;
    }


    void recMsgFromManager(PluginMetaData &msg) Q_DECL_OVERRIDE {
        qDebug() << "GenericPlugin2 recMsgFromManager..." << msg.from;
    }

    bool connectTosendMsgToManager(QObject *receiver, const char *pszSlot, bool isConnect) const Q_DECL_OVERRIDE {
        if(isConnect)
            return connect(this, SIGNAL(sendMsgToManager(PluginMetaData&)), receiver, pszSlot);
        else
            return disconnect(this, SIGNAL(sendMsgToManager(PluginMetaData&)), receiver, pszSlot);
    }

private:
    QMap<QString,QVariant> *map;

signals:
    void sendMsgToManager(PluginMetaData&);

};

#endif // GENERICPLUGIN2_H
