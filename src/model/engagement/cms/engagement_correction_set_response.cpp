#include "engagement_correction_set_response.h"

EngagementCorrectionSetResponse::EngagementCorrectionSetResponse() {}

EngagementCorrectionSetResponse::EngagementCorrectionSetResponse(float azimuthCorrection, float elevationCorrection)
{

}

EngagementCorrectionSetResponse EngagementCorrectionSetResponse::FromJsonObject(QJsonObject obj)
{
    EngagementCorrectionSetResponse model(
        obj.value("azimuth").toDouble(),
        obj.value("elevation").toDouble()
        );

    return model;
}

float EngagementCorrectionSetResponse::getAzimuthCorrection() const
{
    return azimuthCorrection;
}

float EngagementCorrectionSetResponse::getElevationCorrection() const
{
    return elevationCorrection;
}
