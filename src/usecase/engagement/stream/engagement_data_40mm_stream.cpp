#include "engagement_data_40mm_stream.h"
#include "src/shared/common/errors/err_json_parse.h"
#include "src/shared/common/errors/err_object_creation.h"
#include "src/shared/common/errors/err_osd_data.h"
#include "src/shared/utils/utils.h"

EngagementData40mmStream* EngagementData40mmStream::instance = nullptr;

EngagementData40mmStream::EngagementData40mmStream(
        TcpMessagingOpts *config,
        EngagementDataRepository *repoEngagemenData)
    : cfg(config), _repoEngagemenData(repoEngagemenData), currentErr(NoError())
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

void EngagementData40mmStream::onDataReceived(QByteArray data)
{
    try {
        QJsonObject respObj = Utils::byteArrayToJsonObject(data);
        EngagementDataModel model ("", respObj["azimuth"].toDouble(), respObj["elevation"].toDouble(), respObj["status"].toInt());

        qDebug()<<Q_FUNC_INFO<<"Engagement Data Azimuth"<<model.azimuth()<<"Elevation"<<model.elevation();

        _repoEngagemenData->AddEngagement(EngagementDataEntity(
            WeaponTrackAssignEntity ("",0),
            GunBarrelEntity (model.azimuth(), model.elevation()),
            EngagementDataEntity::EngagementStatus(model.getStatus())
            ));

        emit signalDataProcessed(model);

    }catch(ErrJsonParse &e) {
        qDebug()<<Q_FUNC_INFO<<"caught error: "<<e.getMessage();
    }  catch (...) {
        qDebug()<<Q_FUNC_INFO<<"caught unkbnown error";
    }
}

void EngagementData40mmStream::periodeUpdate()
{
    check();
    qDebug() << Q_FUNC_INFO;
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
