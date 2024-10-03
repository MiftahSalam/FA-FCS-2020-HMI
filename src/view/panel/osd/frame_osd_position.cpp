#include "frame_osd_position.h"
#include "qtimer.h"

#include "ui_frame_osd_position.h"
#include "src/di/di.h"
#include "src/shared/utils/utils.h"

#include <QMessageBox>

#ifdef USE_LOG4QT
#include <log4qt/logger.h>
LOG4QT_DECLARE_STATIC_LOGGER(logger, FrameOSDPosition)
#else
#include <QDebug>
#endif

FrameOSDPosition::FrameOSDPosition(QWidget *parent) : QWidget(parent),
    ui(new Ui::FrameOSDPosition),
    _cmsPos(DI::getInstance()->getOSDCMSService()->getServiceOSDCMSPosition()),
    _cmsMode(DI::getInstance()->getOSDCMSService()->getServiceOSDCMSMode()),
    _streamPos(DI::getInstance()->getServiceOSDStream()->getServiceOSDStreamPosition())
{
    ui->setupUi(this);

    // init combobox mode (should be auto by default. make sure to sync with osd server)
    currentMode = OSD_MODE::AUTO;
    //    prevMode = OSD_MODE::AUTO;
    currentModeIndx = 0;
    //    prevModeIndx = 0;
    afterResetModeIndx = false;
    ui->mode->setCurrentModeIndex(currentModeIndx);
    ui->pushButton->setEnabled(false);

    ui->inputLatitude->setInputEnable(false);
    ui->inputLatitude->setStatusFailed();

    ui->inputLongitude->setInputEnable(false);
    ui->inputLongitude->setStatusFailed();

    connect(ui->mode, &FrameOSDMode::signal_currentModeChange, this, &FrameOSDPosition::onModeChange);

    timestamp = QDateTime::currentDateTime();

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &FrameOSDPosition::onTimeout);
    timer->start(1000);

    connect(_cmsPos, &OSDCMSPositionData::signal_setPositionResponse, this, &FrameOSDPosition::onDataResponse);
    connect(_cmsMode, &OSDCMSInputMode::signal_setModeResponse, this, &FrameOSDPosition::onModeChangeResponse);
    connect(_streamPos, &OSDStreamPosition::signalDataProcessed, this, &FrameOSDPosition::onStreamReceive);
}

FrameOSDPosition::~FrameOSDPosition()
{
    delete ui;
}

void FrameOSDPosition::setup()
{
    OSDPositionProp prop{
        "Position",
        TextInputProp{
            "Latitude:", "deg", "latInput", Utils::latDecToStringDegree(0.0)},
        TextInputProp{
            "Longitude:", "deg", "latInput", Utils::lonDecToStringDegree(0.0)},
    };
    ui->groupBox->setTitle(prop.title);
    ui->inputLatitude->setup(prop.lat);
    ui->inputLongitude->setup(prop.lon);

    // todo request reset mode to auto
}

void FrameOSDPosition::resetModeIndex()
{
    afterResetModeIndx = true;

    currentModeIndx = prevModeIndx;
    //    currentMode = prevMode;

    //    disconnect(ui->mode, &FrameOSDMode::signal_currentModeChange, this, &FrameOSDPosition::onModeChange);
    //    ui->mode->setCurrentModeIndex(currentModeIndx);
    //    connect(ui->mode, &FrameOSDMode::signal_currentModeChange, this, &FrameOSDPosition::onModeChange);
}

void FrameOSDPosition::onDataResponse(BaseResponse<PositionModel> resp)
{
#ifdef USE_LOG4QT
    logger()->debug() << Q_FUNC_INFO << " -> resp code: " << resp.getHttpCode() << ", resp msg: " << resp.getMessage();
#else
    qDebug() << Q_FUNC_INFO << "resp code:" << resp.getHttpCode() << "resp msg:" << resp.getMessage();
#endif

    if (resp.getHttpCode() != 0)
    {
        //        ui->mode->setCurrentModeIndex((int)OSD_MODE::AUTO);

        QMessageBox::warning(this, "Request Error", QString("Failed to change manual data with error: %1").arg(resp.getMessage()));
        //        autoUiSetup();
        return;

#ifdef USE_LOG4QT
        logger()->debug() << Q_FUNC_INFO
                          << " -> getLatitude: " << resp.getData().getLatitude()
                          << ", getLongitude: " << resp.getData().getLongitude();
#else
        qDebug() << Q_FUNC_INFO << "resp data getLatitude: " << resp.getData().getLatitude()
                 << "resp data getLongitude: " << resp.getData().getLongitude();
#endif
    }
}

