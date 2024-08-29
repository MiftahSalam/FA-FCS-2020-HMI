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
    ui->pushButtonControlFire->setText("Off");
    ui->pushButtonControlProxFuze->setText("Off");
    ui->pushButtonControlSiren->setText("Off");

    ui->pushButtonControlMount->setChecked(false);
    ui->pushButtonControlSingleshoot->setChecked(false);
    ui->pushButtonControlFire->setChecked(false);
    ui->pushButtonControlProxFuze->setChecked(false);
    ui->pushButtonControlSiren->setChecked(false);
}

void FrameGunControlStatus::setupDI()
{
    gunService = DI::getInstance()->getServiceGunManager();

    connect(gunService, &GunManagerService::OnBarrelModeCheck, this, &FrameGunControlStatus::onModeCheck);
}
