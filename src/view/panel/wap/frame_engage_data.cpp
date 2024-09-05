#include "frame_engage_data.h"
#include "ui_frame_engage_data.h"
#include "src/di/di.h"

FrameEngageData::FrameEngageData(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FrameEngageData),
    engagementDataStream(DI::getInstance()->getServiceEngagement()->getServiceEngagement40mm())
{
    ui->setupUi(this);

    connect(engagementDataStream, &EngagementData40mmStream::signalDataProcessed, this, &FrameEngageData::OnStreamEngegementDataReceived);
}

FrameEngageData::~FrameEngageData()
{
    delete ui;
}

void FrameEngageData::OnStreamEngegementDataReceived(EngagementDataModel model)
{
    // update table
    QString _azimuth = QString::number(model.azimuth());
    QString _elevation = QString::number(model.elevation());
    ui->tableWidgetEngData->item(0,3)->setText(_azimuth);
    ui->tableWidgetEngData->item(0,4)->setText(_elevation);
}

void FrameEngageData::onTimeOut()
{

}
