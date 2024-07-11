#ifndef OSDSETWATERSPEEDREQUEST_H
#define OSDSETWATERSPEEDREQUEST_H

#include "src/model/base_request.h"

class OSDSetWaterSpeedRequest: public BaseModel<OSDSetWaterSpeedRequest>
{
public:
    OSDSetWaterSpeedRequest();
    OSDSetWaterSpeedRequest(double w_speed, double w_course);

    double getSpeed() const;
    double getCourse() const;
    void setSpeed(const double w_speed);
    void setCourse(const double w_course);

private:
    double speed, course;

    // BaseModel interface
public:
    QByteArray toJSON() override;
};
#endif // OSDSETWATERSPEEDREQUEST_H
