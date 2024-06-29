#ifndef FRAME_OSD_SPEED_H
#define FRAME_OSD_SPEED_H

#include <QWidget>

namespace Ui {
class FrameOSDSpeed;
}

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
