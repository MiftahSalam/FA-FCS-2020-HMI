#include "frame_osd_position.h"
#include "qtimer.h"

#include "ui_frame_osd_position.h"
#include "src/di/di.h"
#include "src/shared/utils/utils.h"

#include <QMessageBox>

FrameOSDPosition::FrameOSDPosition(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FrameOSDPosition),
    _cmsPos(DI::getInstance()->getOSDCMSService()->getServiceOSDCMSPosition()),
    _streamPos(DI::getInstance()->getServiceOSDStream()->getServiceOSDStreamPosition())
{
    ui->setupUi(this);

    //init combobox mode (should be auto by default. make sure to sync with osd server)
    currentMode = OSD_MODE::AUTO;
    currentModeIndx = 0;
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
            "Latitude:", "deg", "latInput", Utils::latDecToStringDegree(0.0)
        },
        TextInputProp{
            "Longitude:", "deg", "latInput", Utils::lonDecToStringDegree(0.0)
        },
    };
    ui->groupBox->setTitle(prop.title);
    ui->inputLatitude->setup(prop.lat);
    ui->inputLongitude->setup(prop.lon);

    //todo request reset mode to auto
}

void FrameOSDPosition::resetModeIndex()
{
    afterResetModeIndx = true;
    ui->mode->setCurrentModeIndex(currentModeIndx);
}

void FrameOSDPosition::onDataResponse(PositionModel data)
{
    Q_UNUSED(data); //temporary
    //todo handle response
    qDebug()<<Q_FUNC_INFO;
}

void FrameOSDPosition::onModeChangeResponse(InputModeModel mode)
{
    Q_UNUSED(mode); //temporary
    //handle response
    qDebug()<<Q_FUNC_INFO;

    if (mode.getPosition()) {
        currentModeIndx = int(OSD_MODE::MANUAL);
        manualUiSetup();
        ui->pushButton->click();
    } else {
        currentModeIndx = int(OSD_MODE::AUTO);
        autoUiSetup();
    }
}

void FrameOSDPosition::onModeChange(int index)
{
    if (afterResetModeIndx) {
        QTimer::singleShot(10, this, &FrameOSDPosition::onAfterModeReset);
        return;
    }

    currentMode = (OSD_MODE)index;
    switch (currentMode) {
    case OSD_MODE::AUTO:
        emit signalChangePositionMode(false);
        break;
    case OSD_MODE::MANUAL:
        emit signalChangePositionMode(true);
        manualUiSetup();
        break;
    default:
        break;
    }
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
    //update ui
    qDebug()<<Q_FUNC_INFO;
    auto currError = _streamPos->check();
    if (currError.getCode() == ERROR_CODE_MESSAGING_NOT_CONNECTED.first) {
        notConnectedUiSetup();
    } else if (currError.getCode() == ERROR_CODE_MESSAGING_NO_DATA.first) {
        noDataUiSetup();
    } else if (currError.getCode() == ERROR_CODE_MESSAGING_DATA_INVALID_FORMAT.first) {
        invalidDataUiSetup();
    }

}

void FrameOSDPosition::onStreamReceive(PositionModel model)
{
    qDebug()<<Q_FUNC_INFO<<"position: lat ->"<<model.getLatitude()<<", lon ->"<<model.getLongitude();
    if (currentMode == OSD_MODE::MANUAL) {
        return;
    }

    ui->inputLatitude->setValue(Utils::latDecToStringDegree(model.getLatitude()));
    ui->inputLatitude->setStatusOk();

    ui->inputLongitude->setValue(Utils::lonDecToStringDegree(model.getLongitude()));
    ui->inputLongitude->setStatusOk();
}

void FrameOSDPosition::onUpdateAutoUi()
{
//    qDebug()<<"UPdate UIIII";
    autoUiSetup();
}

