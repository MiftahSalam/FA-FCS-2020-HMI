#include "fire_triangle_repository.h"
#include "src/infra/store/fire_triangle/inmemory/fire_triangle_repository_inmem_impl.h"

FireTriangleRepository::FireTriangleRepository(QObject *parent)
    : QObject{parent}
{
    repoFT40mm = FireTriangleRepositoryInMemImpl::GetInstance();
}

FireTriangleBaseRepository *FireTriangleRepository::getRepoFT40mm() const
{
    return repoFT40mm;
}
