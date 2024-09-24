#include "frame_engage_data.h"
#include "src/shared/common/errors/err_messaging.h"
#include "ui_frame_engage_data.h"
#include "src/di/di.h"

FrameEngageData::FrameEngageData(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FrameEngageData),
    engagementDataStream(DI::getInstance()->getServiceEngagementStream()->getServiceEngagement40mm()),
    wtaService(DI::getInstance()->getServiceWeaponTrackAssign()),
    waService(DI::getInstance()->getServiceWeaponAssign()),
    currError(ErrMessagingDataInvalidFormat())
{
    ui->setupUi(this);

    timer = new QTimer(this);

    int rowCount = ui->tableWidgetEngData->rowCount();
    for (int var = 0; var < rowCount; var++) {
        ui->tableWidgetEngData->removeRow(var);
    }

    connect(timer, &QTimer::timeout, this, &FrameEngageData::onTimeOut);

    connect(waService, &WeaponAssignService::OnAssignModeChange, this, &FrameEngageData::onAssignModeChange);
    connect(engagementDataStream, &EngagementData40mmStream::signalDataProcessed, this, &FrameEngageData::OnStreamEngegementDataReceived);
    connect(wtaService, &WeaponTrackAssignService::signal_assignmentResponseData, this, &FrameEngageData::onAssignmentResponseData);

    timer->start(1000);
}

FrameEngageData::~FrameEngageData()
{
    delete ui;
}

void FrameEngageData::onAssignModeChange(const QString &weapon, const WeaponAssign::WeaponAssignMode &mode)
{
    if (mode == WeaponAssign::NONE) {
        resetEngage(weapon);
    }
}

void FrameEngageData::resetEngage(const QString weapon)
{
    engagementDataStream->DeleteEngage(weapon);

    auto curritems = ui->tableWidgetEngData->findItems(weapon,Qt::MatchCaseSensitive);
    if(!curritems.isEmpty()) {
        auto curItemRow = curritems.first()->row();
        ui->tableWidgetEngData->removeRow(curItemRow);
    }
}

void FrameEngageData::OnStreamEngegementDataReceived(EngagementDataModel model)
{
    auto curr40mmitems = ui->tableWidgetEngData->findItems(
                QString::fromStdString(model.weapon()),
                Qt::MatchCaseSensitive);

    qDebug() << Q_FUNC_INFO << "curr40mmitems:" << curr40mmitems;

    if(!curr40mmitems.isEmpty()) {
        // update table
        auto cur40mmItemRow = curr40mmitems.first();
        QString _azimuth = QString::number(model.azimuth(),'f',2);
        QString _elevation = QString::number(model.elevation(),'f',2);
        ui->tableWidgetEngData->item(cur40mmItemRow->row(),3)->setText(_azimuth);
        ui->tableWidgetEngData->item(cur40mmItemRow->row(),4)->setText(_elevation);
    }
}

void FrameEngageData::onTimeOut()
{
    auto curr40mmitems = ui->tableWidgetEngData->findItems(
                "40mm",
                Qt::MatchCaseSensitive);

    if(!curr40mmitems.isEmpty()) {
        auto curr_error = engagementDataStream->check();
        auto cur40mmItemRow = curr40mmitems.first();

        if (curr_error.getCode() != currError.getCode()) {
            currError = curr_error;

            if (currError.getCode() != ERROR_NO.first)
            {
                for (int var = 0; var < 5; var++) {
                    ui->tableWidgetEngData->item(cur40mmItemRow->row(), var)->setForeground(QBrush(QColor(255,0,0)));
                }

            } else {
                for (int var = 0; var < 5; var++) {
                    ui->tableWidgetEngData->item(cur40mmItemRow->row(), var)->setForeground(QBrush(QColor(0,255,0)));
                }
            }
        }
    }
}

void FrameEngageData::onAssignmentResponseData(BaseResponse<TrackAssignResponse> resp, bool assign)
{
    qDebug() << Q_FUNC_INFO << "resp code:" << resp.getHttpCode() << "resp msg:" << resp.getMessage();

    if (resp.getHttpCode() == 0)
    {
        auto curr40mmitem = ui->tableWidgetEngData->findItems(QString::fromStdString(resp.getData().getWeapon()), Qt::MatchCaseSensitive);
        if (assign) {
            if(curr40mmitem.isEmpty()) {
                engagementDataStream->CreateEngage(resp.getData().getTrackId());

                int newRow = ui->tableWidgetEngData->rowCount();
                ui->tableWidgetEngData->insertRow(newRow);

                qDebug() << Q_FUNC_INFO << "newRow:" << ui->tableWidgetEngData->rowCount();

                ui->tableWidgetEngData->setItem(newRow, 0, new QTableWidgetItem("40mm"));
                ui->tableWidgetEngData->setItem(newRow, 1, new QTableWidgetItem("Engageable"));
                ui->tableWidgetEngData->setItem(newRow, 2, new QTableWidgetItem(QString::number(resp.getData().getTrackId())));
                ui->tableWidgetEngData->setItem(newRow, 3, new QTableWidgetItem("-"));
                ui->tableWidgetEngData->setItem(newRow, 4, new QTableWidgetItem("-"));

                for (int var = 0; var < 5; var++) {
                    ui->tableWidgetEngData->item(newRow, var)->setTextAlignment(Qt::AlignCenter);
                    ui->tableWidgetEngData->item(newRow, var)->setForeground(QBrush(QColor(0,255,0)));
                }
            }
        } else {
            resetEngage(QString::fromStdString(resp.getData().getWeapon()));
        }
    }
}
