#include "frame_gun_correction.h"
#include "ui_frame_gun_correction.h"
#include "src/di/di.h"

#include <QMessageBox>

FrameGunCorrection::FrameGunCorrection(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FrameGunCorrection)
{
    ui->setupUi(this);
}

FrameGunCorrection::~FrameGunCorrection()
{
    delete ui;
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

        engageCorrService->setCorrection(EngagementCorrectionSetRequest(azimuth, elevation));

    }
    catch (...)
    {
        QMessageBox::critical(this, "Fatal Error Barrel Correction", "Request barrel");
    }

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

