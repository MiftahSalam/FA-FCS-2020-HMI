#ifndef FRAME_OSD_WEATHER_H
#define FRAME_OSD_WEATHER_H

#include "src/infra/core/cms/osd/osd_cms_input_mode.h"
#include "src/infra/core/cms/osd/osd_cms_weather_data.h"
#include "src/usecase/osd/stream/osd_stream_weather.h"
#include "src/view/panel/osd/frame_osd_base.h"
#include "src/view/shared/frame_text_input.h"

#include <QWidget>

namespace Ui {
class FrameOSDWeather;
}

struct OSDWeatherProp
{
    QString title;
    TextInputProp Temp;
    TextInputProp Press;
    TextInputProp Hum;
};

class FrameOSDWeather : public QWidget, public FrameOSDBase<WeatherModel, WeatherModel>
{
    Q_OBJECT

public:
    explicit FrameOSDWeather(QWidget *parent = nullptr);
    ~FrameOSDWeather();

    void setup() override;
    void resetModeIndex() override;

public slots:
    void onModeChangeResponse(const QString datafisis, BaseResponse<InputModeModel> mode, bool needConfirm) override;
    void onDataResponse(BaseResponse<WeatherModel> data) override;
    void onStreamReceive(WeatherModel model) override;

signals:
    void signalChangeWeatherMode(bool manual_mode);
    void signalChangeWeatherData(float temp, float hum, float press);

private slots:
    void onModeChange(int index) override;
    void onAfterModeReset() override;
    void onTimeout() override;

    void on_pushButton_clicked();

private:
    Ui::FrameOSDWeather *ui;
    OSDCMSWeatherData *_cmsWeather;
    OSDCMSInputMode *_cmsMode;
    OSDStreamWeather* _streamWeather;

    void manualUiSetup() override;
    void autoUiSetup() override;

    void notConnectedUiSetup() override;
    void noDataUiSetup() override;
    void invalidDataUiSetup() override;

    bool validateInput() override;

    void setErrorInput(BaseError error);
};

#endif // FRAME_OSD_WEATHER_H
