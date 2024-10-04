#ifndef DATETIME_ENTITY_H
#define DATETIME_ENTITY_H

#include "osd_base_entity.h"

class DateTimeEntity: public OSDBaseEntity
{
public:
    DateTimeEntity(float date, float time, const std::string &source, const std::string &status, const OSD_MODE &mode);

    float date() const;
    void setDate(float newDate);
    float time() const;
    void setTime(float newTime);

private:
    float _date;
    float _time;
};

#endif // DATETIME_ENTITY_H
