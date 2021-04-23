#ifndef GENERICPLUGIN1_H
#define GENERICPLUGIN1_H

#include <QObject>
#include <QtPlugin>
#include "../qtpluginmanager/pluginmetadata.h"
#include "../qtpluginmanager/plugininterface.h"

class GenericPlugin1 : public QObject, public PluginInterface
{
    // programmer.json 插件的信息描述类
    Q_OBJECT
    Q_PLUGIN_METADATA(IID PluginInterface_iid FILE "programmer.json") // QT5.0 引入
    Q_INTERFACES(PluginInterface)
public:
    GenericPlugin1() {}
    ~GenericPlugin1() {}

    void setInitData(QMap<QString,QVariant> &data) Q_DECL_OVERRIDE {
        qDebug() << "GenericPlugin1 setInitDat..." << data;
        map=&data;

        PluginMetaData metadata;
        metadata.from = "GenericPlugin1";
        metadata.dest = "GenericPlugin2";
        metadata.type = 1;
        emit sendMsgToManager(metadata);
    }

    QMap<QString,QVariant>& getResultData() Q_DECL_OVERRIDE {
        qDebug() << "GenericPlugin1 getResultData...";
        return *map;
    }


    void recMsgFromManager(PluginMetaData &msg) Q_DECL_OVERRIDE {
        qDebug() << "GenericPlugin1 recMsgFromManager..." << msg.from;
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

#endif // GENERICPLUGIN1_H
