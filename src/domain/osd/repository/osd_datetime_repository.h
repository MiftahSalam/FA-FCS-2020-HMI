#ifndef OSD_DATETIME_REPOSITORY_H
#define OSD_DATETIME_REPOSITORY_H

#include "osd_base_repository.h"
#include "src/domain/osd/entity/osd_datetime_entity.h"

class OSDDateTimeRepository: public OSDBaseRepository
{
public:
    virtual void SetDateTime(const OSDDateTimeEntity &date_time) = 0;
    virtual const OSDDateTimeEntity *GetDateTime() const = 0;
    virtual void UpdateTimeLocalDisplay(const long long &epoch) = 0;
    virtual void UpdateTimeDisplay(const long long &epoch) = 0;
};

#endif // OSD_DATETIME_REPOSITORY_H
