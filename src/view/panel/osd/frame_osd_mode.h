#ifndef FRAME_OSD_MODE_H
#define FRAME_OSD_MODE_H

#include <QWidget>

namespace Ui {
class FrameOSDMode;
}

class FrameOSDMode : public QWidget
{
    Q_OBJECT

public:
    explicit FrameOSDMode(QWidget *parent = nullptr);
    ~FrameOSDMode();

private:
    Ui::FrameOSDMode *ui;
};

#endif // FRAME_OSD_MODE_H
