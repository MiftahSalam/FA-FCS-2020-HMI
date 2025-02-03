#ifndef OSD_SERVICE_H
#define OSD_SERVICE_H

#include <QObject>

#include "src/infra/core/base_response.h"
#include "src/infra/core/osd/cms/osd_cms.h"
#include "src/infra/core/osd/model/date_time/date_time_stream_model.h"
#include "src/infra/core/osd/model/inertia/gyro_model.h"
#include "src/infra/core/osd/model/input_mode/input_mode_model.h"
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
    void signal_processedSetModeResponse(const QString datafisis, BaseResponse<InputModeModel> response, bool needConfirm);
    void signal_processedSetResponseWind(WindModel data);
    void signal_processedSetResponseGyro(GyroModel data);
    void signal_processedSetResponseSpeed(SpeedModel data);
    void signal_processedSetResponsePosition(PositionModel data);
    void signal_processedSetResponseWeather(WeatherModel data);
    void signal_processedSetResponseWaterSpeed(WaterSpeedModel data);

    void signal_processedAutoDataWind(WindStreamModel data);
    void signal_processedAutoDataGyro(GyroStreamModel data);
    void signal_processedAutoDataSpeed(SpeedStreamModel data);
    void signal_processedAutoDataPosition(PositionStreamModel data);
    void signal_processedAutoDataWeather(WeatherStreamModel data);
    void signal_processedAutoDataWaterSpeed(WaterSpeedStreamModel data);
    void signal_processedAutoDataDateTime(DateTimeStreamModel data);

public slots:
    void onUpdateManualDataGyro(BaseResponse<GyroModel> resp);
    void onUpdateManualDataPosition(BaseResponse<PositionModel> resp);
    void onUpdateManualDataSpeed(BaseResponse<SpeedModel> resp);
    void onUpdateManualDataWaterSpeed(BaseResponse<WaterSpeedModel> resp);
    void onUpdateManualDataWeather(BaseResponse<WeatherModel> resp);
    void onUpdateManualDataWind(BaseResponse<WindModel> resp);
    void onUpdateInputMode(const QString datafisis, BaseResponse<InputModeModel> resp, bool needConfirm);

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
