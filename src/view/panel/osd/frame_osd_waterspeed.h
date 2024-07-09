#ifndef FRAME_OSD_WATERSPEED_H
#define FRAME_OSD_WATERSPEED_H

#include <QWidget>

namespace Ui {
class FrameOSDWaterSpeed;
}

class FrameOSDWaterSpeed : public QWidget
{
    Q_OBJECT

public:
    explicit FrameOSDWaterSpeed(QWidget *parent = nullptr);
    ~FrameOSDWaterSpeed();

private:
    Ui::FrameOSDWaterSpeed *ui;
};

#endif // FRAME_OSD_WATERSPEED_H
