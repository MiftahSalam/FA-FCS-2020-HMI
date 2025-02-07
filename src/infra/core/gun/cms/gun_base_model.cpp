#include "gun_base_model.h"

GunBaseModel::GunBaseModel() : _err(ErrUnknown()) {}

GunBaseModel::GunBaseModel(const BaseError &err) : _err(err)
{

}

BaseError GunBaseModel::err() const
{
    return _err;
}

void GunBaseModel::setErr(const BaseError &newErr)
{
    _err = newErr;
}
