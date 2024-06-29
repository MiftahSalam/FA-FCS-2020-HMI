#include "osd_base_repository.h"

//OSDBaseEntity *OSDBaseRepository::_entity{nullptr};

OSDBaseRepository::OSDBaseRepository(OSDBaseEntity *entity)
{
    _entity = entity;
}

void OSDBaseRepository::SetEntity(const OSDBaseEntity &entity) {
    _entity->setManualMode(entity.manualMode());
    _entity->setSource(entity.source());
    _entity->setStatus(entity.status());
};
