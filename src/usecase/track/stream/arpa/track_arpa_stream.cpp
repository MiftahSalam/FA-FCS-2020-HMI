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

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &TrackArpaStream::periodUpdate);

    timer->start(1000);

    // delay for listener to ready
    QTimer::singleShot(10000, this, [this]() {
        connect(consumer, &TcpMessagingWrapper::signalForwardMessage, this, &TrackArpaStream::onDataReceived);
    });
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

void TrackArpaStream::onDataReceived(QByteArray data)
{
    try {
        QJsonArray trackJsonArray = enhanceJsonParse(data);
        if (!trackJsonArray.isEmpty()) {
            for (int var = 0; var < trackJsonArray.size(); var++) {
                QJsonObject respObj = trackJsonArray.at(var).toObject();
                TrackArpaModel model = TrackArpaModel::fromJsonObject(respObj);

                //check source manual
                if(respObj.contains("source"))
                {
                    if(respObj["source"].toString().contains("manual"))
                    {
                        return;
                    }
                }

                if (model.getId() <= 0) {
                    qDebug()<<Q_FUNC_INFO<<"invalid track";
                    return;
                }

                TrackBaseEntity updateTrack(
                            model.getId(),
                            model.getRange(),
                            model.getBearing(),
                            model.getSpeed(),
                            model.getCourse(),
                            model.source(),
                            model.status(),
                            QDateTime::currentMSecsSinceEpoch()
                            );

                const TrackBaseEntity* findTrack = _repoArpa->FindOne(model.getId());
                if (findTrack) {
                    updateTrack.setCurrIdentity(findTrack->getCurrIdentity());
                    updateTrack.setCurrEnv(findTrack->getCurrEnv());
                    updateTrack.setCurrSource(findTrack->getCurrSource());
                }
                _repoArpa->Update(updateTrack);

                handleError(respObj["status"].toString());
            }
        } else {
            qDebug()<<Q_FUNC_INFO<<"empty track array parser";
        }
    } catch (ErrJsonParse &e) {
        qDebug()<<Q_FUNC_INFO<<"caught error: "<<e.getMessage();
    }  catch (...) {
        qDebug()<<Q_FUNC_INFO<<"caught unkbnown error";
    }
}

void TrackArpaStream::periodUpdate()
{
    check();

    qDebug()<<Q_FUNC_INFO<<"track count"<<_repoArpa->FindAll().size();

    long long now = QDateTime::currentMSecsSinceEpoch();
    std::list<TrackBaseEntity*> allTracks = _repoArpa->FindAll();
    foreach (TrackBaseEntity* track, allTracks) {
        if (now - track->getTimeStamp() > arpaConfig->getStaleTimeout()) {
            qDebug()<<Q_FUNC_INFO<<"stale track track "<<track->getId();
            _repoArpa->Remove(track->getId());
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

QJsonArray TrackArpaStream::enhanceJsonParse(const QByteArray data)
{
    QJsonArray result;
    QByteArray bufData = data;
    int idxHeadJson = data.indexOf('{');
    int idxTailJson = data.indexOf('}');
    QByteArray bufProcessedData = bufData.mid(idxHeadJson, idxTailJson - idxHeadJson + 1);

    while (bufData.size() > 0 && idxHeadJson >= 0 && idxTailJson > 0)
    {
        try {
            qDebug()<<Q_FUNC_INFO<<"bufProcessedData"<<bufProcessedData;
            QJsonObject respObj = Utils::byteArrayToJsonObject(bufProcessedData);
            result.append(respObj);
        } catch (...) {
            throw ErrJsonParse();
        }

        bufData.remove(idxHeadJson, idxTailJson - idxHeadJson + 1);

        qDebug()<<Q_FUNC_INFO<<"bufData"<<bufData;

        idxHeadJson = bufData.indexOf('{');
        idxTailJson = bufData.indexOf('}');
        bufProcessedData = bufData.mid(idxHeadJson, idxTailJson - idxHeadJson + 1);

    }

    return result;
}
