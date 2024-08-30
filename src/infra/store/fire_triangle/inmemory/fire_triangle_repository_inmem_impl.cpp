#include "fire_triangle_repository_inmem_impl.h"

FireTriangleRepositoryInMemImpl* FireTriangleRepositoryInMemImpl::instance(nullptr);

FireTriangleRepositoryInMemImpl::FireTriangleRepositoryInMemImpl(FireTriangleEntity *entity)
{
    _entity = entity;
}

FireTriangleBaseRepository *FireTriangleRepositoryInMemImpl::GetInstance()
{
    if (instance == nullptr) {
        FireTriangleEntity* entity = new FireTriangleEntity(0, 0, 0);
        instance = new FireTriangleRepositoryInMemImpl (entity);
    }
    return instance;
}

void FireTriangleRepositoryInMemImpl::SetFireTriangle(const FireTriangleEntity &fte)
{
    _entity->setTTLF(fte.getTTLF());
    _entity->setTTLFX(fte.getTTLFX());
    _entity->setTTLFY(fte.getTTLFY());
}

const FireTriangleEntity *FireTriangleRepositoryInMemImpl::GetFireTriangle() const
{
    return _entity;
}
