#ifndef FIRE_TRIANGLE_REPOSITORY_H
#define FIRE_TRIANGLE_REPOSITORY_H

#include "src/domain/fire_triangle/entity/fire_triangle_entity.h"

class FireTriangleRepository
{
public:
    virtual void SetFireTriangle(const FireTriangleEntity &fte) = 0;
    virtual const FireTriangleEntity* GetFireTriangle() const = 0;
};

#endif // FIRE_TRIANGLE_REPOSITORY_H
