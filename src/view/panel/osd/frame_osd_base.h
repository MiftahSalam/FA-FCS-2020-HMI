#ifndef FRAME_OSD_BASE_H
#define FRAME_OSD_BASE_H

#include "qtimer.h"
#include "src/model/osd/input_mode_model.h"
#include "src/view/shared/constant_ui.h"

template <typename Model> class FrameOSDBase
{
public:
    virtual void setup() = 0;
    virtual void resetModeIndex() = 0;

public slots:
    virtual void onModeChangeResponse(InputModeModel mode) = 0;
    virtual void onStreamReceive(Model model) = 0;

protected:
    QTimer *timer;
    QDateTime timestamp;
    OSD_MODE currentMode;
    int currentModeIndx;
    bool afterResetModeIndx;

private slots:
    virtual void onModeChange(int index) = 0;
    virtual void onAfterModeReset() = 0;
    virtual void onTimeout() = 0;

private:
    virtual void manualUiSetup() = 0;
    virtual void autoUiSetup() = 0;
};

#endif // FRAME_OSD_BASE_H
