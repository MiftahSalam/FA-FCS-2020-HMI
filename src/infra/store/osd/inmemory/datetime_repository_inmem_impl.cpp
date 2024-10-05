#include "datetime_repository_inmem_impl.h"

DateTimeRepositoryInMemImpl* DateTimeRepositoryInMemImpl::instance(nullptr);

DateTimeRepository *DateTimeRepositoryInMemImpl::GetInstance()
{
    if (instance == nullptr){
        DateTimeEntity *entity = new DateTimeEntity("","","","", OSD_MODE::AUTO);
        instance = new DateTimeRepositoryInMemImpl(entity);
    }

    return instance;
}

void DateTimeRepositoryInMemImpl::SetSource(const std::string &source)
{
    _entity->setSource(source);
}

void DateTimeRepositoryInMemImpl::SetStatus(const std::string &status)
{
    _entity->setStatus(status);
}

void DateTimeRepositoryInMemImpl::SetMode(const OSD_MODE &mode)
{

}

void DateTimeRepositoryInMemImpl::SetDateTime(const DateTimeEntity &date_time)
{
    _entity->setDatetimeLocal(date_time.dateTimeLocal());
    _entity->setDateTimeUTC(date_time.dateTimeUTC());
    _entity->setMode(date_time.mode());
    _entity->setSource(date_time.source());
    _entity->setStatus(date_time.status());
}

const DateTimeEntity *DateTimeRepositoryInMemImpl::GetDateTime() const
{
    return _entity;
}

DateTimeRepositoryInMemImpl::DateTimeRepositoryInMemImpl(DateTimeEntity *entity)
{
    _entity = entity;
}

