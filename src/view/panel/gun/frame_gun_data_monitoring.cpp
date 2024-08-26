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
    if (model.getRemote() == true){
        ui->labelGunStatAccess->setText("Remote");
        ui->labelGunStatAccess->setStyleSheet(COLOR_OK_STYLESHEET);
    }else{
        ui->labelGunStatAccess->setText("Local");
        ui->labelGunStatAccess->setStyleSheet(COLOR_FAILED_STYLESHEET);
    }

    if (model.getOpMode() == true){
        ui->labelGunOpMode->setText("Yes");
        ui->labelGunOpMode->setStyleSheet(COLOR_OK_STYLESHEET);
    } else {
        ui->labelGunOpMode->setText("No");
        ui->labelGunOpMode->setStyleSheet(COLOR_FAILED_STYLESHEET);
    }

    if (model.getMount() == true){
        ui->labelGunMount->setText("Yes");
        ui->labelGunMount->setStyleSheet(COLOR_OK_STYLESHEET);
    }else{
        ui->labelGunMount->setText("No");
        ui->labelGunMount->setStyleSheet(COLOR_FAILED_STYLESHEET);
    }

    if (model.getBarrelTemperature() == true){
        ui->labelGunBarrelTemp->setText("Yes");
        ui->labelGunBarrelTemp->setStyleSheet(COLOR_OK_STYLESHEET);
    }else{
        ui->labelGunBarrelTemp->setText("No");
        ui->labelGunBarrelTemp->setStyleSheet(COLOR_FAILED_STYLESHEET);
    }

    if (model.getGunReadyToStart() == true){
        ui->labelGunStatReadyToStart->setText("Ready");
        ui->labelGunStatReadyToStart->setStyleSheet(COLOR_OK_STYLESHEET);
    }else{
        ui->labelGunStatReadyToStart->setText("No");
        ui->labelGunStatReadyToStart->setStyleSheet(COLOR_FAILED_STYLESHEET);
    }

    if (model.getGunReadyToFire() == true){
        ui->labelGunReadytoFire->setText("Ready");
        ui->labelGunReadytoFire->setStyleSheet(COLOR_OK_STYLESHEET);
    }else{
        ui->labelGunReadytoFire->setText("No");
        ui->labelGunReadytoFire->setStyleSheet(COLOR_FAILED_STYLESHEET);
    }

    if (model.getFireMode() == true){
        ui->labelGunFireMode->setText("Yes");
        ui->labelGunFireMode->setStyleSheet(COLOR_OK_STYLESHEET);
    }else{
        ui->labelGunFireMode->setText("No");
        ui->labelGunFireMode->setStyleSheet(COLOR_FAILED_STYLESHEET);
    }

    if (model.getBlindArc() == true){
        ui->labelBlarc->setText("Yes");
        ui->labelBlarc->setStyleSheet(COLOR_OK_STYLESHEET);
    }else{
        ui->labelBlarc->setText("No");
        ui->labelBlarc->setStyleSheet(COLOR_FAILED_STYLESHEET);
    }

    if (model.getMissAlignment() == true){
        ui->labelGunMissAlgn->setText("Yes");
        ui->labelGunMissAlgn->setStyleSheet(COLOR_OK_STYLESHEET);
    }else{
        ui->labelGunMissAlgn->setText("No");
        ui->labelGunMissAlgn->setStyleSheet(COLOR_FAILED_STYLESHEET);
    }

    if (model.getMagazine() == true){
        ui->labelGunMag->setText("Yes");
        ui->labelGunMag->setStyleSheet(COLOR_OK_STYLESHEET);
    }else{
        ui->labelGunMag->setText("No");
        ui->labelGunMag->setStyleSheet(COLOR_FAILED_STYLESHEET);
    }
}

void FrameGunDataMonitoring::onStreamBarrelReceive(GunFeedbackBarrelModel model)
{
    ui->labelGunStatAz->setText(QString::number(model.getAzimuth()));
    ui->labelGunStatEl->setText(QString::number(model.getElevation()));

    auto gunBarrelError = gunBarrelStream->check();
    if (gunBarrelError.getCode() == ERROR_NO.first){
        ui->labelGunStatAz->setStyleSheet(COLOR_OK_STYLESHEET);
        ui->labelGunStatEl->setStyleSheet(COLOR_OK_STYLESHEET);
    }else{
        invalidDataUiSetupBarrel();
    }
}

