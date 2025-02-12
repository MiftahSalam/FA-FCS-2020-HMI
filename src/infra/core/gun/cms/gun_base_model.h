#ifndef GUN_BASE_MODEL_H
#define GUN_BASE_MODEL_H

#include "src/shared/common/errors/err_base.h"

class GunBaseModel
{
public:
    GunBaseModel();
    GunBaseModel(const BaseError &err);

    BaseError err() const;
    void setErr(const BaseError &newErr);

protected:
    BaseError _err;
};

#endif // GUN_BASE_MODEL_H
