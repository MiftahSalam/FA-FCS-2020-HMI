#ifndef FIRETRIANGLEREPOSITORYINMEMIMPL_H
#define FIRETRIANGLEREPOSITORYINMEMIMPL_H

#include "src/domain/fire_triangle/repository/fire_triangle_repository.h"

class FireTriangleRepositoryInMemImpl: public FireTriangleRepository
{
public:
    static FireTriangleRepository *GetInstance();

    // FireTriangleRepository interface
    void SetFireTriangle(const FireTriangleEntity &fte) override;
    const FireTriangleEntity *GetFireTriangle() const override;

protected:
    FireTriangleRepositoryInMemImpl(FireTriangleEntity *entity);

private:
    static FireTriangleRepositoryInMemImpl *instance;
    FireTriangleEntity *_entity;

};

#endif // FIRETRIANGLEREPOSITORYINMEMIMPL_H
