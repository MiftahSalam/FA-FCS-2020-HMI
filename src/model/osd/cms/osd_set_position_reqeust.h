#ifndef OSD_SET_POSITION_REQEUST_H
#define OSD_SET_POSITION_REQEUST_H


class OSDSetPositionReqeust
{
public:
    OSDSetPositionReqeust(double lat, double longi);

    double getLatitude() const;
    double getLongitude() const;
    void setLatitude(const double lat);
    void setLongitude(const double lat);

private:
    double latitude, longitude;
};

#endif // OSD_SET_POSITION_REQEUST_H
