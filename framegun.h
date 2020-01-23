#ifndef FRAMEGUN_H
#define FRAMEGUN_H

#include <QFrame>

namespace Ui {
class FrameGun;
}

class FrameGun : public QFrame
{
    Q_OBJECT

public:
    explicit FrameGun(QWidget *parent = 0);
    ~FrameGun();

private:
    Ui::FrameGun *ui;
};

#endif // FRAMEGUN_H
