#include "osd_stream_gyro.h"
#include "src/shared/common/errors/err_json_parse.h"
#include "src/shared/common/errors/err_object_creation.h"
#include "src/shared/common/errors/err_osd_data.h"
#include "src/shared/utils/utils.h"

#ifdef USE_LOG4QT
#include <log4qt/logger.h>
LOG4QT_DECLARE_STATIC_LOGGER(logger, OSDStreamGyro)
#else
#include <QDebug>
#endif

OSDStreamGyro* OSDStreamGyro::gyroStream = nullptr;

OSDStreamGyro::OSDStreamGyro(
        TcpMessagingOpts *config,
        OSDInertiaRepository *repoInertia,
        OSDCMSInputMode *modeService
        )
    :cfg(config), _repoInertia(repoInertia), serviceMode(modeService), currentErr(NoError())
{
    consumer = new TcpMessagingWrapper(this, config);
    connect(consumer, &TcpMessagingWrapper::signalForwardMessage, this, &OSDStreamGyro::onDataReceived);
}

OSDStreamGyro *OSDStreamGyro::getInstance(
        TcpMessagingOpts *config,
        OSDInertiaRepository *repoInertia,
        OSDCMSInputMode *modeService
        )
{
    if (gyroStream == nullptr) {
        if(config == nullptr) {
            throw ErrObjectCreation();
        }

        if(repoInertia == nullptr) {
            throw ErrObjectCreation();
        }

        if(modeService == nullptr) {
            throw ErrObjectCreation();
        }

        gyroStream = new OSDStreamGyro(config, repoInertia, modeService);
    }

    return gyroStream;

}

BaseError OSDStreamGyro::check()
{
    //TODO: check no data error, invalid data error, etc
    auto connError = consumer->checkConnection();
    if (connError.getCode() != 0) {
        currentErr = static_cast<BaseError>(connError);
        return currentErr;
    }

    return currentErr;
}

void OSDStreamGyro::onDataReceived(QByteArray data)
{
    try {
        QJsonObject respObj = Utils::byteArrayToJsonObject(data);
        GyroModel model(respObj["heading"].toDouble(),respObj["pitch"].toDouble(),respObj["roll"].toDouble());

#ifdef USE_LOG4QT
        logger()->trace()<<Q_FUNC_INFO<<" -> Heading: "<<model.getHeading()
                        <<", pitch: "<<model.getPicth()
                       <<", roll: "<<model.getRoll()
                         ;
#else
        qDebug()<<Q_FUNC_INFO<<"data gyro: heading ->"<<model.getHeading()<<"pitch ->"<<model.getPicth()<<"roll ->"<<model.getRoll();
#endif

        //check source mode manual
        if (respObj.contains("source")) {
            if (respObj["source"].toString().contains("manual")) {
                return;
            }
        }


        auto inertiaMode = serviceMode->getDataMode()->inersia();
        if (!inertiaMode) {
            if(check().getCode() == ERROR_NO.first || check().getCode() == ERROR_CODE_OSD_DATA_PARTIALLY_INVALID.first)
            {
                _repoInertia->SetInertia(OSDInertiaEntity(
                                             model.getHeading(),
                                             model.getPicth(),
                                             model.getRoll(),
                                             respObj["source"].toString().toStdString(),
                                         respObj["status"].toString().toStdString(),
                        OSD_MODE::AUTO
                        ));
            }
        }

        handleError(respObj["status"].toString());

        emit signalDataProcessed(model);
    } catch (ErrJsonParse &e) {
#ifdef USE_LOG4QT
        logger()->error()<<Q_FUNC_INFO<<" -> caught error: "<<e.getMessage();
#else
        qWarning()<<Q_FUNC_INFO<<"caught error: "<<e.getMessage();
#endif
    }  catch (...) {
#ifdef USE_LOG4QT
        logger()->error()<<Q_FUNC_INFO<<" -> caught unkbnown error";
#else
        qWarning()<<Q_FUNC_INFO<<"caught unkbnown error";
#endif
    }
}

void OSDStreamGyro::handleError(const QString &err)
{
    if (err.toStdString().empty()) {
        currentErr = NoError();
    } else if (err.contains("Partially")) {
        currentErr = ErrOSDDataPartiallyInvalid();
    } else if (err.contains("Range")) {
        currentErr = ErrOSDDataOutOfRange();
    } else {
        currentErr = ErrOSDDataInvalid();
    }
}
