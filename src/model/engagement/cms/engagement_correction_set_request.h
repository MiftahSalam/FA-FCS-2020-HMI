#ifndef ENGAGEMENTCORRECTIONSETREQUEST_H
#define ENGAGEMENTCORRECTIONSETREQUEST_H

#include "src/infra/core/base_request.h"

class EngagementCorrectionSetRequest: public BaseModel<EngagementCorrectionSetRequest>
{
public:
    EngagementCorrectionSetRequest();
    EngagementCorrectionSetRequest(float azimuthCorrection, float elevationCorrection);

    // BaseModel interface
    QByteArray toJSON() override;

    float getAzimuthCorrection() const;
    void setAzimuthCorrection(float newAzimuthCorrection);
    float getElevationCorrection() const;
    void setElevationCorrection(float newElevationCorrection);

private:
    float azimuthCorrection, elevationCorrection;
};

#endif // ENGAGEMENTCORRECTIONSETREQUEST_H
