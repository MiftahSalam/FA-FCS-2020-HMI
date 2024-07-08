#ifndef OSDSETSPEEDREQUEST_H
#define OSDSETSPEEDREQUEST_H

#include "src/model/base_request.h"

class OSDSetSpeedRequest: public BaseModel<OSDSetSpeedRequest>
{
public:
    OSDSetSpeedRequest();

    OSDSetSpeedRequest(double _speed, double _course);

    double getSpeed() const;
    double getCourse() const;

    void setSpeed(const double _sog);
    void setCourse(const double _cog);

private:
    double speed, course;

    public:
        QByteArray toJSON() override;
};

#endif // OSDSETSPEEDREQUEST_H
