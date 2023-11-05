#ifndef FRAME_OSD_GYRO_H
#define FRAME_OSD_GYRO_H

#include "src/view/shared/frame_text_input.h"
#include <QFrame>

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

class FrameOSDGyro : public QFrame
{
    Q_OBJECT

public:
    explicit FrameOSDGyro(QWidget *parent = nullptr);
    ~FrameOSDGyro();

    void modify(const OSDGyroProp& prop);

private:
    Ui::FrameOSDGyro *ui;
};

#endif // FRAME_OSD_GYRO_H
