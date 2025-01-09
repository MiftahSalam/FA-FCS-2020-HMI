#include "frameosd.h"
#include "ui_frameosd.h"
//#include "global.h"
//#include "unistd.h"
//#include "math.h"

#include <QMessageBox>
#include <QRegularExpressionValidator>


#define NUMBER_RX "[0-9.-]+$"


FrameOSD::FrameOSD(QWidget *parent) :
    QFrame(parent), ui(new Ui::FrameOSD)
{
    ui->setupUi(this);

    setup();
}

FrameOSD::~FrameOSD()
{
    delete ui;
}

void FrameOSD::setup()
{
    ui->widgetPosition->setup();
    ui->widgetGyro->setup();
    ui->widgetWaterSpeed->setup();
    ui->widgetSpeed->setup();
    ui->widgetWind->setup();
    ui->widgetWeather->setup();

    ui->widgetWaterSpeed->hide();
    ui->widgetWeather->hide();

//    connect(ui->widgetPosition, &FrameOSDPosition::signalChangePositionMode, this, &FrameOSD::onChangePositionMode);
//    connect(ui->widgetPosition, &FrameOSDPosition::signalChangePositionData, this, &FrameOSD::onChangePositionData);
//    connect(this, &FrameOSD::signalOnModeResponse, ui->widgetPosition, &FrameOSDPosition::onModeChangeResponse);
//    connect(this, &FrameOSD::signalOnPositionDataResponse, ui->widgetPosition, &FrameOSDPosition::onDataResponse);
//    connect(_cmsPosition, &OSDCMSPositionData::signal_setPositionResponse, this, &FrameOSD::onPositionDataResponse);

//    connect(ui->widgetGyro, &FrameOSDGyro::signalChangeGyroMode, this, &FrameOSD::onChangeGyroMode);
//    connect(ui->widgetGyro, &FrameOSDGyro::signalChangeGyroData, this, &FrameOSD::onChangeGyroData);
//    connect(this, &FrameOSD::signalOnModeResponse, ui->widgetGyro, &FrameOSDGyro::onModeChangeResponse);
//    connect(this, &FrameOSD::signalOnGyroDataResponse, ui->widgetGyro, &FrameOSDGyro::onDataResponse);
//    connect(_cmsGyro, &OSDCMSGyroData::signal_setGyroResponse, this, &FrameOSD::onGyroDataResponse);

//    connect(_cmsMode, &OSDCMSInputMode::signal_setModeResponse, this, &FrameOSD::onChangeInputModeResponse);

    //added by riyadhi
//    connect(this, &FrameOSD::signalupdateAutoUi, ui->widgetPosition, &FrameOSDPosition::onUpdatePositionAutoUi);
//    connect(this, &FrameOSD::signalGyroUpdateAutoUi, ui->widgetGyro, &FrameOSDGyro::onUpdateGyroAutoUi);
}
