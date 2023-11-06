#ifndef FRAME_OSD_POSITION_H
#define FRAME_OSD_POSITION_H

#include <QWidget>

#include "src/view/panel/osd/frame_osd_base.h"
#include "src/view/shared/constant_ui.h"
#include "src/view/shared/frame_text_input.h"

namespace Ui {
class FrameOSDPosition;
}

struct OSDPositionProp
{
    QString title;
    TextInputProp lat;
    TextInputProp lon;
};

class FrameOSDPosition : public QWidget, public FrameOSDBase
{
    Q_OBJECT

public:
    explicit FrameOSDPosition(QWidget *parent = nullptr);
    ~FrameOSDPosition();

    void setup() override;
    void resetModeIndex() override;

public slots:
    void onModeChangeResponse(InputModeModel mode) override;

signals:
    void signalChangePositionMode(bool manual_mode);

private slots:
    void onModeChange(int index) override;
    void onAfterModeReset() override;

private:
    Ui::FrameOSDPosition *ui;
    OSDCMSPositionData* _cmsPos;

    OSD_MODE currentMode;
    int currentModeIndx;
    bool afterResetModeIndx;

    void manualUiSetup() override;
    void autoUiSetup() override;
};

#endif // FRAME_OSD_POSITION_H
