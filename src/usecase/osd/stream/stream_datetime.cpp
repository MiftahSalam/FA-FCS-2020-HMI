#include "stream_datetime.h"
#include "src/shared/common/errors/err_json_parse.h"
#include "src/shared/common/errors/err_object_creation.h"
#include "src/shared/common/errors/err_osd_data.h"
#include "src/shared/utils/utils.h"

StreamDateTime* StreamDateTime::dateTimeStream = nullptr;

StreamDateTime::StreamDateTime(TcpMessagingOpts *config):
    cfg(config), currentErr(NoError())
{
    consumer = new TcpMessagingWrapper(this, config);
    connect(consumer, &TcpMessagingWrapper::signalForwardMessage, this, &StreamDateTime::onDataReceived);
}

void StreamDateTime::onDataReceived(QByteArray data)
{
    try {
        QJsonObject respObj = Utils::byteArrayToJsonObject(data);
        DateTimeModel model(respObj["date"].toDouble(),respObj["time"].toDouble());

        qDebug()<<Q_FUNC_INFO<<"date"<<model.getDate()<<"time"<<model.getTime();

        handleError(respObj["status"].toString());

        emit signalDataProcessed(model);
    }catch(ErrJsonParse &e) {
        qDebug()<<Q_FUNC_INFO<<"caught error: "<<e.getMessage();
    }  catch (...) {
        qDebug()<<Q_FUNC_INFO<<"caught unkbnown error";
    }
}

void StreamDateTime::handleError(const QString &err)
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

StreamDateTime *StreamDateTime::getInstance(TcpMessagingOpts *config)
{
    if (dateTimeStream == nullptr)
    {
        if(config == nullptr)
        {
            throw ErrObjectCreation();
        }

        dateTimeStream = new StreamDateTime(config);
    }
    return dateTimeStream;
}

BaseError StreamDateTime::check()
{
    auto connError = consumer->checkConnection();
    if (connError.getCode() != 0) {
        currentErr = static_cast<BaseError>(connError);
        return currentErr;
    }

    return currentErr;
}

