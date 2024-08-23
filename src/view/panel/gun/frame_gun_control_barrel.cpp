#include "frame_gun_control_barrel.h"
#include "src/di/di.h"
#include "ui_frame_gun_control_barrel.h"

FrameGunControlBarrel::FrameGunControlBarrel(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::FrameGunControlBarrel)
{
    ui->setupUi(this);
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

void FrameGunControlBarrel::on_pushButton_clicked()
{

}

void FrameGunControlBarrel::manualUiSetup()
{
    gunService = DI::getInstance()->getServiceGunManager();
}

void FrameGunControlBarrel::autoUiSetup()
{

}

bool FrameGunControlBarrel::validateInput()
{

}

void FrameGunControlBarrel::setupDI()
{

}
