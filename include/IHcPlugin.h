#ifndef HCINTERFACE_IHCPLUGINBASE_H
#define HCINTERFACE_IHCPLUGINBASE_H

#include <IHcApplication.h>
#include <QObject>

class IHcPlugin {
public:
    virtual ~IHcPlugin() = default;

    virtual auto main(
        IHcApplication* HcApplication
    ) -> void = 0;
};

#define IID_IHcPlugin "org.infinitycurve.IHcPlugin"
Q_DECLARE_INTERFACE( IHcPlugin, IID_IHcPlugin );

#endif //HCINTERFACE_IHCPLUGINBASE_H
