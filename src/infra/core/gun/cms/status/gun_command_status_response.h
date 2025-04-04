#ifndef GUNCOMMANDSTATUSRESPONSE_H
#define GUNCOMMANDSTATUSRESPONSE_H

#include <QJsonObject>

#include "src/infra/core/gun/cms/gun_base_model.h"

class GunCommandStatusResponse: public GunBaseModel
{
public:
    GunCommandStatusResponse();
    GunCommandStatusResponse(bool mount, bool singleShot, bool fireOrder, bool notFireOrder, bool proxFuze, bool siren);

    static GunCommandStatusResponse FromJsonObject(QJsonObject obj);

    bool getMount() const;
    bool getSingleShot() const;
    bool getFireOrder() const;
    bool getProxFuze() const;
    bool getSiren() const;

private:
    bool mount;
    bool singleShot;
    bool fireOrder;
    bool notFireOrder;
    bool proxFuze;
    bool siren;
};

#endif // GUNCOMMANDSTATUSRESPONSE_H
