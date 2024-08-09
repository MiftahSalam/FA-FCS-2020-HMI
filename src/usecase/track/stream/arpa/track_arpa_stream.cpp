#include "track_arpa_stream.h"
#include "src/shared/common/errors/err_json_parse.h"
#include "src/shared/common/errors/err_object_creation.h"
#include "src/shared/common/errors/err_osd_data.h"
#include "src/shared/utils/utils.h"

TrackArpaStream* TrackArpaStream::arpaStream = nullptr;

TrackArpaStream::TrackArpaStream(
        TcpMessagingOpts *config,
        ArpaConfig *confiArpa,
        TrackBaseRepository *repoArpa
        ): cfg(config), arpaConfig(confiArpa), _repoArpa(repoArpa), currentErr(NoError())
{
    consumer = new TcpMessagingWrapper(this, config);
    connect(consumer, &TcpMessagingWrapper::signalForwardMessage, this, &TrackArpaStream::onDataReceived);

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &TrackArpaStream::periodUpdate);

    timer->start(1000);
}

TrackArpaStream *TrackArpaStream::getInstance(
        TcpMessagingOpts *config = nullptr,
        ArpaConfig *confiArpa = nullptr,
        TrackBaseRepository* repoArpa = nullptr)
{
    if (arpaStream == nullptr) {
        if(config == nullptr) {
            throw ErrObjectCreation();
        }

        if(confiArpa == nullptr) {
            throw ErrObjectCreation();
        }

        if(repoArpa == nullptr) {
            throw ErrObjectCreation();
        }

        arpaStream = new TrackArpaStream(config, confiArpa, repoArpa);
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
}

// TODO: implementation
void TrackArpaStream::onDataReceived(QByteArray data)
{
    try {
        QJsonObject respObj = Utils::byteArrayToJsonObject(data);
        TrackArpaModel model = TrackArpaModel::fromJsonObject(respObj);

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
                              model.source(),
                              model.status(),
                              QDateTime::currentMSecsSinceEpoch()
                              ));

        handleError(respObj["status"].toString());
    } catch (ErrJsonParse &e) {
        qDebug()<<Q_FUNC_INFO<<"caught error: "<<e.getMessage();
    }  catch (...) {
        qDebug()<<Q_FUNC_INFO<<"caught unkbnown error";
    }
}

// TODO: implementation
void TrackArpaStream::periodUpdate()
{
    check();

    qDebug()<<Q_FUNC_INFO<<"track count"<<_repoArpa->FindAll().size();

    long long now = QDateTime::currentMSecsSinceEpoch();
    std::list<TrackBaseEntity*> allTracks = _repoArpa->FindAll();
    foreach (TrackBaseEntity* track, allTracks) {
        if (now - track->timeStamp() > arpaConfig->getStaleTimeout()) {
            qDebug()<<Q_FUNC_INFO<<"stale track track "<<track->Id();
            _repoArpa->Remove(track->Id());
        }
    }
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
