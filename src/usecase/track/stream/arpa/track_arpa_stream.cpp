#include "track_arpa_stream.h"
#include "src/shared/common/errors/err_json_parse.h"
#include "src/shared/common/errors/err_object_creation.h"
#include "src/shared/common/errors/err_osd_data.h"
#include "src/shared/utils/utils.h"

TrackArpaStream* TrackArpaStream::arpaStream = nullptr;

TrackArpaStream::TrackArpaStream(
    TcpMessagingOpts *config,
    TrackBaseRepository *repoArpa)
    : cfg(config), _repoArpa(repoArpa), currentErr(NoError())

{
    consumer = new TcpMessagingWrapper(this, config);
    connect(consumer, &TcpMessagingWrapper::signalForwardMessage, this, &TrackArpaStream::onDataReceived);
}

TrackArpaStream *TrackArpaStream::getInstance(
    TcpMessagingOpts *config = nullptr,
    TrackBaseRepository* repoArpa = nullptr)
{
    if (arpaStream == nullptr) {
        if(config == nullptr) {
            throw ErrObjectCreation();
        }

        if(repoArpa == nullptr) {
            throw ErrObjectCreation();
        }

        arpaStream = new TrackArpaStream(config, repoArpa);
    }

    return arpaStream;
}

BaseError TrackArpaStream::check()
{
    //TODO: check no data error, invalid data error, etc
    auto connError = consumer->checkConnection();
    if (connError.getCode() != 0) {
        currentErr = static_cast<BaseError>(connError);
        return currentErr;
    }

    return currentErr;
    // return consumer->checkConnection();
}

// TODO: implementation
void TrackArpaStream::onDataReceived(QByteArray data)
{
    try {
       QJsonObject respObj = Utils::byteArrayToJsonObject(data);
       TrackArpaModel model(respObj["source"].toString().toStdString(),
                            respObj["status"].toString().toStdString(),
                            respObj["id"].toInt(),
                            respObj["range"].toDouble(),
                            respObj["bearing"].toDouble(),
                            respObj["speed"].toDouble(),
                            respObj["course"].toDouble()
                            );
       //check source manual
       if(respObj.contains("source"))
       {
           if(respObj["source"].toString().contains("manual"))
           {
               return;
           }
       }

       _repoArpa->Update(TrackBaseEntity(
           model.getId(),
           model.getRange(),
           model.getBearing(),
           model.getSpeed(),
           model.getCourse(),
           respObj["source"].toString().toStdString(),
           respObj["status"].toString().toStdString(),
           respObj["time_stamp"].toInt()
           ));

       handleError(respObj["status"].toString());

       emit signalDataProcessed(model);
    } catch (ErrJsonParse &e) {
        qDebug()<<Q_FUNC_INFO<<"caught error: "<<e.getMessage();
    }  catch (...) {
        qDebug()<<Q_FUNC_INFO<<"caught unkbnown error";
    }
}

// TODO: implementation
void TrackArpaStream::periodUpdate()
{

}

void TrackArpaStream::handleError(const QString &err)
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
