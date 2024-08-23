#include "frame_gun_control_barrel.h"
#include "src/di/di.h"
#include "ui_frame_gun_control_barrel.h"

#include <QMessageBox>
#include <QComboBox>

FrameGunControlBarrel::FrameGunControlBarrel(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::FrameGunControlBarrel)
{
    ui->setupUi(this);

    connect(ui->comboBoxGunBarControlMode, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &FrameGunControlBarrel::onModeChange);
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

void FrameGunControlBarrel::onModeChangeResponse(BaseResponse<GunModeBarrelResponse> resp, bool needConfirm)
{
    qDebug() << Q_FUNC_INFO << "resp code:" << resp.getHttpCode() << "resp msg:" << resp.getMessage();
    qDebug() << Q_FUNC_INFO << "needConfirm:" << needConfirm;

    if (resp.getHttpCode() != 0)
    {
        if (needConfirm)
        {
            QMessageBox::warning(this, "Request Error", QString("Failed to input mode with error: %1").arg(resp.getMessage()));
        }

        return;
    }

    qDebug() << Q_FUNC_INFO << "resp code:" << "resp barrel mode manual: " << resp.getData().getManualMode();

    switch (currentMode)
    {
    case GunBarrelModeEntity::AUTO:
        autoModeUiSetup();
        break;
    case GunBarrelModeEntity::MANUAL:
        manualModeUiSetup();
        break;
    default:
        break;
    }
}

void FrameGunControlBarrel::onDataResponse(BaseResponse<GyroModel> data)
{
}

void FrameGunControlBarrel::onModeChange(int index)
{
    currentMode = (GunBarrelModeEntity::MODE)index;
    gunService->setBarrelMode(currentMode);
}

void FrameGunControlBarrel::onTimeout()
{

}

void FrameGunControlBarrel::onStatusStreamUpdate(GunFeedbackStatusModel model)
{
    Q_UNUSED(model);

    gunService->setTechStatus(GunManagerService::ONLINE); //temp. TODO: move to ui gun status monitoring panel
    gunService->updateOpStatus(); //temp. TODO: move to ui gun status monitoring panel
    updateMode();
}

void FrameGunControlBarrel::onModeCheck()
{
    auto curMode = gunService->getBarrelMode();
    if ((GunBarrelModeEntity::MODE)curMode != currentMode)
    {
        disconnect(ui->comboBoxGunBarControlMode, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &FrameGunControlBarrel::onModeChange);
        ui->comboBoxGunBarControlMode->setCurrentIndex((int)curMode);

        switch (curMode) {
        case GunBarrelModeEntity::NONE:
            noneModeUiSetup();
            break;
        case GunBarrelModeEntity::MANUAL:
            manualModeUiSetup();
            break;
        case GunBarrelModeEntity::AUTO:
            autoModeUiSetup();
            break;
        default:
            break;
        }

        gunService->resetBarrel();

        connect(ui->comboBoxGunBarControlMode, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &FrameGunControlBarrel::onModeChange);

        currentMode = curMode;
    }
}

void FrameGunControlBarrel::on_pushButton_clicked()
{

}

void FrameGunControlBarrel::noneModeUiSetup()
{

}

void FrameGunControlBarrel::manualModeUiSetup()
{
}

void FrameGunControlBarrel::autoModeUiSetup()
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

    connect(gunService, &GunManagerService::OnBarrelModeCheck, this, &FrameGunControlBarrel::onModeCheck);
    connect(gunService, &GunManagerService::OnBarrelModeResponse, this, &FrameGunControlBarrel::onModeChangeResponse);
    connect(statusStream, &GunFeedbackStatusStream::signalDataProcessed, this, &FrameGunControlBarrel::onStatusStreamUpdate);
}
