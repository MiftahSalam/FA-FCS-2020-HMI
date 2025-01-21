#ifndef OSD_SET_POSITION_REQUEST_H
#define OSD_SET_POSITION_REQUEST_H

#include "src/model/base_request.h"

class OSDSetPositionRequest: public BaseModel<OSDSetPositionRequest>
{
public:
    OSDSetPositionRequest();
    OSDSetPositionRequest(double lat, double longi);

    double getLatitude() const;
    double getLongitude() const;
    void setLatitude(const double lat);
    void setLongitude(const double lat);

private:
    double latitude, longitude;

    // BaseModel interface
public:
    QByteArray toJSON() override;
};

#endif // OSD_SET_POSITION_REQUEST_H
