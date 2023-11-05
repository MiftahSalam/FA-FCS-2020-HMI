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

    void setCurrentModeIndex(int index);

    ~FrameOSDMode();

signals:
    void signal_currentModeChange(int index);

private:
    Ui::FrameOSDMode *ui;
};

#endif // FRAME_OSD_MODE_H
