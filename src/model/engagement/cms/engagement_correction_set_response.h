#ifndef ENGAGEMENT_CORRECTION_SET_RESPONSE_H
#define ENGAGEMENT_CORRECTION_SET_RESPONSE_H

#include <QJsonObject>

class EngagementCorrectionSetResponse
{
public:
    EngagementCorrectionSetResponse();
    EngagementCorrectionSetResponse(float azimuthCorrection, float elevationCorrection);

    static EngagementCorrectionSetResponse FromJsonObject(QJsonObject obj);

    float getAzimuthCorrection() const;
    float getElevationCorrection() const;

private:
    float azimuthCorrection;
    float elevationCorrection;
};

#endif // ENGAGEMENT_CORRECTION_SET_RESPONSE_H
