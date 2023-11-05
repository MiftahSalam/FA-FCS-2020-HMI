#ifndef FRAME_WEAPON_ASSIGN_H
#define FRAME_WEAPON_ASSIGN_H

#include <QWidget>

namespace Ui {
class FrameWeaponAssign;
}

class FrameWeaponAssign : public QWidget
{
    Q_OBJECT

public:
    explicit FrameWeaponAssign(QWidget *parent = nullptr);
    ~FrameWeaponAssign();

private:
    Ui::FrameWeaponAssign *ui;
};

#endif // FRAME_WEAPON_ASSIGN_H
