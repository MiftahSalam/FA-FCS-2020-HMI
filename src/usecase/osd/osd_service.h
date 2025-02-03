#ifndef OSD_SERVICE_H
#define OSD_SERVICE_H

#include <QObject>

#include "src/infra/core/base_response.h"
#include "src/infra/core/osd/cms/osd_cms.h"
#include "src/infra/core/osd/model/date_time/date_time_stream_model.h"
#include "src/infra/core/osd/model/inertia/gyro_cms_response_model.h"
#include "src/infra/core/osd/model/input_mode/input_mode_cms_response_model.h"
#include "src/infra/core/osd/stream/osd_stream.h"
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
        OSDCMS* osdCms,
        OSDStream* osdStream
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
    void signal_processedSetModeResponse(const QString datafisis, BaseResponse<InputModeResponseModel> response, bool needConfirm);
    void signal_processedSetResponseWind(WindResponseModel data);
    void signal_processedSetResponseGyro(GyroResponseModel data);
    void signal_processedSetResponseSpeed(SpeedResponseModel data);
    void signal_processedSetResponsePosition(PositionResponseModel data);
    void signal_processedSetResponseWeather(WeatherResponseModel data);
    void signal_processedSetResponseWaterSpeed(WaterSpeedResponseModel data);

    void signal_processedAutoDataWind(WindStreamModel data);
    void signal_processedAutoDataGyro(GyroStreamModel data);
    void signal_processedAutoDataSpeed(SpeedStreamModel data);
    void signal_processedAutoDataPosition(PositionStreamModel data);
    void signal_processedAutoDataWeather(WeatherStreamModel data);
    void signal_processedAutoDataWaterSpeed(WaterSpeedStreamModel data);
    void signal_processedAutoDataDateTime(DateTimeStreamModel data);

public slots:
    void onUpdateManualDataGyro(BaseResponse<GyroResponseModel> resp);
    void onUpdateManualDataPosition(BaseResponse<PositionResponseModel> resp);
    void onUpdateManualDataSpeed(BaseResponse<SpeedResponseModel> resp);
    void onUpdateManualDataWaterSpeed(BaseResponse<WaterSpeedResponseModel> resp);
    void onUpdateManualDataWeather(BaseResponse<WeatherResponseModel> resp);
    void onUpdateManualDataWind(BaseResponse<WindResponseModel> resp);
    void onUpdateInputMode(const QString datafisis, BaseResponse<InputModeResponseModel> resp, bool needConfirm);

    void onUpdateAutoDataInertia(GyroStreamModel data);
    void onUpdateAutoDataPosition(PositionStreamModel data);
    void onUpdateAutoDataSpeed(SpeedStreamModel data);
    void onUpdateAutoDataWaterSpeed(WaterSpeedStreamModel data);
    void onUpdateAutoDataWind(WindStreamModel data);
    void onUpdateAutoDataWeather(WeatherStreamModel data);
    void onUpdateAutoDataDateTime(DateTimeStreamModel data);

protected:
    OSDService(
        QObject *parent = nullptr,
        OSDRepository* osdRepo = nullptr,
        OSDCMS* osdCms = nullptr,
        OSDStream* osdStream = nullptr
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
    OSDStream* streamOsd;

    QTimer *timer;

    bool synced;

    void resetToPrevMode();
};

#endif // OSD_SERVICE_H
