#ifndef INPUTMODEMODEL_H
#define INPUTMODEMODEL_H

#include "qjsonobject.h"

class InputModeModel
{
public:
    InputModeModel(bool position, bool speed, bool inersia, bool waterspeed,
                   bool wind, bool weather);
    InputModeModel(){};

    static InputModeModel fromJsonObject(QJsonObject obj);

    bool getPosition() const;
    bool getSpeed() const;
    bool getInersia() const;
    bool getWaterSpeed() const;
    bool getWind() const;
    bool getWeather() const;

private:
    bool position;
    bool speed;
    bool inersia;
    bool waterspeed;
    bool wind;
    bool weather;

};

#endif // INPUTMODEMODEL_H
