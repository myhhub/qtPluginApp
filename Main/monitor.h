#ifndef MONITOR_H
#define MONITOR_H

#include <QObject>
#include <QtDebug>
#include <../qtpluginmanager/plugininterface.h>

class Monitor : public QObject
{
    Q_OBJECT

public slots:
    void recMsgFromManager(PluginMetaData &msg){
        qDebug()  <<"Information..."<< msg.from;
    }

public:
    void monitorInformation(PluginInterface *plugin) {
        bool isConnect = plugin->connectTosendMsgToManager(this, SLOT(recMsgFromManager(PluginMetaData&)), true);
        if (isConnect) {
            qWarning() << "connectToInformation";
        } else {
            qWarning() << "cannot monitor Information";
        }
    }
};

#endif // MONITOR_H
