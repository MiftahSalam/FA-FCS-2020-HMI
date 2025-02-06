#ifndef OSDSETGYROREQUEST_H
#define OSDSETGYROREQUEST_H

#include "src/infra/core/base_request.h"

class OSDSetGyroRequest: public BaseModel
{
public:
    OSDSetGyroRequest();

    OSDSetGyroRequest(double _heading, double _pitch, double _roll);

    double getHeading() const;
    double getPitch() const;
    double getRoll() const;

    void setHeading(const double _hdg);
    void setPitch(const double _pitch);
    void setRoll(const double _roll);

    QByteArray toJSON() override;

private:
    double heading, pitch, roll;
};

#endif // OSDSETGYROREQUEST_H
