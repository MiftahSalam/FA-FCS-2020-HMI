#ifndef OSDSETSPEEDREQUEST_H
#define OSDSETSPEEDREQUEST_H

#include "src/model/base_request.h"

class OSDSetSpeedRequest: public BaseModel<OSDSetSpeedRequest>
{
public:
    OSDSetSpeedRequest();
    OSDSetSpeedRequest(double sog, double cog);

    double getSpeed() const;
    double getCourse() const;
    void setSpeed(const double sog);
    void setCourse(const double cog);

private:
    double speed, course;

    // BaseModel interface
public:
    QByteArray toJSON() override;
};

#endif // OSDSETSPEEDREQUEST_H
