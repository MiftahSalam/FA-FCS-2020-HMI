#include "osd_base_repository_inmem_impl.h"

OSDBaseRepositoryInMemImpl::OSDBaseRepositoryInMemImpl(OSDBaseEntity *entity)
{
    _entity = entity;
}

void OSDBaseRepositoryInMemImpl::SetEntity(const OSDBaseEntity &entity) {
    _entity->setMode(entity.mode());
    _entity->setSource(entity.source());
    _entity->setStatus(entity.status());
};
