#include "gun_coverage_stream.h"
#include "src/shared/common/errors/err_json_parse.h"
#include "src/shared/common/errors/err_object_creation.h"
#include "src/shared/utils/utils.h"

#ifdef USE_LOG4QT
#include <log4qt/logger.h>
LOG4QT_DECLARE_STATIC_LOGGER(logger, GunCoverageStream)
#else
#include <QDebug>
#endif

GunCoverageStream *GunCoverageStream::gunCoverageStream = nullptr;

GunCoverageStream::GunCoverageStream(
        TcpMessagingOpts *config,
        GunCoverageRepository *repoGunCov
        ): cfg(config), _repoGunCov(repoGunCov), currentErr(NoError())
{
    consumer = new TcpMessagingWrapper(this, config);
    connect(consumer, &TcpMessagingWrapper::signalForwardMessage, this, &GunCoverageStream::onDataReceived);

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &GunCoverageStream::periodUpdate);
    timer->start(1000);
}

void GunCoverageStream::onDataReceived(QByteArray data)
{
    try {
        QJsonObject respObj = Utils::byteArrayToJsonObject(data);
        GunCoverageModel model(respObj["max_range"].toDouble(),respObj["blind_arc"].toDouble(),respObj["orientation"].toDouble());

#ifdef USE_LOG4QT
        logger()->trace()<<Q_FUNC_INFO<<" -> Gun coverage. max range: "<<model.getMaxRange()
                        <<", blind arc: "<<model.getBlindArc()
                       <<", orientation: "<<model.getOrientation()
                         ;
#else
        qDebug()<<Q_FUNC_INFO<<"data gun coverage: max range ->"<<model.getMaxRange()<<"blind arc ->"<<model.getBlindArc()
               <<"orientation ->"<<model.getOrientation();
#endif

        _repoGunCov->SetGunCoverage(GunCoverageEntity(
                                        model.getMaxRange(),
                                        model.getBlindArc(),
                                        model.getOrientation()
                                        ));

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

void GunCoverageStream::periodUpdate()
{
    check();
}

void GunCoverageStream::handleError(const QString &err)
{

}

GunCoverageStream *GunCoverageStream::getInstance(
        TcpMessagingOpts *config = nullptr,
        GunCoverageRepository *repoGunCov = nullptr
        )
{
    if(gunCoverageStream == nullptr)
    {
        if(config == nullptr)
        {
            throw ErrObjectCreation();
        }

        if(repoGunCov == nullptr) {
            throw ErrObjectCreation();
        }

        gunCoverageStream = new GunCoverageStream(config, repoGunCov);
    }

    return gunCoverageStream;
}

BaseError GunCoverageStream::check()
{    
    auto connError = consumer->checkConnection();
    if (connError.getCode() != 0) {
        currentErr = static_cast<BaseError>(connError);
        return currentErr;
    }

    return currentErr;
}

const GunCoverageEntity *GunCoverageStream::GetCoverage() const
{
    return _repoGunCov->GetGunCoverage();
}



