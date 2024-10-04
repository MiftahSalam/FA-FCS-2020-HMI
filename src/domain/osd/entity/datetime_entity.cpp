#include "datetime_entity.h"

DateTimeEntity::DateTimeEntity(float date, float time, const std::string &source, const std::string &status, const OSD_MODE &mode)
    : OSDBaseEntity(source, status, mode), _date(date),_time(time)
{

}

float DateTimeEntity::date() const
{
    return _date;
}

void DateTimeEntity::setDate(float newDate)
{
    _date = newDate;
}

float DateTimeEntity::time() const
{
    return _time;
}

void DateTimeEntity::setTime(float newTime)
{
    _time = newTime;
}
