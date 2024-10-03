#include "frame_gun_control_barrel.h"
#include "src/di/di.h"
#include "ui_frame_gun_control_barrel.h"

#include <QMessageBox>
#include <QComboBox>

#ifdef USE_LOG4QT
#include <log4qt/logger.h>
LOG4QT_DECLARE_STATIC_LOGGER(logger, FrameGunControlBarrel)
#else
#include <QDebug>
#endif

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

    noneModeUiSetup();
    updateMode();
}

void FrameGunControlBarrel::onModeChangeResponse(BaseResponse<GunModeBarrelResponse> resp, bool needConfirm)
{
#ifdef USE_LOG4QT
    logger()->debug() << Q_FUNC_INFO << " -> resp code: " << resp.getHttpCode()
                      << ", resp msg: " << resp.getMessage()
                      << ", barrel mode manual: " << resp.getData().getManualMode()
                      << ", needConfirm: " << needConfirm;
#else
    qDebug() << Q_FUNC_INFO << "resp code:" << resp.getHttpCode() << "resp msg:" << resp.getMessage();
    qDebug() << Q_FUNC_INFO << "needConfirm:" << needConfirm;
    qDebug() << Q_FUNC_INFO << "resp code:" << "resp barrel mode manual: " << resp.getData().getManualMode();
#endif

    if (resp.getHttpCode() != 0)
    {
        if (needConfirm)
        {
            QMessageBox::warning(this, "Request Error", QString("Failed to input gun mode with error: %1").arg(resp.getMessage()));
        }

        //        return;
        currentMode = gunService->getBarrelMode();

        disconnect(ui->comboBoxGunBarControlMode, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &FrameGunControlBarrel::onModeChange);
        ui->comboBoxGunBarControlMode->setCurrentIndex(0);
        connect(ui->comboBoxGunBarControlMode, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &FrameGunControlBarrel::onModeChange);
    }

    switch (currentMode)
    {
    case GunBarrelModeEntity::NONE:
        noneModeUiSetup();
        break;
    case GunBarrelModeEntity::AUTO:
        autoModeUiSetup();
        break;
    case GunBarrelModeEntity::MANUAL:
        manualModeUiSetup();
        break;
    default:
        break;
    }

    resetBarrel(needConfirm);
}

void FrameGunControlBarrel::onBarrelDataResponse(BaseResponse<GunCommandBarrelResponse> resp, bool needConfirm)
{
#ifdef USE_LOG4QT
    logger()->debug() << Q_FUNC_INFO << " -> resp code: " << resp.getHttpCode()
                      << ", resp msg: " << resp.getMessage()
                      << ", barrel az: " << resp.getData().getAzimuth()
                      << ", barrel el: " << resp.getData().getElevation()
                         ;
#else
    qDebug() << Q_FUNC_INFO << "resp code:" << resp.getHttpCode() << "resp msg:" << resp.getMessage();
    qDebug() << Q_FUNC_INFO
             << "resp data getAzimuth: " << resp.getData().getAzimuth()
             << "resp data getElevation: " << resp.getData().getElevation();
#endif

    if (resp.getHttpCode() != 0)
    {
        if (needConfirm) {
            QMessageBox::critical(this, "Fatal Error Barrel Control", QString("Failed to change manual data with error: %1").arg(resp.getMessage()));
        }

        return;
    }
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

        resetBarrel(false);

        connect(ui->comboBoxGunBarControlMode, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &FrameGunControlBarrel::onModeChange);

        currentMode = curMode;
    }

    updateMode();
}

void FrameGunControlBarrel::on_pushButtonGunBarControlApply_clicked()
{
    if (!validateInput())
    {
        return;
    }

    try
    {
        float azimuth = ui->inputAzimuth->getCurrentValue().toFloat();
        float elevation = ui->inputElevation->getCurrentValue().toFloat();

        gunService->setBarrel(azimuth, elevation);
    }
    catch (...)
    {
        QMessageBox::critical(this, "Fatal Error Barrel Control", "Request barrel");
    }
}

void FrameGunControlBarrel::noneModeUiSetup()
{
    ui->inputAzimuth->setInputEnable(false);
    ui->inputElevation->setInputEnable(false);
    ui->pushButtonGunBarControlApply->setEnabled(false);
}

void FrameGunControlBarrel::manualModeUiSetup()
{
    ui->inputAzimuth->setModeManual();
    ui->inputAzimuth->setInputEnable(true);
    ui->inputElevation->setModeManual();
    ui->inputElevation->setInputEnable(true);
    ui->pushButtonGunBarControlApply->setEnabled(true);
}

void FrameGunControlBarrel::autoModeUiSetup()
{
    ui->inputAzimuth->setInputEnable(false);
    ui->inputElevation->setInputEnable(false);
    ui->pushButtonGunBarControlApply->setEnabled(false);
}

bool FrameGunControlBarrel::validateInput()
{
    bool ok;

    QString azimuth = ui->inputAzimuth->getCurrentValue();
    float valueAz = azimuth.toFloat(&ok);

    if ((valueAz < 0) || (valueAz > 360) || (!ok))
    {
        QMessageBox::critical(this, "Fatal Error Barrel Control", "Invalid azimuth input : out of range.\nValid input : 0-360");
        return false;
    }

    QString elevation = ui->inputElevation->getCurrentValue();
    float valueEl = elevation.toFloat(&ok);

    if ((valueEl < -20) || (valueEl > 80) || (!ok))
    {
        QMessageBox::critical(this, "Fatal Error Barrel Control", "Invalid elevation input : out of range.\nValid input : -20 to 80");
        return false;
    }

    return true;
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

void FrameGunControlBarrel::resetBarrel(bool needConfirm)
{
    gunService->resetBarrel(needConfirm);
    ui->inputAzimuth->setValue("0.0");
    ui->inputElevation->setValue("0.0");
}

void FrameGunControlBarrel::setupDI()
{
    gunService = DI::getInstance()->getServiceGunManager();
    statusStream = DI::getInstance()->getServiceGunStream()->getServiceGunFeedback();

    connect(gunService, &GunManagerService::OnBarrelModeCheck, this, &FrameGunControlBarrel::onModeCheck);
    connect(gunService, &GunManagerService::OnBarrelModeResponse, this, &FrameGunControlBarrel::onModeChangeResponse);
    connect(gunService, &GunManagerService::OnBarrelPositionResponse, this, &FrameGunControlBarrel::onBarrelDataResponse);
    connect(statusStream, &GunFeedbackStatusStream::signalDataProcessed, this, &FrameGunControlBarrel::onStatusStreamUpdate);
}
