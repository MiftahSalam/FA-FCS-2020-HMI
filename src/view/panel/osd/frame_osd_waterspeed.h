#ifndef FRAME_OSD_WATERSPEED_H
#define FRAME_OSD_WATERSPEED_H

#include <QWidget>

#include "src/infra/core/cms/osd/osd_cms_input_mode.h"
#include "src/infra/core/cms/osd/osd_cms_waterspeed_data.h"
#include "src/usecase/osd/stream/osd_stream_waterspeed.h"
#include "src/view/panel/osd/frame_osd_base.h"
#include "src/view/shared/frame_text_input.h"

namespace Ui {
class FrameOSDWaterSpeed;
}

struct OSDWaterSpeedProp
{
    QString title;
    TextInputProp waterspeed;
    TextInputProp watercourse;
};

class FrameOSDWaterSpeed : public QWidget, public FrameOSDBase<WaterSpeedModel, WaterSpeedModel>
{
    Q_OBJECT

public:
    explicit FrameOSDWaterSpeed(QWidget *parent = nullptr);
    ~FrameOSDWaterSpeed();

    void setup() override;
    void resetModeIndex() override;

public slots:
    void onModeChangeResponse(const QString datafisis, BaseResponse<InputModeModel> resp, bool needConfirm) override;
    void onDataResponse(BaseResponse<WaterSpeedModel> data) override;
    void onStreamReceive(WaterSpeedModel model) override;
    //added by riyadhi
    void onUpdateWaterSpeedAutoUi();

signals:
    void signalChangeWaterSpeedMode(bool manual_mode);
    void signalChangeWaterSpeedData(float w_speed, float w_course);

private slots:
    void onModeChange(int index) override;
    void onAfterModeReset() override;
    void onTimeout() override;

    void on_pushButton_clicked();

private:
    Ui::FrameOSDWaterSpeed *ui;
    OSDCMSWaterSpeedData *_cmsWS;
    OSDCMSInputMode *_cmsMode;
    OSDStreamWaterSpeed* _streamWS;

    // FrameOSDBase interface
    void manualUiSetup() override;
    void autoUiSetup() override;

    void notConnectedUiSetup() override;
    void noDataUiSetup() override;
    void invalidDataUiSetup() override;

    bool validateInput() override;

    void setErrorInput(BaseError error);
};

#endif // FRAME_OSD_WATERSPEED_H
