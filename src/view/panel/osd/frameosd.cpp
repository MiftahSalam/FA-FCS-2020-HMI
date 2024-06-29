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

    connect(ui->widgetPosition, &FrameOSDPosition::signalChangePositionMode, this, &FrameOSD::onChangePositionMode);
    connect(ui->widgetPosition, &FrameOSDPosition::signalChangePositionData, this, &FrameOSD::onChangePositionData);
    connect(this, &FrameOSD::signalOnModeResponse, ui->widgetPosition, &FrameOSDPosition::onModeChangeResponse);
    connect(this, &FrameOSD::signalOnPositionDataResponse, ui->widgetPosition, &FrameOSDPosition::onDataResponse);

    connect(_cmsMode, &OSDCMSInputMode::signal_setModeResponse, this, &FrameOSD::onChangeInputModeResponse);

    connect(_cmsPosition, &OSDCMSPositionData::signal_setPositionResponse, this, &FrameOSD::onPositionDataResponse);

    //added by riyadhi
    connect(this, &FrameOSD::signalupdateAutoUi, ui->widgetPosition, &FrameOSDPosition::onUpdateAutoUi);
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
        //        ui->widgetGyro->resetModeIndex();
    }
}
