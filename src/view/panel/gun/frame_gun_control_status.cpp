#include "frame_gun_control_status.h"
#include "src/di/di.h"
#include "ui_frame_gun_control_status.h"

#include <QMessageBox>

FrameGunControlStatus::FrameGunControlStatus(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::FrameGunControlStatus)
{
    ui->setupUi(this);

    setupDI();
    setup();
}

FrameGunControlStatus::~FrameGunControlStatus()
{
    delete ui;
}

void FrameGunControlStatus::setup()
{
    resetStatus();
    availableUiSetup(false);
    updateMode();
}

void FrameGunControlStatus::onStatusDataResponse(BaseResponse<GunCommandStatusResponse> resp)
{
    qDebug() << Q_FUNC_INFO << "resp code:" << resp.getHttpCode() << "resp msg:" << resp.getMessage();

    if (resp.getHttpCode() != 0)
    {
        QMessageBox::critical(this, "Fatal Error Status Control", QString("Failed to change status data with error: %1").arg(resp.getMessage()));
        return;
    }

    ui->pushButtonControlMount->setText(resp.getData().getMount() ? "On" : "Off");
    ui->pushButtonControlSingleshoot->setText(resp.getData().getSingleShot() ? "On" : "Off");
    ui->pushButtonControlFire->setText(resp.getData().getFireOrder() ? "Fire" : "Not Fire");
    ui->pushButtonControlProxFuze->setText(resp.getData().getProxFuze() ? "On" : "Off");
    ui->pushButtonControlSiren->setText(resp.getData().getSiren() ? "On" : "Off");

}

void FrameGunControlStatus::onModeCheck()
{
    updateMode();
}

void FrameGunControlStatus::availableUiSetup(bool available)
{
    ui->pushButtonControlMount->setEnabled(available);
    ui->pushButtonControlSingleshoot->setEnabled(available);
    ui->pushButtonControlFire->setEnabled(available);
    ui->pushButtonControlProxFuze->setEnabled(available);
    ui->pushButtonControlSiren->setEnabled(available);
}

void FrameGunControlStatus::updateMode()
{
    GunManagerService::OPERATIONAL_STATUS opState = gunService->getCurrentOpStat();

    if (opState != currentOpMode)
    {
        switch (opState) {
        case GunManagerService::STANDBY:
        case GunManagerService::ASSIGNED:
            availableUiSetup(true);
            break;
        default:
            resetStatus();
            availableUiSetup(false);
            break;
        }

        currentOpMode = opState;
    }
}

void FrameGunControlStatus::resetStatus()
{
    ui->pushButtonControlMount->setText("Off");
    ui->pushButtonControlSingleshoot->setText("Off");
    ui->pushButtonControlFire->setText("No Fire");
    ui->pushButtonControlProxFuze->setText("Off");
    ui->pushButtonControlSiren->setText("Off");

    ui->pushButtonControlMount->setChecked(false);
    ui->pushButtonControlSingleshoot->setChecked(false);
    ui->pushButtonControlFire->setChecked(false);
    ui->pushButtonControlProxFuze->setChecked(false);
    ui->pushButtonControlSiren->setChecked(false);

    gunService->resetStatus();
}

void FrameGunControlStatus::setupDI()
{
    gunService = DI::getInstance()->getServiceGunManager();

    currentOpMode = gunService->getCurrentOpStat();

    connect(gunService, &GunManagerService::OnBarrelModeCheck, this, &FrameGunControlStatus::onModeCheck);
    connect(gunService, &GunManagerService::OnStatusResponse, this, &FrameGunControlStatus::onStatusDataResponse);
}

void FrameGunControlStatus::on_pushButtonControlMount_clicked(bool checked)
{
//    ui->pushButtonControlMount->setText(checked ? "On" : "Off");
    gunService->setStatusMount(checked);
}


void FrameGunControlStatus::on_pushButtonControlSingleshoot_clicked(bool checked)
{
//    ui->pushButtonControlSingleshoot->setText(checked ? "On" : "Off");
    gunService->setStatusSingleShot(checked);
}


void FrameGunControlStatus::on_pushButtonControlFire_clicked(bool checked)
{
//    ui->pushButtonControlFire->setText(checked ? "Fire" : "No Fire");
    gunService->setStatusFire(checked);
}


void FrameGunControlStatus::on_pushButtonControlProxFuze_clicked(bool checked)
{
//    ui->pushButtonControlProxFuze->setText(checked ? "On" : "Off");
    gunService->setStatusProxFuze(checked);
}


void FrameGunControlStatus::on_pushButtonControlSiren_clicked(bool checked)
{
//    ui->pushButtonControlSiren->setText(checked ? "On" : "Off");
    gunService->setStatusSiren(checked);
}