void FrameOSDPosition::onModeChangeResponse(const QString datafisis, BaseResponse<InputModeModel> resp, bool needConfirm)
{
    if (datafisis != "position")
    {
        return;
    }

#ifdef USE_LOG4QT
    logger()->debug() << Q_FUNC_INFO << " -> resp code: " << resp.getHttpCode()
                      << ", resp msg: " << resp.getMessage()
                      << ", needConfirm: " << needConfirm
                         ;
#else
    qDebug() << Q_FUNC_INFO << "resp code:" << resp.getHttpCode() << "resp msg:" << resp.getMessage();
    qDebug() << Q_FUNC_INFO << "needConfirm:" << needConfirm;
#endif

    if (resp.getHttpCode() != 0)
    {
        resetModeIndex();
        if (needConfirm)
        {
            QMessageBox::warning(this, "Request Error", QString("Failed to input mode with error: %1").arg(resp.getMessage()));
        }

        return;
    }

#ifdef USE_LOG4QT
    logger()->trace() << Q_FUNC_INFO << " -> resp data. position mode: " << resp.getData().getPosition();
#else
    qDebug() << Q_FUNC_INFO << "resp code:" << "resp data position mode: " << resp.getData().getPosition();
#endif

    //    prevMode = currentMode;
    //    prevModeIndx = currentModeIndx;

    //    auto currentMode = (OSD_MODE)resp.getData().getPosition();
    switch (currentMode)
    {
    case OSD_MODE::AUTO:
        autoUiSetup();
        break;
    case OSD_MODE::MANUAL:
        manualUiSetup();
        on_pushButton_clicked();
        break;
    default:
        break;
    }
}

void FrameOSDPosition::onModeChange(int index)
{
    //    if (afterResetModeIndx) {
    //        QTimer::singleShot(10, this, &FrameOSDPosition::onAfterModeReset);
    //        return;
    //    }

    bool manual_mode;
    switch ((OSD_MODE)index)
    {
    case OSD_MODE::AUTO:
        manual_mode = false;
        currentMode = OSD_MODE::AUTO;
        //        autoUiSetup();
        break;
    case OSD_MODE::MANUAL:
        manual_mode = true;
        currentMode = OSD_MODE::MANUAL;
        //        manualUiSetup();
        break;
    default:
        break;
    }

    _cmsMode->setDataMode("position", manual_mode);
}

void FrameOSDPosition::onAfterModeReset()
{
    afterResetModeIndx = false;
}

void FrameOSDPosition::autoUiSetup()
{
    ui->pushButton->setEnabled(false);

    ui->inputLatitude->setInputEnable(false);
    ui->inputLatitude->setStatusFailed();

    ui->inputLongitude->setInputEnable(false);
    ui->inputLongitude->setStatusFailed();
}

void FrameOSDPosition::manualUiSetup()
{
    ui->pushButton->setEnabled(true);

    ui->inputLatitude->setInputEnable(true);
    ui->inputLatitude->setModeManual();

    ui->inputLongitude->setInputEnable(true);
    ui->inputLongitude->setModeManual();
}

void FrameOSDPosition::onTimeout()
{
    // update ui
    //    qDebug() << Q_FUNC_INFO;

    auto currError = _streamPos->check();
    if (currError.getCode() == ERROR_CODE_MESSAGING_NOT_CONNECTED.first)
    {
        notConnectedUiSetup();
    }
    else if (currError.getCode() == ERROR_CODE_MESSAGING_NO_DATA.first)
    {
        noDataUiSetup();
    }
    else if (currError.getCode() == ERROR_CODE_MESSAGING_DATA_INVALID_FORMAT.first)
    {
        invalidDataUiSetup();
    }

    setErrorInput(currError);

    auto curMode = _cmsMode->getDataMode();
    bool posMode = curMode.getPosition();
    if ((OSD_MODE)posMode != currentMode)
    {
        disconnect(ui->mode, &FrameOSDMode::signal_currentModeChange, this, &FrameOSDPosition::onModeChange);
        if (posMode)
        {
            ui->mode->setCurrentModeIndex(1);
            manualUiSetup();

            float lat = Utils::latStrToDegree(ui->inputLatitude->getCurrentValue());
            float lon = Utils::lonStrToDegree(ui->inputLongitude->getCurrentValue());

            _cmsPos->set(OSDSetPositionRequest(lat, lon));
        }
        else
        {
            ui->mode->setCurrentModeIndex(0);
            autoUiSetup();
        }
        connect(ui->mode, &FrameOSDMode::signal_currentModeChange, this, &FrameOSDPosition::onModeChange);

        currentMode = (OSD_MODE)posMode;
    }
}