void FrameGunDataMonitoring::onTimeout()
{
    auto gunStatusError = gunStatusStream->check();
    if (gunStatusError.getCode() == ERROR_CODE_MESSAGING_NOT_CONNECTED.first) {
        offlineUiSetup();
    }    else if (gunStatusError.getCode() == ERROR_CODE_MESSAGING_NO_DATA.first){
        noDataUiSetupStatus();
    }else{
        ui->labelGunStatTech->setText("Online");
        ui->labelGunStatTech->setStyleSheet(COLOR_OK_STYLESHEET);
    }

    auto gunBarrelError = gunBarrelStream->check();
    if (gunBarrelError.getCode() == ERROR_CODE_MESSAGING_NOT_CONNECTED.first){
        notConnectedUiSetupBarrel();
    }else if (gunBarrelError.getCode() == ERROR_CODE_MESSAGING_NO_DATA.first){
        noDataUiSetupBarrel();
    }else if (gunBarrelError.getCode() == ERROR_CODE_MESSAGING_DATA_INVALID_FORMAT.first){
        invalidDataUiSetupBarrel();
    }
}

void FrameGunDataMonitoring::notConnectedUiSetupBarrel()
{
    ui->labelGunStatAz->setStyleSheet(COLOR_FAILED_STYLESHEET);
    ui->labelGunStatEl->setStyleSheet(COLOR_FAILED_STYLESHEET);
}

void FrameGunDataMonitoring::noDataUiSetupBarrel()
{
    ui->labelGunStatAz->setStyleSheet(COLOR_FAILED_STYLESHEET);
    ui->labelGunStatEl->setStyleSheet(COLOR_FAILED_STYLESHEET);
}

void FrameGunDataMonitoring::noDataUiSetupStatus()
{
    ui->labelGunOpMode->setStyleSheet(COLOR_FAILED_STYLESHEET);

    ui->labelGunMount->setStyleSheet(COLOR_FAILED_STYLESHEET);

    ui->labelGunStatAccess->setStyleSheet(COLOR_FAILED_STYLESHEET);

    ui->labelGunBarrelTemp->setStyleSheet(COLOR_FAILED_STYLESHEET);

    ui->labelGunStatReadyToStart->setStyleSheet(COLOR_FAILED_STYLESHEET);

    ui->labelGunReadytoFire->setStyleSheet(COLOR_FAILED_STYLESHEET);

    ui->labelGunFireMode->setStyleSheet(COLOR_FAILED_STYLESHEET);

    ui->labelBlarc->setStyleSheet(COLOR_FAILED_STYLESHEET);

    ui->labelGunMissAlgn->setStyleSheet(COLOR_FAILED_STYLESHEET);

    ui->labelGunMag->setStyleSheet(COLOR_FAILED_STYLESHEET);
}

void FrameGunDataMonitoring::invalidDataUiSetupBarrel()
{
    ui->labelGunStatAz->setStyleSheet(COLOR_FAILED_STYLESHEET);
    ui->labelGunStatEl->setStyleSheet(COLOR_FAILED_STYLESHEET);
}

void FrameGunDataMonitoring::offlineUiSetup()
{
    ui->labelGunStatTech->setText("Offline");
    ui->labelGunStatTech->setStyleSheet(COLOR_FAILED_STYLESHEET);

    ui->labelGunOpMode->setText("No");
    ui->labelGunOpMode->setStyleSheet(COLOR_FAILED_STYLESHEET);

    ui->labelGunMount->setText("No");
    ui->labelGunMount->setStyleSheet(COLOR_FAILED_STYLESHEET);

    ui->labelGunStatAccess->setText("Local");
    ui->labelGunStatAccess->setStyleSheet(COLOR_FAILED_STYLESHEET);

    ui->labelGunBarrelTemp->setText("No");
    ui->labelGunBarrelTemp->setStyleSheet(COLOR_FAILED_STYLESHEET);

    ui->labelGunStatReadyToStart->setText("No");
    ui->labelGunStatReadyToStart->setStyleSheet(COLOR_FAILED_STYLESHEET);

    ui->labelGunReadytoFire->setText("No");
    ui->labelGunReadytoFire->setStyleSheet(COLOR_FAILED_STYLESHEET);

    ui->labelGunFireMode->setText("No");
    ui->labelGunFireMode->setStyleSheet(COLOR_FAILED_STYLESHEET);

    ui->labelBlarc->setText("No");
    ui->labelBlarc->setStyleSheet(COLOR_FAILED_STYLESHEET);

    ui->labelGunMissAlgn->setText("No");
    ui->labelGunMissAlgn->setStyleSheet(COLOR_FAILED_STYLESHEET);

    ui->labelGunMag->setText("No");
    ui->labelGunMag->setStyleSheet(COLOR_FAILED_STYLESHEET);
}
