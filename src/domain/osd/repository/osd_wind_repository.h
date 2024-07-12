#ifndef OSD_WIND_REPOSITORY_H
#define OSD_WIND_REPOSITORY_H

#include "osd_base_repository.h"
#include "src/domain/osd/entity/osd_wind_entity.h"

class OSDWindRepository: public OSDBaseRepository
{
public:
    virtual void SetWind(const OSDWindEntity &wind) = 0;
    virtual const OSDWindEntity* GetWind() const = 0;

};

#endif // OSDWINDREPOSITORY_H
