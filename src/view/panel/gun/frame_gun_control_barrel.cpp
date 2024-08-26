#include "frame_gun_control_barrel.h"
#include "src/di/di.h"
#include "ui_frame_gun_control_barrel.h"

FrameGunControlBarrel::FrameGunControlBarrel(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::FrameGunControlBarrel)
{
    ui->setupUi(this);

    setupDI();
    setup();
}

FrameGunControlBarrel::~FrameGunControlBarrel()
{
    delete ui;
}

void FrameGunControlBarrel::setup()
{
    BarrelControlProp prop{
        "Gun Barrel Control",
        TextInputProp{
            "Azimuth:", "deg", "AzimuthInput", "0"},
        TextInputProp{
            "Elevation:", "deg", "ElvationInput", "0"}};

    ui->groupBoxGunBarrCon->setTitle(prop.title);
    ui->inputAzimuth->setup(prop.azimuth);
    ui->inputElevation->setup(prop.elevation);

    updateMode();
}

void FrameGunControlBarrel::onDataResponse(BaseResponse<GyroModel> data)
{
}

void FrameGunControlBarrel::onModeChange(int index)
{

}

void FrameGunControlBarrel::onTimeout()
{

}

void FrameGunControlBarrel::onStatusStreamUpdate(GunFeedbackStatusModel model)
{
    Q_UNUSED(model);

    updateMode();
}

void FrameGunControlBarrel::on_pushButton_clicked()
{

}

void FrameGunControlBarrel::manualUiSetup()
{
}

void FrameGunControlBarrel::autoUiSetup()
{

}

bool FrameGunControlBarrel::validateInput()
{

}

void FrameGunControlBarrel::updateMode()
{
    GunManagerService::OPERATIONAL_STATUS opState = gunService->getCurrentOpStat();
    switch (opState) {
    case GunManagerService::STANDBY:
    case GunManagerService::ASSIGNED:
        ui->comboBoxGunBarControlMode->setEnabled(true);
        break;
    default:
        ui->comboBoxGunBarControlMode->setEnabled(false);
        break;
    }
}

void FrameGunControlBarrel::setupDI()
{
    gunService = DI::getInstance()->getServiceGunManager();
    statusStream = DI::getInstance()->getServiceGunStream()->getServiceGunFeedback();

    connect(statusStream, &GunFeedbackStatusStream::signalDataProcessed, this, &FrameGunControlBarrel::onStatusStreamUpdate);
}
