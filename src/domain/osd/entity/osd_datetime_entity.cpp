#include "osd_datetime_entity.h"


OSDDateTimeEntity::OSDDateTimeEntity(const std::string dateTimeLocal, const std::string dateTimeUTC, const std::string &source, const std::string &status, const OSD_MODE &mode)
    :OSDBaseEntity(source, status, mode), _dateTimeLocal(dateTimeLocal), _dateTimeUTC(dateTimeUTC)

{
    _dateTimeLocalProcessed = 0;
    _dateTimeProcessed = 0;
}

std::string OSDDateTimeEntity::dateTimeLocal() const
{
    return _dateTimeLocal;
}

void OSDDateTimeEntity::setDatetimeLocal(const std::string &newDateTimeLocal)
{
    _dateTimeLocal = newDateTimeLocal;
}

std::string OSDDateTimeEntity::dateTimeUTC() const
{
    return _dateTimeUTC;
}

void OSDDateTimeEntity::setDateTimeUTC(const std::string &newDateTimeUTC)
{
    _dateTimeUTC = newDateTimeUTC;
}

long long OSDDateTimeEntity::dateTimeLocalProcessed() const
{
    return _dateTimeLocalProcessed;
}

void OSDDateTimeEntity::setDateTimeLocalProcessed(long long newDateTimeLocalProcessed)
{
    _dateTimeLocalProcessed = newDateTimeLocalProcessed;
}

long long OSDDateTimeEntity::dateTimeProcessed() const
{
    return _dateTimeProcessed;
}

void OSDDateTimeEntity::setDateTimeProcessed(long long newDateTimeProcessed)
{
    _dateTimeProcessed = newDateTimeProcessed;
}

