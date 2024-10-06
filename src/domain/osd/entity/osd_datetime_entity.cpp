#include "osd_datetime_entity.h"


DateTimeEntity::DateTimeEntity(const std::string dateTimeLocal, const std::string dateTimeUTC, const std::string &source, const std::string &status, const OSD_MODE &mode)
    :OSDBaseEntity(source, status, mode), _dateTimeLocal(dateTimeLocal), _dateTimeUTC(dateTimeUTC)

{

}

std::string DateTimeEntity::dateTimeLocal() const
{
    return _dateTimeLocal;
}

void DateTimeEntity::setDatetimeLocal(const std::string &newDateTimeLocal)
{
    _dateTimeLocal = newDateTimeLocal;
}

std::string DateTimeEntity::dateTimeUTC() const
{
    return _dateTimeUTC;
}

void DateTimeEntity::setDateTimeUTC(const std::string &newDateTimeUTC)
{
    _dateTimeUTC = newDateTimeUTC;
}

