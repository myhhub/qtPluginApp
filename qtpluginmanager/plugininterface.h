#ifndef PLUGININTERFACE_H
#define PLUGININTERFACE_H
#include "pluginmetadata.h"
class PluginInterface
{
public:
    virtual ~PluginInterface() {}

public:

    virtual void setInitData(QMap<QString,QVariant> &data) = 0;

    virtual QMap<QString,QVariant>& getResultData() = 0;

    virtual void recMsgFromManager(PluginMetaData &msg) = 0;

    //connect to signals sendMsgToManager
    //virtual void sendMsgToManager(PluginMetaData &msg) = 0;
    virtual bool connectTosendMsgToManager(QObject *receiver, const char* pszSlot, bool isConnect = true) const = 0;
};
#define PluginInterface_iid "QtPlugins.QtPluginsManager.PluginInterface"

Q_DECLARE_INTERFACE(PluginInterface, PluginInterface_iid)

#endif // PLUGININTERFACE_H
