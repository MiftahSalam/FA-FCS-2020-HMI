#ifndef OSD_SERVICE_H
#define OSD_SERVICE_H

#include <QObject>

#include "src/infra/core/base_response.h"
#include "src/infra/core/osd/cms/osd_cms.h"
#include "src/infra/core/osd/model/inertia/gyro_model.h"
#include "src/infra/core/osd/model/input_mode/input_mode_model.h"
#include "src/infra/store/osd/osd_repository.h"

class OSDService : public QObject
{
    Q_OBJECT
public:
    OSDService(OSDService &other) = delete;
    void operator=(const OSDService&) = delete;
    ~OSDService() override;

    static OSDService* getInstance(
        QObject *parent,
        OSDRepository* osdRepo,
        OSDCMS* osdCms
        );

    void setDataMode(const QString &dataFisis, const OSD_MODE mode);
    const OSDInputModeEntity* getDataMode() const;

    void setManualDataInertia(OSDSetGyroRequest req);
    void setManualDataSpeed(OSDSetSpeedRequest req);
    void setManualDataPosition(OSDSetPositionRequest req);
    void setManualDataWaterSpeed(OSDSetWaterSpeedRequest req);
    void setManualDataWeather(OSDSetWeatherRequest req);
    void setManualDataWind(OSDSetWindRequest req);

    float getOSDHeading() const;

    BaseError getOSDAutoStatusInertia() const;
    BaseError getOSDAutoStatusSpeed() const;
    BaseError getOSDAutoStatusWind() const;
    BaseError getOSDAutoStatusPosition() const;
    BaseError getOSDAutoStatusWaterSpeed() const;
    BaseError getOSDAutoStatusWeather() const;

signals:
    void signal_processedSetModeResponse(const QString datafisis, BaseResponse<InputModeModel> response, bool needConfirm);

public slots:
    void onUpdateManualDataGyro(BaseResponse<GyroModel> resp);
    void onUpdateManualDataPosition(BaseResponse<PositionModel> resp);
    void onUpdateManualDataSpeed(BaseResponse<SpeedModel> resp);
    void onUpdateManualDataWaterSpeed(BaseResponse<WaterSpeedModel> resp);
    void onUpdateManualDataWeather(BaseResponse<WeatherModel> resp);
    void onUpdateManualDataWind(BaseResponse<WindModel> resp);
    void onUpdateInputMode(const QString datafisis, BaseResponse<InputModeModel> resp, bool needConfirm);

    void onUpdateAutoDataPosition(PositionModel data);

protected:
    OSDService(
        QObject *parent = nullptr,
        OSDRepository* osdRepo = nullptr,
        OSDCMS* osdCms = nullptr
        );

private slots:
    void onTimerTimeout();
    void sync();

private:
    static OSDService *instance;
    OSDInputModeRepository *repoMode;
    OSDInputModeEntity *previousMode;
    OSDRepository *repoOsd;
    OSDCMS *cmsOsd;
};

#endif // OSD_SERVICE_H
