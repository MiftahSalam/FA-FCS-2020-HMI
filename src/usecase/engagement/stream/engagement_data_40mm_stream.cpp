#include "engagement_data_40mm_stream.h"
#include "src/shared/common/errors/err_json_parse.h"
#include "src/shared/common/errors/err_object_creation.h"
#include "src/shared/utils/utils.h"

#ifdef USE_LOG4QT
#include <log4qt/logger.h>
LOG4QT_DECLARE_STATIC_LOGGER(logger, EngagementData40mmStream)
#else
#include <QDebug>
#endif

EngagementData40mmStream* EngagementData40mmStream::instance = nullptr;

EngagementData40mmStream::EngagementData40mmStream(
        TcpMessagingOpts *config,
        EngagementDataRepository *repoEngagemenData)
    : cfg(config), _repoEngagemenData(repoEngagemenData), currentErr(NoError()), currentTrack(-1)
{
    consumer = new TcpMessagingWrapper(this, config);
    connect(consumer, &TcpMessagingWrapper::signalForwardMessage, this, &EngagementData40mmStream::onDataReceived);

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &EngagementData40mmStream::periodeUpdate);

    timer->start(1000);
}

EngagementData40mmStream *EngagementData40mmStream::getInstance(
        TcpMessagingOpts *config = nullptr,
        EngagementDataRepository* repoEngagemenData = nullptr
        )
{
    if (instance == nullptr) {
        if(config == nullptr) {
            throw ErrObjectCreation();
        }

        if(repoEngagemenData == nullptr) {
            throw ErrObjectCreation();
        }

        instance = new EngagementData40mmStream(config, repoEngagemenData);
    }

    return instance;
}

void EngagementData40mmStream::CreateEngage(const int &trackId)
{
    if (_repoEngagemenData->AddEngagement(EngagementDataEntity(
                                              WeaponTrackAssignEntity ("40mm", trackId),
                                              GunBarrelEntity (0, 0),
                                              EngagementDataEntity::EngagementStatus()
                                              ))) {
        currentTrack = trackId;
    }

}

void EngagementData40mmStream::DeleteEngage(const QString weapon)
{
    if(_repoEngagemenData->RemoveEngagement(WeaponTrackAssignEntity (weapon.toStdString(), currentTrack))) {
        currentTrack = -1;
    }
}

void EngagementData40mmStream::onDataReceived(QByteArray data)
{
    try {
        QJsonObject respObj = Utils::byteArrayToJsonObject(data);
        EngagementDataModel model ("40mm", respObj["azimuth"].toDouble(), respObj["elevation"].toDouble(), respObj["status"].toInt());

#ifdef USE_LOG4QT
        logger()->trace()<<Q_FUNC_INFO<<" -> Engagement Data. Azimuth: "<<model.azimuth()<<", Elevation: "<<model.elevation();
#else
        qDebug()<<Q_FUNC_INFO<<"Engagement Data Azimuth"<<model.azimuth()<<"Elevation"<<model.elevation();
#endif

        _repoEngagemenData->UpdateEngagementBarrel(EngagementDataEntity(
            WeaponTrackAssignEntity ("40mm", currentTrack),
            GunBarrelEntity (model.azimuth(), model.elevation()),
            EngagementDataEntity::EngagementStatus(model.getStatus())
            ));

        currentErr = NoError();

        emit signalDataProcessed(model);

    }catch(ErrJsonParse &e) {
#ifdef USE_LOG4QT
        logger()->error()<<Q_FUNC_INFO<<" -> caught error: "<<e.getMessage();
#else
        qWarning()<<Q_FUNC_INFO<<"caught error: "<<e.getMessage();
#endif
    }  catch (...) {
#ifdef USE_LOG4QT
        logger()->error()<<Q_FUNC_INFO<<" -> caught unkbnown error";
#else
        qWarning()<<Q_FUNC_INFO<<"caught unkbnown error";
#endif
    }
}

void EngagementData40mmStream::periodeUpdate()
{
    check();
#ifdef USE_LOG4QT
        logger()->trace()<<Q_FUNC_INFO;
#else
    qDebug()<<Q_FUNC_INFO;
#endif
}

BaseError EngagementData40mmStream::check()
{
    auto connError = consumer->checkConnection();
    if (connError.getCode() != 0) {
        currentErr = static_cast<BaseError>(connError);
        return currentErr;
    }

    return currentErr;
}
