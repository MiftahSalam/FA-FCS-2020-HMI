#ifndef FRAME_OSD_SPEED_H
#define FRAME_OSD_SPEED_H

#include <QWidget>

#include "src/usecase/osd/stream/osd_stream_speed.h"
#include "src/view/panel/osd/frame_osd_base.h"
#include "src/view/shared/frame_text_input.h"

namespace Ui {
class FrameOSDSpeed;
}

struct OSDSpeedProp
{
    QString title;
    TextInputProp sog;
    TextInputProp cog;
};

class FrameOSDSpeed : public QWidget, public FrameOSDBase<SpeedModel, SpeedModel>
{
    Q_OBJECT

public:
    explicit FrameOSDSpeed(QWidget *parent = nullptr);
    ~FrameOSDSpeed();

    void setup() override;
    void resetModeIndex() override;

public slots:
    void onModeChangeResponse(InputModeModel mode) override;
    void onDataResponse(SpeedModel data) override;
    void onStreamReceive(SpeedModel model) override;
    //added by riyadhi
    void onUpdateAutoUi();

signals:
    void signalChangeSpeedMode(bool manual_mode);
    void signalChangeSpeedData(float sog, float cog);

private slots:
    void onModeChange(int index) override;
    void onAfterModeReset() override;
    void onTimeout() override;


private:
    Ui::FrameOSDSpeed *ui;
    OSDCMSSpeedData*_cmsSpeed;
    OSDStreamSpeed* _streamSpeed;

    // FrameOSDBase interface
    void manualUiSetup() override;
    void autoUiSetup() override;

    void notConnectedUiSetup() override;
    void noDataUiSetup() override;
    void invalidDataUiSetup() override;

    bool validateInput() override;
};
#endif // FRAME_OSD_SPEED_H
