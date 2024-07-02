#ifndef OSD_POSITION_REPOSITORY_H
#define OSD_POSITION_REPOSITORY_H

#include "osd_base_repository.h"
#include "src/domain/osd/entity/osd_position_entity.h"

class OSDPositionRepository: public OSDBaseRepository
{
public:
    virtual void SetPosition(const OSDPositionEntity &pos) = 0;
    virtual const OSDPositionEntity* GetPosition() const = 0;

};

#endif // OSD_POSITION_REPOSITORY_H
