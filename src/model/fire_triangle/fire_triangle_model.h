#ifndef FIRE_TRIANGLE_MODEL_H
#define FIRE_TRIANGLE_MODEL_H

#include <QJsonObject>

class FireTriangleModel
{
public:
    FireTriangleModel(double ttlf, double ttlf_x, double ttlf_y);
    FireTriangleModel(){}

    static FireTriangleModel fromJsonObject(QJsonObject obj);

    double getTtlf() const;
    double getTtlfX() const;
    double getTtlfY() const;

private:
    double _ttlf, _ttlfX, _ttlfY;
};

#endif // FIRE_TRIANGLE_MODEL_H
