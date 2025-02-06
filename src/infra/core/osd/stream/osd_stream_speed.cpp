#include "osd_stream_speed.h"
#include "src/shared/common/errors/err_json_parse.h"
#include "src/shared/common/errors/err_object_creation.h"
#include "src/shared/common/errors/err_osd_data.h"
#include "src/shared/utils/utils.h"

#ifdef USE_LOG4QT
#include <log4qt/logger.h>
LOG4QT_DECLARE_STATIC_LOGGER(logger, OSDStreamSpeed)
#else
#include <QDebug>
#endif

OSDStreamSpeed* OSDStreamSpeed::speedStream = nullptr;

OSDStreamSpeed::OSDStreamSpeed(
        TcpMessagingOpts *config
        )
    :cfg(config), currentErr(NoError())
{
    consumer = new TcpMessagingWrapper(this, config);
    connect(consumer, &TcpMessagingWrapper::signalForwardMessage, this, &OSDStreamSpeed::onDataReceived);
}

OSDStreamSpeed *OSDStreamSpeed::getInstance(
    TcpMessagingOpts *config)
{
    if (speedStream == nullptr) {
        if(config == nullptr) {
            throw ErrObjectCreation();
        }

        speedStream = new OSDStreamSpeed(config);
    }

    return speedStream;
}

BaseError OSDStreamSpeed::check()
{
    //TODO: check no data error, invalid data error, etc
    auto connError = consumer->checkConnection();
    if (connError.getCode() != 0) {
        currentErr = static_cast<BaseError>(connError);
        return currentErr;
    }

    return currentErr;
}

void OSDStreamSpeed::onDataReceived(QByteArray data)
{
    SpeedStreamModel model;
    try {
        QJsonObject respObj = Utils::byteArrayToJsonObject(data);
        model = SpeedStreamModel::fromJsonObject(respObj);

#ifdef USE_LOG4QT
        logger()->trace()<<Q_FUNC_INFO<<" -> speed: "<<model.getSpeed()
                        <<", course: "<<model.getCourse()
                          ;
#else
        qDebug()<<Q_FUNC_INFO<<"data speed: SOG ->"<<model.getSpeed()<<"COG ->"<<model.getCourse();
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

void OSDStreamSpeed::handleError(const QString &err)
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

