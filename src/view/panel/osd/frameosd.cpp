#include "frameosd.h"
#include "src/di/di.h"
#include "ui_frameosd.h"
//#include "global.h"
//#include "unistd.h"
//#include "math.h"

#include <QMessageBox>
#include <QRegExpValidator>

#define NUMBER_RX "[0-9.-]+$"


FrameOSD::FrameOSD(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::FrameOSD),
    _cmsMode(DI::getInstance()->getOSDCMSService()->getServiceOSDCMSMode()),
    _cmsPosition(DI::getInstance()->getOSDCMSService()->getServiceOSDCMSPosition()),
    _cmsSpeed(DI::getInstance()->getOSDCMSService()->getServiceOSDCMSSpeed()),
    currentMode(InputModeModel(false, false, false))
{
    ;

    ui->setupUi(this);

    setup();
}

FrameOSD::~FrameOSD()
{
    delete ui;
}

void FrameOSD::onChangePositionMode(bool manual_mode)
{
    InputModeModel mode(
                manual_mode,
                currentMode.getSpeed(),
                currentMode.getInersia()
                );
    currentMode = mode;
    lastUpdateMode = "position";

    _cmsMode->set(OSDInputModeRequest(
                      currentMode.getPosition(),
                      currentMode.getSpeed(),
                      currentMode.getInersia()
                      ));
}

void FrameOSD::onChangePositionData(float lat, float lon)
{
    _cmsPosition->set(OSDSetPositionRequest(lat, lon));
}


void FrameOSD::onPositionDataResponse(BaseResponse<PositionModel> resp)
{
    if (resp.getHttpCode() != 0) {
        resetToPrevMode();
        QMessageBox::warning(this, "Request Error", QString("Failed to change manual data with error: %1").arg(resp.getMessage()));


        return;
    }

    qDebug()<<Q_FUNC_INFO<<"resp code:"<<resp.getHttpCode()
           <<"resp msg:"<<resp.getMessage()
          <<"resp data latitude: "<<resp.getData()->getLatitude()
         <<"resp data longitude: "<<resp.getData()->getLongitude()
          ;

    emit signalOnPositionDataResponse(*resp.getData());
}

void FrameOSD::onChangeSpeedMode(bool manual_mode)
{
    InputModeModel mode(
                currentMode.getPosition(),
                manual_mode,
                currentMode.getInersia()
                );
    currentMode = mode;
    lastUpdateMode = "speed";

    _cmsMode->set(OSDInputModeRequest(
                      currentMode.getPosition(),
                      currentMode.getSpeed(),
                      currentMode.getInersia()
                      ));
}

void FrameOSD::onChangeSpeedData(float sog, float cog)
{
    _cmsSpeed->set(OSDSetSpeedRequest(sog, cog));
}

void FrameOSD::onSpeedDataResponse(BaseResponse<SpeedModel> resp)
{
    if (resp.getHttpCode() != 0) {
        resetToPrevMode();
        QMessageBox::warning(this, "Request Error", QString("Failed to change manual data with error: %1").arg(resp.getMessage()));
        emit signalupdateAutoUi();
        return;
    }

    qDebug()<<Q_FUNC_INFO<<"resp code:"<<resp.getHttpCode()
           <<"resp msg:"<<resp.getMessage()
          <<"resp data speed: "<<resp.getData()->getSpeed_OG()
         <<"resp data course: "<<resp.getData()->getCourse_OG()
          ;

    emit signalOnSpeedDataResponse(*resp.getData());
}

void FrameOSD::onChangeInputModeResponse(BaseResponse<InputModeModel> resp)
{
    if (resp.getHttpCode() != 0) {
        resetToPrevMode();
        QMessageBox::warning(this, "Request Error", QString("Failed to input mode with error: %1").arg(resp.getMessage()));
        //added by riyadhi
        emit signalupdateAutoUi();
        //
        return;
    }

    qDebug()<<Q_FUNC_INFO<<"resp code:"<<resp.getHttpCode()
           <<"resp msg:"<<resp.getMessage()
          <<"resp data inersiamode: "<<resp.getData()->getInersia()
         <<"resp data position mode: "<<resp.getData()->getPosition()
        <<"resp data speed mode: "<<resp.getData()->getSpeed()
          ;

    emit signalOnModeResponse(*resp.getData());
}

void FrameOSD::setup()
{
    ui->widgetGyro->setup(OSDGyroProp{
                              "Inertia",
                              TextInputProp{
                                  "Heading:", "deg", "headingInput", "0.0"
                              },
                              TextInputProp{
                                  "Pitch:", "deg", "pitchInput", "0.0"
                              },
                              TextInputProp{
                                  "Roll:", "deg", "rollInput", "0.0"
                              },
                          });
    ui->widgetPosition->setup();
    ui->widgetSpeed->setup();

    connect(ui->widgetPosition, &FrameOSDPosition::signalChangePositionMode, this, &FrameOSD::onChangePositionMode);
    connect(ui->widgetPosition, &FrameOSDPosition::signalChangePositionData, this, &FrameOSD::onChangePositionData);
    connect(this, &FrameOSD::signalOnModeResponse, ui->widgetPosition, &FrameOSDPosition::onModeChangeResponse);
    connect(this, &FrameOSD::signalOnPositionDataResponse, ui->widgetPosition, &FrameOSDPosition::onDataResponse);

    connect(ui->widgetSpeed, &FrameOSDSpeed::signalChangeSpeedMode, this, &FrameOSD::onChangeSpeedMode);
    connect(ui->widgetSpeed, &FrameOSDSpeed::signalChangeSpeedData, this, &FrameOSD::onChangeSpeedData);
    connect(this, &FrameOSD::signalOnModeResponse, ui->widgetSpeed, &FrameOSDSpeed::onModeChangeResponse);
    connect(this, &FrameOSD::signalOnSpeedDataResponse, ui->widgetSpeed, &FrameOSDSpeed::onDataResponse);

    connect(_cmsPosition, &OSDCMSPositionData::signal_setPositionResponse, this, &FrameOSD::onPositionDataResponse);
    connect(_cmsSpeed, &OSDCMSSpeedData::signal_setSpeedResponse, this, &FrameOSD::onSpeedDataResponse);


    connect(_cmsMode, &OSDCMSInputMode::signal_setModeResponse, this, &FrameOSD::onChangeInputModeResponse);


    //added by riyadhi
    connect(this, &FrameOSD::signalupdateAutoUi, ui->widgetPosition, &FrameOSDPosition::onUpdateAutoUi);
    connect(this, &FrameOSD::signalupdateAutoUi, ui->widgetSpeed, &FrameOSDSpeed::onUpdateAutoUi);
}

void FrameOSD::resetToPrevMode()
{
    if (lastUpdateMode == "position") {
        InputModeModel mode(
                    !currentMode.getPosition(),
                    currentMode.getSpeed(),
                    currentMode.getInersia()
                    );
        currentMode = mode;
        ui->widgetPosition->resetModeIndex();
    } else if (lastUpdateMode == "speed") {
        InputModeModel mode(
                    currentMode.getPosition(),
                    !currentMode.getSpeed(),
                    currentMode.getInersia()
                    );
        currentMode = mode;
        ui->widgetSpeed->resetModeIndex();
        //        ui->widgetGyro->resetModeIndex();
    }
}
