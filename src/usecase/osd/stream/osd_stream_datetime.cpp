#include "osd_stream_datetime.h"
#include "src/shared/common/errors/err_json_parse.h"
#include "src/shared/common/errors/err_object_creation.h"
#include "src/shared/common/errors/err_osd_data.h"
#include "src/shared/utils/utils.h"

OSDStreamDateTime* OSDStreamDateTime::dateTimeStream = nullptr;

OSDStreamDateTime::OSDStreamDateTime(TcpMessagingOpts *config, OSDDateTimeRepository *repoDateTime):
    cfg(config), _repoDateTime(repoDateTime), currentErr(NoError())
{
    consumer = new TcpMessagingWrapper(this, config);
    connect(consumer, &TcpMessagingWrapper::signalForwardMessage, this, &OSDStreamDateTime::onDataReceived);

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &OSDStreamDateTime::periodeUpdate);
    timer->start(1000);
}

void OSDStreamDateTime::onDataReceived(QByteArray data)
{
    try {
        //{"date_time_utc": "2009-01-19T03:27:50Z","date_time_local": "2009-01-18T17:00:50+07:00", "status": "", "source": "input_2"}
        QJsonObject respObj = Utils::byteArrayToJsonObject(data);
        DateTimeModel model(respObj["date_time_utc"].toString().toStdString(),respObj["date_time_local"].toString().toStdString());

        qDebug()<<Q_FUNC_INFO<<"time local"<<QString::fromStdString(model.getDateTimeLocal())
               <<"time UTC"<<QString::fromStdString(model.getDateTimeUTC());

        if(check().getCode() == ERROR_NO.first){
            _repoDateTime->SetDateTime(OSDDateTimeEntity(
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

void OSDStreamDateTime::periodeUpdate()
{
    check();

    QDateTime curTimeLocalEpoch = QDateTime::fromMSecsSinceEpoch(_repoDateTime->GetDateTime()->dateTimeLocalProcessed());
    QDateTime curTimeEpoch = QDateTime::fromMSecsSinceEpoch(_repoDateTime->GetDateTime()->dateTimeProcessed());

    _repoDateTime->UpdateTimeDisplay(curTimeEpoch.addSecs(1).toMSecsSinceEpoch());
    _repoDateTime->UpdateTimeLocalDisplay(curTimeLocalEpoch.addSecs(1).toMSecsSinceEpoch());
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

OSDStreamDateTime *OSDStreamDateTime::getInstance(TcpMessagingOpts *config, OSDDateTimeRepository *repoDateTime)
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

        dateTimeStream = new OSDStreamDateTime(config, repoDateTime);
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

