#ifndef FRAME_GUN_DATA_MONITORING_H
#define FRAME_GUN_DATA_MONITORING_H

#include <QFrame>

namespace Ui {
class FrameGunDataMonitoring;
}

class FrameGunDataMonitoring : public QFrame
{
    Q_OBJECT

public:
    explicit FrameGunDataMonitoring(QWidget *parent = nullptr);
    ~FrameGunDataMonitoring();

private:
    Ui::FrameGunDataMonitoring *ui;
};

#endif // FRAME_GUN_DATA_MONITORING_H
