#include "engagement_correction_set_request.h"
#include <QJsonObject>
#include <QJsonDocument>

EngagementCorrectionSetRequest::EngagementCorrectionSetRequest()
{

}


QByteArray EngagementCorrectionSetRequest::toJSON()
{
    QJsonObject obj;
    obj["azimuth_correction"] = azimuthCorrection;
    obj["elevation_correction"] = elevationCorrection;

    return QJsonDocument(obj).toJson();
}

float EngagementCorrectionSetRequest::getAzimuthCorrection() const
{
    return azimuthCorrection;
}

void EngagementCorrectionSetRequest::setAzimuthCorrection(float newAzimuthCorrection)
{
    azimuthCorrection = newAzimuthCorrection;
}

float EngagementCorrectionSetRequest::getElevationCorrection() const
{
    return elevationCorrection;
}

void EngagementCorrectionSetRequest::setElevationCorrection(float newElevationCorrection)
{
    elevationCorrection = newElevationCorrection;
}

EngagementCorrectionSetRequest::EngagementCorrectionSetRequest(float azimuthCorrection, float elevationCorrection) : azimuthCorrection(azimuthCorrection),
    elevationCorrection(elevationCorrection)
{}
