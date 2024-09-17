#include "frame_gun_correction.h"
#include "ui_frame_gun_correction.h"
#include "src/di/di.h"

#include <QMessageBox>

FrameGunCorrection::FrameGunCorrection(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FrameGunCorrection)
{
    ui->setupUi(this);

    int rowCount = ui->tableWidgetCorrection->rowCount();
    for (int var = 0; var < rowCount; var++) {
        ui->tableWidgetCorrection->removeRow(var);
    }
    ui->pushButtonCorrectionApply->setDisabled(true);

    setupDI();
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

        engageService->SetCorrection(azimuth, elevation);
    }
    catch (...)
    {
        QMessageBox::critical(this, "Fatal Error Engagement Barrel Correction", "Request barrel");
    }
}

void FrameGunCorrection::on_engageCorrResponse(BaseResponse<EngagementCorrectionSetResponse> resp)
{
    qDebug() << Q_FUNC_INFO << "resp code:" << resp.getHttpCode() << "resp msg:" << resp.getMessage();

    if (resp.getHttpCode() != 0)
    {
        QMessageBox::critical(this, "Fatal Error Engagement Barrel Correction", QString("Failed to change with error: %1").arg(resp.getMessage()));
        return;
    }

    qDebug() << Q_FUNC_INFO
             << "resp data getAzimuthCorr: " << resp.getData().getAzimuthCorrection()
             << "resp data getElevationCorr: " << resp.getData().getElevationCorrection();
}

void FrameGunCorrection::onAssignmentResponseData(BaseResponse<TrackAssignResponse> resp, bool assign)
{
    if (resp.getHttpCode() == 0) {
        auto currItem = ui->tableWidgetCorrection->findItems(QString::fromStdString(resp.getData().getWeapon()), Qt::MatchCaseSensitive);
        if (assign) {
            if(currItem.isEmpty()) {
                int newRow = ui->tableWidgetCorrection->rowCount();
                ui->tableWidgetCorrection->insertRow(newRow);

                qDebug() << Q_FUNC_INFO << "newRow:" << ui->tableWidgetCorrection->rowCount();

                ui->tableWidgetCorrection->setItem(newRow, 0, new QTableWidgetItem(QString::fromStdString(resp.getData().getWeapon())));
                ui->tableWidgetCorrection->setItem(newRow, 1, new QTableWidgetItem("0.0"));
                ui->tableWidgetCorrection->setItem(newRow, 2, new QTableWidgetItem("0.0"));

                for (int var = 0; var < 3; var++) {
                    ui->tableWidgetCorrection->item(newRow, var)->setTextAlignment(Qt::AlignCenter);
                    ui->tableWidgetCorrection->item(newRow, var)->setForeground(QBrush(QColor(0,255,0)));
                }

                ui->pushButtonCorrectionApply->setDisabled(false);

                engageService->SetCorrection(0., 0.);
            }
        } else {
            if(!currItem.isEmpty()) {
                int curItemRow = currItem.first()->row();
                ui->tableWidgetCorrection->removeRow(curItemRow);
                ui->pushButtonCorrectionApply->setDisabled(true);
            }
        }
    }
}

void FrameGunCorrection::onAssignModeChange(const QString &weapon, const WeaponAssign::WeaponAssignMode &mode)
{
    auto curritems = ui->tableWidgetCorrection->findItems(weapon,Qt::MatchCaseSensitive);
    if(!curritems.isEmpty()) {
        auto curItemRow = curritems.first()->row();
        if (mode == WeaponAssign::NONE) {
            ui->tableWidgetCorrection->removeRow(curItemRow);
            ui->pushButtonCorrectionApply->setDisabled(true);
        }
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

void FrameGunCorrection::setupDI()
{
    engageService = DI::getInstance()->getServiceWeaponTrackAssign();
    waService = DI::getInstance()->getServiceWeaponAssign();

    connect(waService, &WeaponAssignService::OnAssignModeChange, this, &FrameGunCorrection::onAssignModeChange);
    connect(engageService, &WeaponTrackAssignService::signal_assignmentResponseData, this, &FrameGunCorrection::onAssignmentResponseData);
    connect(engageService, &WeaponTrackAssignService::signal_engagementCorrResponse, this, &FrameGunCorrection::on_engageCorrResponse);
}

