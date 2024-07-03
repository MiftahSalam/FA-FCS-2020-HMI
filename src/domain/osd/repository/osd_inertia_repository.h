#ifndef OSD_INERTIA_REPOSITORY_H
#define OSD_INERTIA_REPOSITORY_H

#include "osd_base_repository.h"
#include "src/domain/osd/entity/osd_inertia_entity.h"

class OSDInertiaRepository: public OSDBaseRepository
{
public:
    virtual void SetInertia(const OSDInertiaEntity &pos) = 0;
    virtual const OSDInertiaEntity* GetInertia() const = 0;
};

#endif // OSD_INERTIA_REPOSITORY_H
