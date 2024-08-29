#include "fire_triangle_model.h"

FireTriangleModel::FireTriangleModel(double ttlf, double ttlf_x, double ttlf_y):
    _ttlf(ttlf), _ttlfX(ttlf_x), _ttlfY(ttlf_y)
{

}

FireTriangleModel FireTriangleModel::fromJsonObject(QJsonObject obj)
{
    FireTriangleModel model(
        obj.value("ttlf").toDouble(),
        obj.value("ttlf_x").toDouble(),
        obj.value("ttlf_y").toDouble()
        );
    return model;
}

double FireTriangleModel::getTtlf() const
{
    return _ttlf;
}

double FireTriangleModel::getTtlfX() const
{
    return _ttlfX;
}

double FireTriangleModel::getTtlfY() const
{
    return _ttlfY;
}
