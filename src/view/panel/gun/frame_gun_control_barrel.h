#ifndef FRAME_GUN_CONTROL_BARREL_H
#define FRAME_GUN_CONTROL_BARREL_H

#include <QFrame>

namespace Ui {
class FrameGunControlBarrel;
}

class FrameGunControlBarrel : public QFrame
{
    Q_OBJECT

public:
    explicit FrameGunControlBarrel(QWidget *parent = nullptr);
    ~FrameGunControlBarrel();

private:
    Ui::FrameGunControlBarrel *ui;
};

#endif // FRAME_GUN_CONTROL_BARREL_H
