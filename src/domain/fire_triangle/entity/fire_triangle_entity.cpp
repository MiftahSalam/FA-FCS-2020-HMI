#include "fire_triangle_entity.h"

FireTriangleEntity::FireTriangleEntity(float ttlf, float ttlf_x, float ttlf_y) : _ttlf(ttlf),
    _ttlf_x(ttlf_x),
    _ttlf_y(ttlf_y)
{}

float FireTriangleEntity::getTTLF() const
{
    return _ttlf;
}

void FireTriangleEntity::setTTLF(float newTtlf)
{
    _ttlf = newTtlf;
}

float FireTriangleEntity::getTTLFX() const
{
    return _ttlf_x;
}

void FireTriangleEntity::setTTLFX(float newTtlf_x)
{
    _ttlf_x = newTtlf_x;
}

float FireTriangleEntity::getTTLFY() const
{
    return _ttlf_y;
}

void FireTriangleEntity::setTTLFY(float newTtlf_y)
{
    _ttlf_y = newTtlf_y;
}
