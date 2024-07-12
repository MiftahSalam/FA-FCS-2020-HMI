#include "engagement_correction_set_request.h"

EngagementCorrectionSetRequest::EngagementCorrectionSetRequest()
{

}


QByteArray EngagementCorrectionSetRequest::toJSON()
{
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
