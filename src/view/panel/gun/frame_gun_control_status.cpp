#include "frame_gun_control_status.h"
#include "src/di/di.h"
#include "ui_frame_gun_control_status.h"

#include <QMessageBox>

#ifdef USE_LOG4QT
#include <log4qt/logger.h>
LOG4QT_DECLARE_STATIC_LOGGER(logger, FrameGunControlStatus)
#else
#include <QDebug>
#endif

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

void FrameGunControlStatus::onStatusDataResponse(GunCommandStatusResponse resp, bool needConfirm)
{
#ifdef USE_LOG4QT
    logger()->debug() << Q_FUNC_INFO << " -> resp code: " << resp.err().getCode()
                      << ", resp msg: " << resp.err().getMessage()
                         ;
#else
    qDebug() << Q_FUNC_INFO << "resp code:" << resp.getHttpCode() << "resp msg:" << resp.getMessage();
#endif

    if (resp.err().getCode() != ERROR_NO.first)
    {
        if (needConfirm) {
            QMessageBox::critical(this, "Fatal Error Status Control", QString("Failed to change status data with error: %1").arg(resp.err().getMessage()));
        }

        ui->pushButtonControlMount->setChecked(previousStatus.getMount());
        ui->pushButtonControlSingleshoot->setChecked(previousStatus.getSingleShot());
        ui->pushButtonControlFire->setChecked(previousStatus.getFireOrder());
        ui->pushButtonControlProxFuze->setChecked(previousStatus.getProxFuze());
        ui->pushButtonControlSiren->setChecked(previousStatus.getSiren());

        return;
    }

    bool mount = resp.getMount();
    bool single = resp.getSingleShot();
    bool fire = resp.getFireOrder();
    bool prox_fuze = resp.getProxFuze();
    bool siren = resp.getSiren();

    ui->pushButtonControlMount->setText( mount ? "On" : "Off");
    ui->pushButtonControlSingleshoot->setText( single ? "Single" : "Auto");
    ui->pushButtonControlFire->setText( fire ? "Fire" : "Not Fire");
    ui->pushButtonControlProxFuze->setText( prox_fuze ? "On" : "Off");
    ui->pushButtonControlSiren->setText( siren ? "On" : "Off");

    previousStatus.setMount(gunService->getCurrentStatus()->mount());
    previousStatus.setSingleShot(gunService->getCurrentStatus()->single_shot());
    previousStatus.setFireOrder(gunService->getCurrentStatus()->fire());
    previousStatus.setProxFuze(gunService->getCurrentStatus()->proximity());
    previousStatus.setSiren(gunService->getCurrentStatus()->siren());
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
    ui->pushButtonControlSingleshoot->setText("Single");
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

    previousStatus.setMount(gunService->getCurrentStatus()->mount());
    previousStatus.setSingleShot(gunService->getCurrentStatus()->single_shot());
    previousStatus.setFireOrder(gunService->getCurrentStatus()->fire());
    previousStatus.setProxFuze(gunService->getCurrentStatus()->proximity());
    previousStatus.setSiren(gunService->getCurrentStatus()->siren());

    connect(gunService, &GunManagerService::OnBarrelModeCheck, this, &FrameGunControlStatus::onModeCheck);
    connect(gunService, &GunManagerService::OnStatusResponse, this, &FrameGunControlStatus::onStatusDataResponse);
}

void FrameGunControlStatus::on_pushButtonControlMount_clicked(bool checked)
{
    gunService->setStatusMount(checked);
}


void FrameGunControlStatus::on_pushButtonControlSingleshoot_clicked(bool checked)
{
    gunService->setStatusSingleShot(checked);
}


void FrameGunControlStatus::on_pushButtonControlFire_clicked(bool checked)
{
    gunService->setStatusFire(checked);
}


void FrameGunControlStatus::on_pushButtonControlProxFuze_clicked(bool checked)
{
    gunService->setStatusProxFuze(checked);
}


void FrameGunControlStatus::on_pushButtonControlSiren_clicked(bool checked)
{
    gunService->setStatusSiren(checked);
}

