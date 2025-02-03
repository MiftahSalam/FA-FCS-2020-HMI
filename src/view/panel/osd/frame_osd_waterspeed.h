#ifndef FRAME_OSD_WATERSPEED_H
#define FRAME_OSD_WATERSPEED_H

#include <QWidget>

#include "src/usecase/osd/osd_service.h"
#include "src/infra/core/osd/model/water_speed/water_speed_stream_model.h"
#include "src/infra/core/osd/cms/water_speed/waterspeed_model.h"
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

class FrameOSDWaterSpeed : public QWidget, public FrameOSDBase<WaterSpeedStreamModel, WaterSpeedModel>
{
    Q_OBJECT

public:
    explicit FrameOSDWaterSpeed(QWidget *parent = nullptr);
    ~FrameOSDWaterSpeed();

    void setup() override;
    void resetModeIndex() override;

public slots:
    void onModeChangeResponse(const QString datafisis, BaseResponse<InputModeModel> resp, bool needConfirm) override;
    void onDataResponse(WaterSpeedModel data) override;
    void onStreamReceive(WaterSpeedStreamModel model) override;
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

    OSDService *_serviceOSD;

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
