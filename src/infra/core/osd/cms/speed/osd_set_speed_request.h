#ifndef OSDSETSPEEDREQUEST_H
#define OSDSETSPEEDREQUEST_H

#include "src/infra/core/base_request.h"

class OSDSetSpeedRequest: public BaseModel
{
public:
    OSDSetSpeedRequest();

    OSDSetSpeedRequest(double _speed, double _course);

    double getSpeed() const;
    double getCourse() const;

    void setSpeed(const double _sog);
    void setCourse(const double _cog);

    // BaseModel interface
    QByteArray toJSON() override;

private:
    double speed, course;
};

#endif // OSDSETSPEEDREQUEST_H
