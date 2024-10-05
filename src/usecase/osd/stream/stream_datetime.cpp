#include "stream_datetime.h"
#include "src/shared/common/errors/err_json_parse.h"
#include "src/shared/common/errors/err_object_creation.h"
#include "src/shared/common/errors/err_osd_data.h"
#include "src/shared/utils/utils.h"

StreamDateTime* StreamDateTime::dateTimeStream = nullptr;

StreamDateTime::StreamDateTime(TcpMessagingOpts *config, DateTimeRepository *repoDateTime):
    cfg(config), _repoDateTime(repoDateTime), currentErr(NoError())
{
    consumer = new TcpMessagingWrapper(this, config);
    connect(consumer, &TcpMessagingWrapper::signalForwardMessage, this, &StreamDateTime::onDataReceived);
}

void StreamDateTime::onDataReceived(QByteArray data)
{
    try {
        QJsonObject respObj = Utils::byteArrayToJsonObject(data);
        DateTimeModel model(respObj["date_time_utc"].toString().toStdString(),respObj["date_time_local"].toString().toStdString());

        qDebug()<<Q_FUNC_INFO<<"time local"<<model.getDateTimeLocal()<<"time UTC"<<model.getDateTimeUTC();

        if(check().getCode() == ERROR_NO.first){
            _repoDateTime->SetDateTime(DateTimeEntity(
                model.getDateTimeLocal(),
                model.getDateTimeUTC(),
                respObj["source"].toString().toStdString(),
                respObj["status"].toString().toStdString(),
                OSD_MODE::AUTO
                ));
        }

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

StreamDateTime *StreamDateTime::getInstance(TcpMessagingOpts *config, DateTimeRepository *repoDateTime)
{
    if (dateTimeStream == nullptr)
    {
        if(config == nullptr)
        {
            throw ErrObjectCreation();
        }

        if(repoDateTime == nullptr){
            throw ErrObjectCreation();
        }

        dateTimeStream = new StreamDateTime(config, repoDateTime);
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

