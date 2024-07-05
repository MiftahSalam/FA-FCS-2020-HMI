#ifndef GUNCOMMANDREQUEST_H
#define GUNCOMMANDREQUEST_H

#include "src/model/base_request.h"

class GunCommandRequest: public BaseModel<GunCommandRequest>
{
public:
    GunCommandRequest();

    // BaseModel interface
public:
    QByteArray toJSON() override;
};

#endif // GUNCOMMANDREQUEST_H
