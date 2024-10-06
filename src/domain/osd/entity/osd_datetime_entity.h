#ifndef OSD_DATETIME_ENTITY_H
#define OSD_DATETIME_ENTITY_H

#include "osd_base_entity.h"

class DateTimeEntity: public OSDBaseEntity
{
public:
    DateTimeEntity(const std::string dateTimeLocal, const std::string dateTimeUTC, const std::string &source, const std::string &status, const OSD_MODE &mode);

    std::string dateTimeLocal() const;
    void setDatetimeLocal(const std::string &newDateTimeLocal);
    std::string dateTimeUTC() const;
    void setDateTimeUTC(const std::string &newDateTimeUTC);

private:
    std::string _dateTimeLocal;
    std::string _dateTimeUTC;
};

#endif // OSD_DATETIME_ENTITY_H
