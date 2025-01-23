#ifndef OSDSETWINDREQUEST_H
#define OSDSETWINDREQUEST_H

#include "src/infra/core/base_request.h"

class OSDSetWindRequest: public BaseModel
{
public:
    OSDSetWindRequest();
    OSDSetWindRequest(double _speed, double _direction);

    double getSpeed() const;
    double getDirection() const;
    void setSpeed(const double _speed);
    void setDirection(const double _direction);

    // BaseModel interface
    QByteArray toJSON() override;

private:
    double speed, direction;    
};

#endif // OSDSETWINDREQUEST_H
