#include "osd_stream_waterspeed.h"
#include "src/shared/common/errors/err_json_parse.h"
#include "src/shared/common/errors/err_object_creation.h"
#include "src/shared/common/errors/err_osd_data.h"
#include "src/shared/utils/utils.h"

#ifdef USE_LOG4QT
#include <log4qt/logger.h>
LOG4QT_DECLARE_STATIC_LOGGER(logger, OSDStreamWaterSpeed)
#else
#include <QDebug>
#endif

OSDStreamWaterSpeed* OSDStreamWaterSpeed::waterspeedStream = nullptr;

OSDStreamWaterSpeed::OSDStreamWaterSpeed(TcpMessagingOpts *config)
    : cfg(config), currentErr(NoError())
{
    consumer = new TcpMessagingWrapper(this, config);
    connect(consumer, &TcpMessagingWrapper::signalForwardMessage, this, &OSDStreamWaterSpeed::onDataReceived);
}

OSDStreamWaterSpeed *OSDStreamWaterSpeed::getInstance(
    TcpMessagingOpts *config = nullptr)
{
    if (waterspeedStream == nullptr) {
        if(config == nullptr) {
            throw ErrObjectCreation();
        }

        waterspeedStream = new OSDStreamWaterSpeed(config);
    }
    return waterspeedStream;
}

BaseError OSDStreamWaterSpeed::check()
{
    //    qDebug()<<Q_FUNC_INFO;
    //TODO: check no data error, invalid data error, etc
    auto connError = consumer->checkConnection();
    if (connError.getCode() != 0) {
        currentErr = static_cast<BaseError>(connError);
        return currentErr;
    }

    return currentErr;
}

void OSDStreamWaterSpeed::onDataReceived(QByteArray data)
{
    WaterSpeedStreamModel model;
    try {
        QJsonObject respObj = Utils::byteArrayToJsonObject(data);
        model = WaterSpeedStreamModel::fromJsonObject(respObj);

#ifdef USE_LOG4QT
        logger()->trace()<<Q_FUNC_INFO<<" -> speed: "<<model.getSpeed()
                        <<", course: "<<model.getCourse()
                          ;
#else
        qDebug()<<Q_FUNC_INFO<<"data Water Speed: water speed ->"<<model.getSpeed()<<"water course ->"<<model.getCourse();
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

void OSDStreamWaterSpeed::handleError(const QString &err)
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
