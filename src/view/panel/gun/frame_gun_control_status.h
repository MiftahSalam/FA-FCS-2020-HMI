#ifndef FRAME_GUN_CONTROL_STATUS_H
#define FRAME_GUN_CONTROL_STATUS_H

#include <QFrame>

namespace Ui {
class FrameGunControlStatus;
}

class FrameGunControlStatus : public QFrame
{
    Q_OBJECT

public:
    explicit FrameGunControlStatus(QWidget *parent = nullptr);
    ~FrameGunControlStatus();

private:
    Ui::FrameGunControlStatus *ui;
};

#endif // FRAME_GUN_CONTROL_STATUS_H
