#ifndef OSD_SET_POSITION_REQUEST_H
#define OSD_SET_POSITION_REQUEST_H

#include "src/infra/core/base_request.h"

class OSDSetPositionRequest: public BaseModel
{
public:
    OSDSetPositionRequest();
    OSDSetPositionRequest(double lat, double longi);

    double getLatitude() const;
    double getLongitude() const;
    void setLatitude(const double lat);
    void setLongitude(const double lat);

    // BaseModel interface
    QByteArray toJSON() override;

private:
    double latitude, longitude;
};

#endif // OSD_SET_POSITION_REQUEST_H
