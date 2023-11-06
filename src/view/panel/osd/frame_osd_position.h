#ifndef FRAME_OSD_POSITION_H
#define FRAME_OSD_POSITION_H

#include <QWidget>

#include "src/usecase/osd/cms/osd_cms_input_mode.h"
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

class FrameOSDPosition : public QWidget
{
    Q_OBJECT

public:
    explicit FrameOSDPosition(QWidget *parent = nullptr);
    ~FrameOSDPosition();

    void setup(const OSDPositionProp& prop);
    void resetModeIndex();

public slots:
    void onModeChangeResponse(InputModeModel mode);

signals:
    void signalChangePositionMode(bool manual_mode);

private slots:
    void onModeChange(int index);
    void onAfterModeReset();

private:
    Ui::FrameOSDPosition *ui;
    OSDCMSPositionData* _cmsPos;

    OSD_MODE currentMode;
    int currentModeIndx;
    bool afterResetModeIndx;

    void manualUiSetup();
    void autoUiSetup();
};

#endif // FRAME_OSD_POSITION_H
