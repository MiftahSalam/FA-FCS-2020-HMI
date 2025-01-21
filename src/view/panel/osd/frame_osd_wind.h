#ifndef FRAME_OSD_WIND_H
#define FRAME_OSD_WIND_H

#include <QWidget>

#include "src/infra/core/cms/osd/input_mode/osd_cms_input_mode.h"
#include "src/infra/core/cms/osd/wind/osd_cms_wind_data.h"
#include "src/usecase/osd/stream/osd_stream_wind.h"
#include "src/view/panel/osd/frame_osd_base.h"
#include "src/view/shared/frame_text_input.h"

namespace Ui {
class FrameOSDWind;
}

struct OSDWindProp
{
    QString title;
    TextInputProp speed;
    TextInputProp direction;
};

class FrameOSDWind : public QWidget, public FrameOSDBase<WindModel, WindModel>
{
    Q_OBJECT

public:
    explicit FrameOSDWind(QWidget *parent = nullptr);
    ~FrameOSDWind();

    void setup() override;
    void resetModeIndex() override;

public slots:
    void onModeChangeResponse(const QString datafisis, BaseResponse<InputModeModel> resp, bool needConfirm) override;
    void onDataResponse(BaseResponse<WindModel> data) override;
    void onStreamReceive(WindModel model) override;
    //added by riyadhi
    void onUpdateWindAutoUi();

signals:
    void signalChangeWindMode(bool manual_mode);
    void signalChangeWindData(float _speed, float _direction);

private slots:
    void onModeChange(int index) override;
    void onAfterModeReset() override;
    void onTimeout() override;

    void on_pushButton_clicked();

private:
    Ui::FrameOSDWind *ui;
    OSDCMSWindData *_cmsWind;
    OSDCMSInputMode *_cmsMode;
    OSDStreamWind* _streamWind;

    // FrameOSDBase interface
    void manualUiSetup() override;
    void autoUiSetup() override;

    void notConnectedUiSetup() override;
    void noDataUiSetup() override;
    void invalidDataUiSetup() override;

    bool validateInput() override;

    void setErrorInput(BaseError error);
};

#endif // FRAME_OSD_WIND_H
