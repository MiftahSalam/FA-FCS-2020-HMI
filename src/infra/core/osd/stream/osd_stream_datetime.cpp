#include "osd_stream_datetime.h"
#include "src/shared/common/errors/err_json_parse.h"
#include "src/shared/common/errors/err_object_creation.h"
#include "src/shared/common/errors/err_osd_data.h"
#include "src/shared/utils/utils.h"

#ifdef USE_LOG4QT
#include <log4qt/logger.h>
LOG4QT_DECLARE_STATIC_LOGGER(logger, OSDStreamDateTime)
#else
#include <QDebug>
#endif

OSDStreamDateTime* OSDStreamDateTime::dateTimeStream = nullptr;

OSDStreamDateTime::OSDStreamDateTime(TcpMessagingOpts *config
                                     ):
    cfg(config), currentErr(NoError())
{
    consumer = new TcpMessagingWrapper(this, config);
    connect(consumer, &TcpMessagingWrapper::signalForwardMessage, this, &OSDStreamDateTime::onDataReceived);
}

void OSDStreamDateTime::onDataReceived(QByteArray data)
{
    DateTimeStreamModel model;
    try {
        //{"date_time_utc": "2009-01-19T03:27:50Z","date_time_local": "2009-01-18T17:00:50+07:00", "status": "", "source": "input_2"}
        QJsonObject respObj = Utils::byteArrayToJsonObject(data);
        model = DateTimeStreamModel::fromJsonObject(respObj);

#ifdef USE_LOG4QT
        logger()->trace()<<Q_FUNC_INFO<<" -> time local: "<<model.getDateTimeLocal()
                        <<", time UTC: "<<model.getDateTimeUTC()
                         ;
#else
        qDebug()<<Q_FUNC_INFO<<"time local"<<QString::fromStdString(model.getDateTimeLocal())
               <<"time UTC"<<QString::fromStdString(model.getDateTimeUTC());
#endif

        handleError(respObj["status"].toString());

    }catch(ErrJsonParse &e) {
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

void OSDStreamDateTime::handleError(const QString &err)
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

OSDStreamDateTime *OSDStreamDateTime::getInstance(TcpMessagingOpts *config)
{
    if (dateTimeStream == nullptr)
    {
        if(config == nullptr)
        {
            throw ErrObjectCreation();
        }

        dateTimeStream = new OSDStreamDateTime(config);
    }
    return dateTimeStream;
}

BaseError OSDStreamDateTime::check()
{
    auto connError = consumer->checkConnection();
    if (connError.getCode() != 0) {
        currentErr = static_cast<BaseError>(connError);
        return currentErr;
    }

    return currentErr;
}

