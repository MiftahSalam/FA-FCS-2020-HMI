#ifndef FRAME_OSD_SPEED_H
#define FRAME_OSD_SPEED_H

#include "src/usecase/osd/cms/osd_cms_input_mode.h"
#include "src/usecase/osd/stream/osd_stream_gyro.h"
#include "src/view/panel/osd/frame_osd_base.h"
#include "src/view/shared/frame_text_input.h"
#include <QWidget>

namespace Ui {
class FrameOSDSpeed;
}

struct OSDSpeedProp
{
    QString title;
    TextInputProp speed;
    TextInputProp course;
};

class FrameOSDSpeed : public QWidget
{
    Q_OBJECT

public:
    explicit FrameOSDSpeed(QWidget *parent = nullptr);
    ~FrameOSDSpeed();

private:
    Ui::FrameOSDSpeed *ui;
};

#endif // FRAME_OSD_SPEED_H