void FrameOSDPosition::notConnectedUiSetup()
{
    if (currentMode == OSD_MODE::MANUAL) {
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
    if (currentMode == OSD_MODE::MANUAL) {
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
    if (currentMode == OSD_MODE::MANUAL) {
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
    if (!validateInput()) {
        return;
    }

    try {
        float lat = Utils::latStrToDegree(ui->inputLatitude->getCurrentValue());
        float lon = Utils::lonStrToDegree(ui->inputLongitude->getCurrentValue());

        emit signalChangePositionData(lat,lon);
    } catch (...) {
        QMessageBox::critical(this, "Fatal Error Position", "Invalid position input" );
    }
}

bool FrameOSDPosition::validateInput()
{
    // ==== check latitude ====
        QString lat_str_trimmed = ui->inputLatitude->getCurrentValue();
        lat_str_trimmed.remove(" ");

        QString str = lat_str_trimmed;
        QStringList list1 = str.split("-");

        if(list1.size()<2)
        {
            QMessageBox::critical(this, "Fatal Error Latitude", "Invalid input value" );
            return false;
        }

        QString deg = list1.at(0);
        QStringList list2 = list1.at(1).split("'");

        if(list2.size()!=4)
        {
            QMessageBox::critical(this, "Fatal Error Latitude", "Invalid input value" );
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
            QMessageBox::critical(this, "Fatal Error Latitude", "Invalid degree input value.\nValid input : 00-90" );
            return false;
        }

        float valuemin = min.toFloat(&ok)/60.0;
        if ((!ok) || (valuemin >= 1))
        {
            QMessageBox::critical(this, "Fatal Error Latitude", "Invalid minute input value.\nValid input : 00-59" );
            return false;
        }

        float valuesec = sec.toFloat(&ok)/3600.0;
        if ((!ok) || (valuesec > (1.0/60.0)))
        {
            QMessageBox::critical(this, "Fatal Error Latitude", "Invalid second input value.\nValid input : 00-59" );
            return false;
        }

        float valueLat = valuedeg+valuemin+valuesec;

        if(sign == "S")
            valueLat *= -1.0;
        else if((sign != "S") && (sign != "N"))
        {
            QMessageBox::critical(this, "Fatal Error Latitude", "Invalid orientation input value.\nValid input : S/N" );
            return false;
        }

        if ((valueLat < -90) || (valueLat > 90) )
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

        if(long_list1.size()<2)
        {
            QMessageBox::critical(this, "Fatal Error Longitude", "Invalid input value" );
            return false;
        }

        QString degg = long_list1.at(0);
        QStringList long_list2 = long_list1.at(1).split("'");

        if(long_list2.size()!=4)
        {
            QMessageBox::critical(this, "Fatal Error Longitude", "Invalid input value" );
            return false;
        }

        QString minn = long_list2.at(0);
        QString secc = long_list2.at(1);
        QString signn = long_list2.at(3);

    //    qDebug() <<degg  <<minn <<secc <<signn;

        bool ok1;
        float valuedegg = degg.toFloat(&ok1);
        if (!ok1)
        {
            QMessageBox::critical(this, "Fatal Error Longitude", "Invalid degree input value.\nValid input : 00-180" );
            return false;
        }

        float valueminn = minn.toFloat()/60.0;
        if ((!ok) || (valueminn >= 1))
        {
            QMessageBox::critical(this, "Fatal Error Longitude", "Invalid minute input value.\nValid input : 00-59" );
            return false;
        }

        float valuesecc = secc.toFloat()/3600.0;
        if ((!ok) || (valuesecc > (1.0/60.0)))
        {
            QMessageBox::critical(this, "Fatal Error Longitude", "Invalid second input value.\nValid input : 00-59" );
            return false;
        }

        float valueLong = valuedegg+valueminn+valuesecc;

        if(signn == "W")
            valueLong *= -1.0;
        else if ((signn != "W") && (signn != "E"))
        {
            QMessageBox::critical(this, "Fatal Error Longitude", "Invalid orientation input value.\nValid input : W/E" );
            return false;
        }

        if ((valueLong < -180) || (valueLong > 180) )
        {
            QMessageBox::critical(this, "Fatal Error Longitude", "Invalid input : out of range.\nValid input : 00-00'00'' - 180-00'00''" );
            return false;
        }

        //    qDebug() << Q_FUNC_INFO<<valuedegg << valueminn <<valuesecc <<valueLong;

        return true;
}
