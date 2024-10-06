#include "osd_datetime_repository_inmem_impl.h"

OSDDateTimeRepositoryInMemImpl* OSDDateTimeRepositoryInMemImpl::instance(nullptr);

OSDDateTimeRepository *OSDDateTimeRepositoryInMemImpl::GetInstance()
{
    if (instance == nullptr){
        OSDDateTimeEntity *entity = new OSDDateTimeEntity("","","","", OSD_MODE::AUTO);
        instance = new OSDDateTimeRepositoryInMemImpl(entity);
    }

    return instance;
}

void OSDDateTimeRepositoryInMemImpl::SetSource(const std::string &source)
{
    _entity->setSource(source);
}

void OSDDateTimeRepositoryInMemImpl::SetStatus(const std::string &status)
{
    _entity->setStatus(status);
}

void OSDDateTimeRepositoryInMemImpl::SetMode(const OSD_MODE &mode)
{

}

void OSDDateTimeRepositoryInMemImpl::SetDateTime(const OSDDateTimeEntity &date_time)
{
    _entity->setDatetimeLocal(date_time.dateTimeLocal());
    _entity->setDateTimeUTC(date_time.dateTimeUTC());
    _entity->setMode(date_time.mode());
    _entity->setSource(date_time.source());
    _entity->setStatus(date_time.status());
}

const OSDDateTimeEntity *OSDDateTimeRepositoryInMemImpl::GetDateTime() const
{
    return _entity;
}

OSDDateTimeRepositoryInMemImpl::OSDDateTimeRepositoryInMemImpl(OSDDateTimeEntity *entity)
{
    _entity = entity;
}

