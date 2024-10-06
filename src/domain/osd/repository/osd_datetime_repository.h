#ifndef OSD_DATETIME_REPOSITORY_H
#define OSD_DATETIME_REPOSITORY_H

#include "osd_base_repository.h"
#include "src/domain/osd/entity/osd_datetime_entity.h"

class DateTimeRepository: public OSDBaseRepository
{
public:
    virtual void SetDateTime(const DateTimeEntity &date_time) = 0;
    virtual const DateTimeEntity *GetDateTime() const = 0;
};

#endif // OSD_DATETIME_REPOSITORY_H
