#include "osd_datetime_repository_inmem_impl.h"
#include "qdatetime.h"

OSDDateTimeRepositoryInMemImpl* OSDDateTimeRepositoryInMemImpl::instance(nullptr);

OSDDateTimeRepository *OSDDateTimeRepositoryInMemImpl::GetInstance()
{
    if (instance == nullptr){
        OSDDateTimeEntity *entity = new OSDDateTimeEntity("","","","invalid", OSD_MODE::AUTO);
        entity->setDateTimeLocalProcessed(QDateTime::currentDateTime().toMSecsSinceEpoch());
        entity->setDateTimeProcessed(QDateTime::currentDateTimeUtc().toMSecsSinceEpoch());

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
    _entity->setDateTimeLocalProcessed(
                QDateTime::fromString(
                    QString::fromStdString(date_time.dateTimeLocal()), "dd-MM-yyyy hh:mm:ss").toMSecsSinceEpoch()
                );
    _entity->setDateTimeProcessed(
                QDateTime::fromString(
                    QString::fromStdString(date_time.dateTimeUTC()), "dd-MM-yyyy hh:mm:ss").toMSecsSinceEpoch()
                );
}

const OSDDateTimeEntity *OSDDateTimeRepositoryInMemImpl::GetDateTime() const
{
    return _entity;
}

void OSDDateTimeRepositoryInMemImpl::UpdateTimeLocalDisplay(const long long &epoch)
{
    if (_entity->dateTimeLocalProcessed() != 0) {
        _entity->setDateTimeLocalProcessed(epoch);
    }
}

void OSDDateTimeRepositoryInMemImpl::UpdateTimeDisplay(const long long &epoch)
{
    if (_entity->dateTimeProcessed() != 0) {
        _entity->setDateTimeProcessed(epoch);
    }
}

OSDDateTimeRepositoryInMemImpl::OSDDateTimeRepositoryInMemImpl(OSDDateTimeEntity *entity)
{
    _entity = entity;
}

