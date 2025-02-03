#include "osd_stream_position.h"
#include "src/shared/common/errors/err_json_parse.h"
#include "src/shared/common/errors/err_object_creation.h"
#include "src/shared/common/errors/err_osd_data.h"
#include "src/shared/utils/utils.h"

#ifdef USE_LOG4QT
#include <log4qt/logger.h>
LOG4QT_DECLARE_STATIC_LOGGER(logger, OSDStreamPosition)
#else
#include <QDebug>
#endif

OSDStreamPosition* OSDStreamPosition::positionStream = nullptr;

OSDStreamPosition::OSDStreamPosition(TcpMessagingOpts *config)
//OSDStreamPosition::OSDStreamPosition(AMQPConfig *config)
    : cfg(config), currentErr(NoError())
{
    /*
    AMQPOptions *opt = new AMQPOptions(
                config->getInstance("")->getServerAddress(),
                "fa-fcs-hmi:position",
                "position",
                "topic",
                "*.position",
                QStringList()
                );
    consumer = new AmqpConsumerWrapper(this, opt);
    connect(consumer, &AmqpConsumerWrapper::signalForwardMessage, this, &OSDStreamPosition::onDataReceived);
    consumer->Connect();
    */
    consumer = new TcpMessagingWrapper(this, config);
    connect(consumer, &TcpMessagingWrapper::signalForwardMessage, this, &OSDStreamPosition::onDataReceived);
}

OSDStreamPosition *OSDStreamPosition::getInstance(
    TcpMessagingOpts *config = nullptr)
//OSDStreamPosition *OSDStreamPosition::getInstance(AMQPConfig *config = nullptr)
{
    if (positionStream == nullptr) {
        if(config == nullptr) {
            throw ErrObjectCreation();
        }

        positionStream = new OSDStreamPosition(config);
    }

    return positionStream;
}

BaseError OSDStreamPosition::check()
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

void OSDStreamPosition::onDataReceived(QByteArray data)
{
    PositionStreamModel model;
    try {
        QJsonObject respObj = Utils::byteArrayToJsonObject(data);
        model = PositionStreamModel::fromJsonObject(respObj);

#ifdef USE_LOG4QT
        logger()->trace()<<Q_FUNC_INFO<<" -> latitude: "<<model.getLatitude()
                        <<", longitude: "<<model.getLongitude()
                          ;
#else
        qDebug()<<Q_FUNC_INFO<<"data position: lat ->"<<model.getLatitude()<<"lon ->"<<model.getLongitude();
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

void OSDStreamPosition::handleError(const QString &err)
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
