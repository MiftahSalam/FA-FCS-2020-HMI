#ifndef OSDSETWATERSPEEDREQUEST_H
#define OSDSETWATERSPEEDREQUEST_H

#include "src/model/base_request.h"

class OSDSetWaterSpeedRequest: public BaseModel<OSDSetWaterSpeedRequest>
{
public:
    OSDSetWaterSpeedRequest();
    OSDSetWaterSpeedRequest(double w_speed, double w_course);

    double getWaterSpeed() const;
    double getWaterCourse() const;
    void setWaterSpeed(const double w_speed);
    void setWaterCourse(const double w_course);

private:
    double waterspeed, watercourse;

    // BaseModel interface
public:
    QByteArray toJSON() override;
};
#endif // OSDSETWATERSPEEDREQUEST_H
