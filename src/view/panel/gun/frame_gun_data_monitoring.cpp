#include "frame_gun_data_monitoring.h"
#include "ui_frame_gun_data_monitoring.h"
#include "src/di/di.h"


FrameGunDataMonitoring::FrameGunDataMonitoring(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::FrameGunDataMonitoring),
    gunStatusStream(DI::getInstance()->getServiceGunStream()->getServiceGunFeedback()),
    gunBarrelStream(DI::getInstance()->getServiceGunStream()->getServiceGunBarrel())
{
    ui->setupUi(this);

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &FrameGunDataMonitoring::onTimeout);
    timer->start(1000);

    connect(gunStatusStream, &GunFeedbackStatusStream::signalDataProcessed, this, &FrameGunDataMonitoring::onStreamStatusReceive);
    connect(gunBarrelStream, &GunFeedbackBarrelStream::signalDataProcessed, this, &FrameGunDataMonitoring::onStreamBarrelReceive);
}

FrameGunDataMonitoring::~FrameGunDataMonitoring()
{
    delete ui;
}

void FrameGunDataMonitoring::onStreamStatusReceive(GunFeedbackStatusModel model)
{
    if (model.getOpMode() == true){
        ui->labelGunStatOp->setText("Standby");
        ui->labelGunStatOp->setStyleSheet(COLOR_OK_STYLESHEET);
    } else {
        ui->labelGunStatOp->setText("Not Ready");
        ui->labelGunStatOp->setStyleSheet(COLOR_FAILED_STYLESHEET);
    }
}

void FrameGunDataMonitoring::onStreamBarrelReceive(GunFeedbackBarrelModel model)
{
    ui->labelGunStatAz->setText(QString::number(model.getAzimuth()));
    ui->labelGunStatEl->setText(QString::number(model.getElevation()));
}

void FrameGunDataMonitoring::onTimeout()
{
    auto gunStatusError = gunStatusStream->check();
    if (gunStatusError.getCode() == ERROR_CODE_MESSAGING_NOT_CONNECTED.first) {
        ui->labelGunStatTech->setText("Offline");
        ui->labelGunStatTech->setStyleSheet(COLOR_FAILED_STYLESHEET);
    }else{
        ui->labelGunStatTech->setText("Online");
        ui->labelGunStatTech->setStyleSheet(COLOR_OK_STYLESHEET);
    }

    auto gunBarrelError = gunBarrelStream->check();
    if (gunBarrelError.getCode() == ERROR_CODE_MESSAGING_NOT_CONNECTED.first){
        ui->labelGunStatAz->setStyleSheet(COLOR_FAILED_STYLESHEET);
        ui->labelGunStatEl->setStyleSheet(COLOR_FAILED_STYLESHEET);
    }else{
        ui->labelGunStatAz->setStyleSheet(COLOR_OK_STYLESHEET);
        ui->labelGunStatEl->setStyleSheet(COLOR_OK_STYLESHEET);
    }
}
