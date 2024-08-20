#ifndef FIRETRIANGLEENTITY_H
#define FIRETRIANGLEENTITY_H


class FireTriangleEntity
{
public:
    FireTriangleEntity(float ttlf, float ttlf_x, float ttlf_y);

    float getTTLF() const;
    void setTTLF(float newTtlf);
    float getTTLFX() const;
    void setTTLFX(float newTtlf_x);
    float getTTLFY() const;
    void setTTLFY(float newTtlf_y);

private:
    float _ttlf;
    float _ttlf_x;
    float _ttlf_y;
};

#endif // FIRETRIANGLEENTITY_H
