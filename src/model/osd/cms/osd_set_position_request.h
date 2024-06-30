#ifndef OSD_SET_POSITION_REQUEST_H
#define OSD_SET_POSITION_REQUEST_H

#include "src/model/base_request.h"

class OSDSetPositionRequest: public BaseModel<OSDSetPositionRequest>
{
public:
    OSDSetPositionRequest();
    OSDSetPositionRequest(double lat, double lon);

    double getLatitude() const;
    double getLongitude() const;
    void setLatitude(const double lat);
    void setLongitude(const double lon);

private:
    double latitude, longitude;

    // BaseModel interface
public:
    QByteArray toJSON() override;
};

#endif // OSD_SET_POSITION_REQUEST_H
