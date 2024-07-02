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
    _cmsGyro(DI::getInstance()->getOSDCMSService()->getServiceOSDCMSGyro()),
    currentMode(InputModeModel(false, false, false))
{
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
        emit signalupdateAutoUi();
        return;
    }

    qDebug()<<Q_FUNC_INFO<<"resp code:"<<resp.getHttpCode()
           <<"resp msg:"<<resp.getMessage()
          <<"resp data latitude: "<<resp.getData()->getLatitude()
         <<"resp data longitude: "<<resp.getData()->getLongitude()
          ;

    emit signalOnPositionDataResponse(*resp.getData());
}

void FrameOSD::onChangeGyroMode(bool manual_mode)
{
    InputModeModel mode(
                currentMode.getPosition(),
                currentMode.getSpeed(),
                manual_mode
                );
    currentMode = mode;
    lastUpdateMode = "inertia";

    _cmsMode->set(OSDInputModeRequest(
                      currentMode.getPosition(),
                      currentMode.getSpeed(),
                      currentMode.getInersia()
                      ));
}

void FrameOSD::onChangeGyroData(float heading, float pitch, float roll)
{
    _cmsGyro->set(OSDSetGyroRequest(heading, pitch, roll));
}

void FrameOSD::onGyroDataResponse(BaseResponse<GyroModel> resp)
{
    if (resp.getHttpCode() != 0) {
        resetToPrevMode();
        QMessageBox::warning(this, "Request Error", QString("Failed to change manual data with error: %1").arg(resp.getMessage()));
//        emit signalupdateAutoUi();
        signalGyroUpdateAutoUi();
        return;
    }

    qDebug()<<Q_FUNC_INFO<<"resp code:"<<resp.getHttpCode()
           <<"resp msg:"<<resp.getMessage()
          <<"resp data heading: "<<resp.getData()->getHeading()
         <<"resp data Pitch: "<<resp.getData()->getPicth()
        <<"resp data Roll: "<<resp.getData()->getRoll()
          ;

    emit signalOnGyroDataResponse(*resp.getData());
}

void FrameOSD::onChangeInputModeResponse(BaseResponse<InputModeModel> resp)
{
    if (resp.getHttpCode() != 0) {
        resetToPrevMode();
        QMessageBox::warning(this, "Request Error", QString("Failed to input mode with error: %1").arg(resp.getMessage()));
        //added by riyadhi
        emit signalupdateAutoUi();
        emit signalGyroUpdateAutoUi();
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
    ui->widgetPosition->setup();
    ui->widgetGyro->setup();

    connect(ui->widgetPosition, &FrameOSDPosition::signalChangePositionMode, this, &FrameOSD::onChangePositionMode);
    connect(ui->widgetPosition, &FrameOSDPosition::signalChangePositionData, this, &FrameOSD::onChangePositionData);
    connect(this, &FrameOSD::signalOnModeResponse, ui->widgetPosition, &FrameOSDPosition::onModeChangeResponse);
    connect(this, &FrameOSD::signalOnPositionDataResponse, ui->widgetPosition, &FrameOSDPosition::onDataResponse);
    connect(_cmsPosition, &OSDCMSPositionData::signal_setPositionResponse, this, &FrameOSD::onPositionDataResponse);

    connect(ui->widgetGyro, &FrameOSDGyro::signalChangeGyroMode, this, &FrameOSD::onChangeGyroMode);
    connect(ui->widgetGyro, &FrameOSDGyro::signalChangeGyroData, this, &FrameOSD::onChangeGyroData);
    connect(this, &FrameOSD::signalOnModeResponse, ui->widgetGyro, &FrameOSDGyro::onModeChangeResponse);
    connect(this, &FrameOSD::signalOnGyroDataResponse, ui->widgetGyro, &FrameOSDGyro::onDataResponse);
    connect(_cmsGyro, &OSDCMSGyroData::signal_setGyroResponse, this, &FrameOSD::onGyroDataResponse);

    connect(_cmsMode, &OSDCMSInputMode::signal_setModeResponse, this, &FrameOSD::onChangeInputModeResponse);

    //added by riyadhi
    connect(this, &FrameOSD::signalupdateAutoUi, ui->widgetPosition, &FrameOSDPosition::onUpdatePositionAutoUi);
    connect(this, &FrameOSD::signalGyroUpdateAutoUi, ui->widgetGyro, &FrameOSDGyro::onUpdateGyroAutoUi);
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
//        ui->widgetSpee->resetModeIndex();
    } else if (lastUpdateMode == "inertia"){
        InputModeModel mode(
                    currentMode.getPosition(),
                    currentMode.getSpeed(),
                    !currentMode.getInersia());
        currentMode = mode;
        ui->widgetGyro->resetModeIndex();
    }
}
