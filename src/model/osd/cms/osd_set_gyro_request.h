#ifndef OSDSETGYROREQUEST_H
#define OSDSETGYROREQUEST_H

#include "src/model/base_request.h"



class OSDSetGyroRequest: public BaseModel<OSDSetGyroRequest>
{
public:
    OSDSetGyroRequest();

    OSDSetGyroRequest(double _heading, double _pitch, double _roll);

    double getHeading() const;
    double getPitch() const;
    double getRoll() const;

    void setHeading(const double hdg);
    void setPitch(const double pit);
    void setRoll(const double rol);

private:
    double heading, pitch, roll;

public:
    QByteArray toJSON() override;
};

#endif // OSDSETGYROREQUEST_H