void FrameOSDPosition::onStreamReceive(PositionModel model)
{
    //    qDebug() << Q_FUNC_INFO << "position: lat ->" << model.getLatitude() << ", lon ->" << model.getLongitude();
    auto currentMode = (OSD_MODE)_cmsMode->getDataMode().getPosition();
    if (currentMode == OSD_MODE::MANUAL)
    {
        return;
    }

    ui->inputLatitude->setStatusOk();
    ui->inputLongitude->setStatusOk();

    auto currStreamErr = _streamPos->check();

    // validity pitch roll stream check
    ui->inputLatitude->setValue(Utils::latDecToStringDegree(model.getLatitude()));
    ui->inputLongitude->setValue(Utils::lonDecToStringDegree(model.getLongitude()));

    setErrorInput(currStreamErr);
}

void FrameOSDPosition::onUpdatePositionAutoUi()
{
    //    qDebug()<<"UPdate UIIII";
    autoUiSetup();
}

void FrameOSDPosition::notConnectedUiSetup()
{
    auto currentMode = (OSD_MODE)_cmsMode->getDataMode().getPosition();
    if (currentMode == OSD_MODE::MANUAL)
    {
        return;
    }

    ui->pushButton->setEnabled(false);

    ui->inputLatitude->setInputEnable(false);
    ui->inputLatitude->setStatusFailed();

    ui->inputLongitude->setInputEnable(false);
    ui->inputLongitude->setStatusFailed();
}

void FrameOSDPosition::noDataUiSetup()
{
    auto currentMode = (OSD_MODE)_cmsMode->getDataMode().getPosition();
    if (currentMode == OSD_MODE::MANUAL)
    {
        return;
    }

    ui->pushButton->setEnabled(false);

    ui->inputLatitude->setInputEnable(false);
    ui->inputLatitude->setStatusFailed();

    ui->inputLongitude->setInputEnable(false);
    ui->inputLongitude->setStatusFailed();
}

void FrameOSDPosition::invalidDataUiSetup()
{
    auto currentMode = (OSD_MODE)_cmsMode->getDataMode().getPosition();
    if (currentMode == OSD_MODE::MANUAL)
    {
        return;
    }

    ui->pushButton->setEnabled(false);

    ui->inputLatitude->setInputEnable(false);
    ui->inputLatitude->setStatusFailed();

    ui->inputLongitude->setInputEnable(false);
    ui->inputLongitude->setStatusFailed();
}

void FrameOSDPosition::on_pushButton_clicked()
{
    if (!validateInput())
    {
        return;
    }

    try
    {
        float lat = Utils::latStrToDegree(ui->inputLatitude->getCurrentValue());
        float lon = Utils::lonStrToDegree(ui->inputLongitude->getCurrentValue());

        _cmsPos->set(OSDSetPositionRequest(lat, lon));
        //        emit signalChangePositionData(lat,lon);
    }
    catch (...)
    {
        QMessageBox::critical(this, "Fatal Error Position", "Invalid position input");
    }
}

