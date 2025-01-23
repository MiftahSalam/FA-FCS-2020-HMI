#ifndef OSDINPUTMODEREQUEST_H
#define OSDINPUTMODEREQUEST_H

#include "src/infra/core/base_request.h"

class OSDInputModeRequest: public BaseModel
{
public:
    OSDInputModeRequest();
    OSDInputModeRequest(bool position, bool speed, bool inersia, bool waterspeed, bool wind, bool weather);

    bool getPosition() const;
    void setPosition(bool newPosition);
    bool getSpeed() const;
    void setSpeed(bool newSpeed);
    bool getInersia() const;
    void setInersia(bool newInersia);
    bool getWaterSpeed() const;
    void setWaterSpeed(bool newWaterSpeed);
    bool getWind() const;
    void setWind(bool newWind);
    bool getWeather() const;
    void setWeather(bool newWeather);

    // BaseModel interface
    QByteArray toJSON() override;

private:
    bool position;
    bool speed;
    bool inersia;
    bool waterspeed;
    bool wind;
    bool weather;
};

#endif // OSDINPUTMODEREQUEST_H
