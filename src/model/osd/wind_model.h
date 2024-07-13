#ifndef WINDMODEL_H
#define WINDMODEL_H

#include <QJsonObject>

class WindModel
{
public:
    WindModel(double speed, double direction);
    WindModel(){};

    static WindModel FromJsonObject(QJsonObject obj);

    double getSpeed() const;
    double getDirection() const;

private:
    double speed, direction;
};


#endif // WINDMODEL_H