bool FrameOSDPosition::validateInput()
{
    // ==== check latitude ====
    QString lat_str_trimmed = ui->inputLatitude->getCurrentValue();
    lat_str_trimmed.remove(" ");

    QString str = lat_str_trimmed;
    QStringList list1 = str.split("-");

    if (list1.size() < 2)
    {
        QMessageBox::critical(this, "Fatal Error Latitude", "Invalid input value");
        return false;
    }

    QString deg = list1.at(0);
    QStringList list2 = list1.at(1).split("'");

    if (list2.size() != 4)
    {
        QMessageBox::critical(this, "Fatal Error Latitude", "Invalid input value");
        return false;
    }

    QString min = list2.at(0);
    QString sec = list2.at(1);
    QString sign = list2.at(3);

    //    qDebug() << deg  <<min <<sec <<sign;

    bool ok;
    float valuedeg = deg.toFloat(&ok);
    if (!ok)
    {
        QMessageBox::critical(this, "Fatal Error Latitude", "Invalid degree input value.\nValid input : 00-90");
        return false;
    }

    float valuemin = min.toFloat(&ok) / 60.0;
    if ((!ok) || (valuemin >= 1))
    {
        QMessageBox::critical(this, "Fatal Error Latitude", "Invalid minute input value.\nValid input : 00-59");
        return false;
    }

    float valuesec = sec.toFloat(&ok) / 3600.0;
    if ((!ok) || (valuesec > (1.0 / 60.0)))
    {
        QMessageBox::critical(this, "Fatal Error Latitude", "Invalid second input value.\nValid input : 00-59");
        return false;
    }

    float valueLat = valuedeg + valuemin + valuesec;

    if (sign == "S")
        valueLat *= -1.0;
    else if ((sign != "S") && (sign != "N"))
    {
        QMessageBox::critical(this, "Fatal Error Latitude", "Invalid orientation input value.\nValid input : S/N");
        return false;
    }

    if ((valueLat < -90) || (valueLat > 90))
    {
        QMessageBox::critical(this, "Fatal Error Latitude", "Invalid input : out of range.\nValid input : 00-00'00'' - 90-00'00''");
        return false;
    }

    //    qDebug() << Q_FUNC_INFO<<valuedeg << valuemin <<valuesec <<valueLat;

    // ==== check longitude ====
    QString long_str_trimmed = ui->inputLongitude->getCurrentValue();
    long_str_trimmed.remove(" ");

    QString str1 = long_str_trimmed;
    QStringList long_list1 = str1.split("-");

    if (long_list1.size() < 2)
    {
        QMessageBox::critical(this, "Fatal Error Longitude", "Invalid input value");
        return false;
    }

    QString degg = long_list1.at(0);
    QStringList long_list2 = long_list1.at(1).split("'");

    if (long_list2.size() != 4)
    {
        QMessageBox::critical(this, "Fatal Error Longitude", "Invalid input value");
        return false;
    }

    QString minn = long_list2.at(0);
    QString secc = long_list2.at(1);
    QString signn = long_list2.at(3);

    //    qDebug() <<degg  <<minn <<secc <<signn;

    float valuedegg = degg.toFloat(&ok);
    if (!ok)
    {
        QMessageBox::critical(this, "Fatal Error Longitude", "Invalid degree input value.\nValid input : 00-180");
        return false;
    }

    float valueminn = minn.toFloat(&ok) / 60.0;
    if ((!ok) || (valueminn >= 1))
    {
        QMessageBox::critical(this, "Fatal Error Longitude", "Invalid minute input value.\nValid input : 00-59");
        return false;
    }

    float valuesecc = secc.toFloat(&ok) / 3600.0;
    if ((!ok) || (valuesecc > (1.0 / 60.0)))
    {
        QMessageBox::critical(this, "Fatal Error Longitude", "Invalid second input value.\nValid input : 00-59");
        return false;
    }

    float valueLong = valuedegg + valueminn + valuesecc;

    if (signn == "W")
        valueLong *= -1.0;
    else if ((signn != "W") && (signn != "E"))
    {
        QMessageBox::critical(this, "Fatal Error Longitude", "Invalid orientation input value.\nValid input : W/E");
        return false;
    }

    if ((valueLong < -180) || (valueLong > 180))
    {
        QMessageBox::critical(this, "Fatal Error Longitude", "Invalid input : out of range.\nValid input : 00-00'00'' - 180-00'00''");
        return false;
    }

    //    qDebug() << Q_FUNC_INFO<<valuedegg << valueminn <<valuesecc <<valueLong;

    return true;
}

void FrameOSDPosition::setErrorInput(BaseError error)
{
    if (currentMode == OSD_MODE::MANUAL)
    {
        ui->inputLatitude->setToolTip("");
        ui->inputLongitude->setToolTip("");
        return;
    }

    if (error.getCode() == ERROR_NO.first)
    {
        ui->inputLatitude->setStatusOk();
        ui->inputLongitude->setStatusOk();

        ui->inputLatitude->setToolTip("");
        ui->inputLongitude->setToolTip("");
    }
    else
    {
        ui->inputLatitude->setStatusFailed();
        ui->inputLongitude->setStatusFailed();

        ui->inputLatitude->setToolTip(error.getMessage());
        ui->inputLongitude->setToolTip(error.getMessage());
    }

}
