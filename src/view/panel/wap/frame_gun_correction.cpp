#include "frame_gun_correction.h"
#include "ui_frame_gun_correction.h"
#include "src/di/di.h"

#include <QMessageBox>

FrameGunCorrection::FrameGunCorrection(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FrameGunCorrection)
{
    ui->setupUi(this);
    setupDI();
}

FrameGunCorrection::~FrameGunCorrection()
{
    delete ui;
}

void FrameGunCorrection::setup()
{

}

void FrameGunCorrection::on_pushButtonCorrectionApply_clicked()
{
    if (!validateInput())
    {
        return;
    }

    try
    {
        float azimuth = (ui->tableWidgetCorrection->item(0,1)->text()).toFloat();
        float elevation = (ui->tableWidgetCorrection->item(0,2)->text()).toFloat();

        engageService->SetCorrection(azimuth, elevation);

    }
    catch (...)
    {
        QMessageBox::critical(this, "Fatal Error Barrel Correction", "Request barrel");
    }

}

void FrameGunCorrection::on_engageCorrResponse(BaseResponse<EngagementCorrectionSetResponse> resp)
{
    qDebug() << Q_FUNC_INFO << "resp code:" << resp.getHttpCode() << "resp msg:" << resp.getMessage();

    if (resp.getHttpCode() != 0)
    {
        QMessageBox::critical(this, "Fatal Error Barrel Correction", QString("Failed to change with error: %1").arg(resp.getMessage()));
        return;
    }

    qDebug() << Q_FUNC_INFO
             << "resp data getAzimuthCorr: " << resp.getData().getAzimuthCorrection()
             << "resp data getElevationCorr: " << resp.getData().getElevationCorrection();
}

bool FrameGunCorrection::validateInput()
{
    bool ok;

    QString azimuthCorr = ui->tableWidgetCorrection->item(0,1)->text();
    float valueAz = azimuthCorr.toFloat(&ok);

    if ((valueAz < -30) or (valueAz >30) or (!ok))
    {
        QMessageBox::critical(this, "Fatal Error Barrel Correction", "Invalid azimuth input : out of range.\nValid input : -30 to 30");
        return false;
    }

    QString elevationCorr = ui->tableWidgetCorrection->item(0,2)->text();
    float valueEl = elevationCorr.toFloat(&ok);

    if ((valueEl < -20) or (valueEl >20) or (!ok))
    {
        QMessageBox::critical(this, "Fatal Error Barrel Correction", "Invalid elevation input : out of range.\nValid input : -20 to 20");
        return false;
    }

    return true;
}

void FrameGunCorrection::setupDI()
{
    engageService = DI::getInstance()->getServiceWeaponTrackAssign();

    connect(engageService, &WeaponTrackAssignService::signal_engagementCorrResponse, this, &FrameGunCorrection::on_engageCorrResponse);
}

