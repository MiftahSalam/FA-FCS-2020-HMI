#ifndef FRAME_OSD_BASE_H
#define FRAME_OSD_BASE_H

#include "qtimer.h"
#include "src/model/osd/input_mode_model.h"

class FrameOSDBase
{
public:
    virtual void setup() = 0;
    virtual void resetModeIndex() = 0;

public slots:
    virtual void onModeChangeResponse(InputModeModel mode) = 0;

protected:
    QTimer *timer;

private slots:
    virtual void onModeChange(int index) = 0;
    virtual void onAfterModeReset() = 0;
    virtual void onTimeout() = 0;

private:
    virtual void manualUiSetup() = 0;
    virtual void autoUiSetup() = 0;
};

#endif // FRAME_OSD_BASE_H
