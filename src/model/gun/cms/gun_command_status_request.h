#ifndef GUNCOMMANDSTATUSREQUEST_H
#define GUNCOMMANDSTATUSREQUEST_H

#include "src/model/base_request.h"

class GunCommandStatusRequest: public BaseModel<GunCommandStatusRequest>
{
public:
    GunCommandStatusRequest();
    GunCommandStatusRequest(bool mount, bool singleShot, bool fireOrder, bool proxFuze, bool siren);

    // BaseModel interface
    QByteArray toJSON() override;

    bool getMount() const;
    void setMount(bool newMount);
    bool getSingleShot() const;
    void setSingleShot(bool newSingleShot);
    bool getFireOrder() const;
    void setFireOrder(bool newFireOrder);
    bool getProxFuze() const;
    void setProxFuze(bool newProxFuze);
    bool getSiren() const;
    void setSiren(bool newSiren);

private:
    bool mount;
    bool singleShot;
    bool fireOrder;
    bool proxFuze;
    bool siren;
};

#endif // GUNCOMMANDSTATUSREQUEST_H
