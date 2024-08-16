#include "frame_gun_data_monitoring.h"
#include "ui_frame_gun_data_monitoring.h"

FrameGunDataMonitoring::FrameGunDataMonitoring(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::FrameGunDataMonitoring)
{
    ui->setupUi(this);
}

FrameGunDataMonitoring::~FrameGunDataMonitoring()
{
    delete ui;
}
