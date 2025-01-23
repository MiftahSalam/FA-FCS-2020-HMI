#ifndef OSDSETWATERSPEEDREQUEST_H
#define OSDSETWATERSPEEDREQUEST_H

#include "src/infra/core/base_request.h"

class OSDSetWaterSpeedRequest: public BaseModel
{
public:
    OSDSetWaterSpeedRequest();
    OSDSetWaterSpeedRequest(double w_speed, double w_course);

    double getSpeed() const;
    double getCourse() const;
    void setSpeed(const double w_speed);
    void setCourse(const double w_course);

    // BaseModel interface
    QByteArray toJSON() override;

private:
    double speed, course;
};
#endif // OSDSETWATERSPEEDREQUEST_H
