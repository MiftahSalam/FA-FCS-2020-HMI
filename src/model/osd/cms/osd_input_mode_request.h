#ifndef OSDINPUTMODEREQUEST_H
#define OSDINPUTMODEREQUEST_H


#include "src/model/base_request.h"

class OSDInputModeRequest: public BaseModel<OSDInputModeRequest>
{
public:
    OSDInputModeRequest();
    OSDInputModeRequest(bool position, bool speed, bool inersia, bool waterspeed);

    bool getPosition() const;
    void setPosition(bool newPosition);
    bool getSpeed() const;
    void setSpeed(bool newSpeed);
    bool getInersia() const;
    void setInersia(bool newInersia);
    bool getWaterSpeed() const;
    void setWaterSpeed(bool newWaterSpeed);


private:
    bool position;
    bool speed;
    bool inersia;
    bool waterspeed;

    // BaseModel interface
public:
    QByteArray toJSON() override;
};

#endif // OSDINPUTMODEREQUEST_H
