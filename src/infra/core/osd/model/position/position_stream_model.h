#ifndef POSITION_STREAM_MODEL_H
#define POSITION_STREAM_MODEL_H

#include "qjsonobject.h"

#include "src/infra/core/osd/model/osd_base_model.h"

class PositionStreamModel: public OSDBaseModel
{
public:
    PositionStreamModel(OSDBaseModel base_data, double lat, double lon);
    PositionStreamModel();

    static PositionStreamModel fromJsonObject(QJsonObject obj);

    double getLatitude() const;
    double getLongitude() const;

private:
    double latitude, longitude;
};

#endif // POSITION_STREAM_MODEL_H
