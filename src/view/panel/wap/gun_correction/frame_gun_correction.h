#ifndef FRAME_GUN_CORRECTION_H
#define FRAME_GUN_CORRECTION_H

#include <QWidget>

namespace Ui {
class FrameGunCorrection;
}

class FrameGunCorrection : public QWidget
{
    Q_OBJECT

public:
    explicit FrameGunCorrection(QWidget *parent = nullptr);
    ~FrameGunCorrection();

private:
    Ui::FrameGunCorrection *ui;
};

#endif // FRAME_GUN_CORRECTION_H
