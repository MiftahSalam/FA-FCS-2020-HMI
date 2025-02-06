#ifndef InputModeResponseModel_H
#define InputModeResponseModel_H

#include "qjsonobject.h"

class InputModeResponseModel
{
public:
    InputModeResponseModel(bool position, bool speed, bool inersia, bool waterspeed,
                   bool wind, bool weather);
    InputModeResponseModel(){};

    static InputModeResponseModel fromJsonObject(QJsonObject obj);

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

#endif // InputModeResponseModel_H
