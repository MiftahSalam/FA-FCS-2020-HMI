#include "track_arpa_stream.h"
#include "src/shared/common/errors/err_json_parse.h"
#include "src/shared/common/errors/err_object_creation.h"

TrackArpaStream* TrackArpaStream::arpaStream = nullptr;

TrackArpaStream::TrackArpaStream(TcpMessagingOpts *config, TrackBaseRepository *repoArpa)
    : cfg(config), _repoArpa(repoArpa)

{
    consumer = new TcpMessagingWrapper(this, config);
    connect(consumer, &TcpMessagingWrapper::signalForwardMessage, this, &TrackArpaStream::onDataReceived);
}

TrackArpaStream *TrackArpaStream::getInstance(TcpMessagingOpts *config = nullptr, TrackBaseRepository* repoArpa = nullptr)
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
//    qDebug()<<Q_FUNC_INFO;
    //TODO: check no data error, invalid data error, etc
    return consumer->checkConnection();
}

// TODO: implementation
void TrackArpaStream::onDataReceived(QByteArray data)
{
    try {
//        QJsonObject respObj = Utils::byteArrayToJsonObject(data);
//        TrackArpaModel model();

//        qDebug()<<Q_FUNC_INFO<<"data position: lat ->"<<model.getLatitude()<<"lon ->"<<model.getLongitude();

//        emit signalDataProcessed(model);
    } catch (ErrJsonParse &e) {
        qDebug()<<Q_FUNC_INFO<<"caught error: "<<e.getMessage();
    }  catch (...) {
        qDebug()<<Q_FUNC_INFO<<"caught unkbnown error";
    }

    //TODO: update repo
    //    _repoPos->SetEntity(); //temp
}

// TODO: implementation
void TrackArpaStream::periodUpdate()
{

}
