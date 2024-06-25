#ifndef FRAME_OSD_GYRO_H
#define FRAME_OSD_GYRO_H

#include "src/usecase/osd/stream/osd_stream_position.h"
#include "src/view/panel/osd/frame_osd_base.h"
#include "src/view/shared/frame_text_input.h"
#include <QFrame>
#include <QWidget>

namespace Ui {
class FrameOSDGyro;
}

struct OSDGyroProp
{
    QString title;
    TextInputProp heading;
    TextInputProp pitch;
    TextInputProp roll;
};

class FrameOSDGyro : public QWidget
{
    Q_OBJECT

public:
    explicit FrameOSDGyro(QWidget *parent = nullptr);
    ~FrameOSDGyro();



    void setup(const OSDGyroProp& prop);

private slots:
    void onModeChange(int index);

private:
    Ui::FrameOSDGyro *ui;
};

#endif // FRAME_OSD_GYRO_H
