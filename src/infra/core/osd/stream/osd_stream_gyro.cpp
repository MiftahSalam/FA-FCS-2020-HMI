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
        TcpMessagingOpts *config
        )
    :cfg(config), currentErr(NoError())
{
    consumer = new TcpMessagingWrapper(this, config);
    connect(consumer, &TcpMessagingWrapper::signalForwardMessage, this, &OSDStreamGyro::onDataReceived);
}

OSDStreamGyro *OSDStreamGyro::getInstance(
    TcpMessagingOpts *config)
{
    if (gyroStream == nullptr) {
        if(config == nullptr) {
            throw ErrObjectCreation();
        }

        gyroStream = new OSDStreamGyro(config);
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
    GyroStreamModel model;
    try {
        QJsonObject respObj = Utils::byteArrayToJsonObject(data);
        model = GyroStreamModel::fromJsonObject(respObj);

#ifdef USE_LOG4QT
        logger()->trace()<<Q_FUNC_INFO<<" -> Heading: "<<model.getHeading()
                        <<", pitch: "<<model.getPitch()
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

        handleError(respObj["status"].toString());
    } catch (ErrJsonParse &e) {
#ifdef USE_LOG4QT
        logger()->error()<<Q_FUNC_INFO<<" -> caught error: "<<e.getMessage();
#else
        qWarning()<<Q_FUNC_INFO<<"caught error: "<<e.getMessage();
#endif

        model.setErr(e);
        model.setMode(OSD_MODE::AUTO);
    }  catch (...) {
#ifdef USE_LOG4QT
        logger()->error()<<Q_FUNC_INFO<<" -> caught unkbnown error";
#else
        qWarning()<<Q_FUNC_INFO<<"caught unkbnown error";
#endif
    }

    emit signalDataProcessed(model);
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
