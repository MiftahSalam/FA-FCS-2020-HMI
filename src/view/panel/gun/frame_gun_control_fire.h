#ifndef FRAME_GUN_CONTROL_FIRE_H
#define FRAME_GUN_CONTROL_FIRE_H

#include <QFrame>

namespace Ui {
class FrameGunControlFire;
}

class FrameGunControlFire : public QFrame
{
    Q_OBJECT

public:
    explicit FrameGunControlFire(QWidget *parent = nullptr);
    ~FrameGunControlFire();

private:
    Ui::FrameGunControlFire *ui;
};

#endif // FRAME_GUN_CONTROL_FIRE_H
