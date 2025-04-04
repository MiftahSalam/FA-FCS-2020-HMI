#include "frame_gun_data_monitoring.h"
#include "ui_frame_gun_data_monitoring.h"
#include "src/di/di.h"
#include "src/view/shared/constant_ui.h"


FrameGunDataMonitoring::FrameGunDataMonitoring(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::FrameGunDataMonitoring),
    gunManagerService(DI::getInstance()->getServiceGunManager())
{
    ui->setupUi(this);

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &FrameGunDataMonitoring::onTimeout);
    timer->start(1000);

    connect(gunManagerService, &GunManagerService::signal_processedGunStatus, this, &FrameGunDataMonitoring::onStreamStatusReceive);
    connect(gunManagerService, &GunManagerService::signal_processedGunBarrel, this, &FrameGunDataMonitoring::onStreamBarrelReceive);
}

FrameGunDataMonitoring::~FrameGunDataMonitoring()
{
    delete ui;
}

void FrameGunDataMonitoring::onStreamStatusReceive(GunFeedbackStatusModel model)
{
    if (gunManagerService->getCurrentTechStat() != GunManagerService::ONLINE) {
        return;
    }

    if (model.getRemote() == true){
        ui->labelGunStatAccess->setText("Remote");
        ui->labelGunStatAccess->setStyleSheet(COLOR_OK_STYLESHEET);
    }else{
        ui->labelGunStatAccess->setText("Local");
        ui->labelGunStatAccess->setStyleSheet(COLOR_FAILED_STYLESHEET);

        gunManagerService->resetOpStatus();
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
    if (gunManagerService->getCurrentTechStat() != GunManagerService::ONLINE) {
        return;
    }

    ui->labelGunStatAz->setText(QString::number(model.getAzimuth(),'f', 2));
    ui->labelGunStatEl->setText(QString::number(model.getElevation(),'f', 2));

    auto gunBarrelError = gunManagerService->getGunBarrelError();
    if (gunBarrelError.getCode() == ERROR_NO.first){
        ui->labelGunStatAz->setStyleSheet(COLOR_OK_STYLESHEET);
        ui->labelGunStatEl->setStyleSheet(COLOR_OK_STYLESHEET);
    }else{
        invalidDataUiSetupBarrel();
    }
}

void FrameGunDataMonitoring::onTimeout()
{
    updateTechStatus();
    updateOperationalUiSetup();
    updateBarrelStatus();
}

void FrameGunDataMonitoring::updateTechStatus()
{
    auto gunStatusError = gunManagerService->getGunStatusError();
    auto gunBarrelError = gunManagerService->getGunBarrelError();

    if (gunStatusError.getCode() == ERROR_NO.first && gunBarrelError.getCode() == ERROR_NO.first) {
        ui->labelGunStatTech->setText("Online");
        ui->labelGunStatTech->setStyleSheet(COLOR_OK_STYLESHEET);

        gunManagerService->setTechStatus(GunManagerService::ONLINE);
    } else {
        offlineUiSetup();
        gunManagerService->setTechStatus(GunManagerService::OFFLINE);
        gunManagerService->resetStatus();
    }
}

void FrameGunDataMonitoring::updateBarrelStatus()
{
    auto gunStatusError = gunManagerService->getGunStatusError();
    auto gunBarrelError = gunManagerService->getGunBarrelError();

    if (gunStatusError.getCode() != ERROR_NO.first && gunBarrelError.getCode() != ERROR_NO.first) {
        notConnectedUiSetupBarrel();

        gunManagerService->resetBarrel(false);

        ui->labelGunStatAz->setText(QString::number(gunManagerService->getCurrentBarrel()->azimuth()));
        ui->labelGunStatEl->setText(QString::number(gunManagerService->getCurrentBarrel()->elevation()));
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
    ui->labelGunStatTech->setText("Online");
    ui->labelGunStatTech->setStyleSheet(COLOR_OK_STYLESHEET);

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

void FrameGunDataMonitoring::updateOperationalUiSetup()
{
    gunManagerService->updateOpStatus();

    GunManagerService::OPERATIONAL_STATUS stat = gunManagerService->getCurrentOpStat();
    switch (stat) {
    case GunManagerService::NOT_AVAIL:
        ui->labelGunStatOp->setText("Not Available");
        ui->labelGunStatOp->setStyleSheet(COLOR_FAILED_STYLESHEET);
        break;
    case GunManagerService::STANDBY:
        ui->labelGunStatOp->setText("Standby");
        ui->labelGunStatOp->setStyleSheet(COLOR_MANUAL_STYLESHEET);
        break;
    case GunManagerService::ASSIGNED:
        ui->labelGunStatOp->setText("Assigned");
        ui->labelGunStatOp->setStyleSheet(COLOR_OK_STYLESHEET);
        break;
    default:
        break;
    }
}
