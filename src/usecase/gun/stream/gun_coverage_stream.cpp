#include "gun_coverage_stream.h"
#include "src/shared/common/errors/err_json_parse.h"
#include "src/shared/common/errors/err_object_creation.h"
#include "src/shared/common/errors/err_osd_data.h"
#include "src/shared/utils/utils.h"

GunCoverageStream *GunCoverageStream::gunCoverageStream = nullptr;

GunCoverageStream::GunCoverageStream(
    TcpMessagingOpts *config,
    GunCoverageRepository *repoGunCov
    ): cfg(config), _repoGunCov(repoGunCov), currentErr(NoError())
{
    consumer = new TcpMessagingWrapper(this, config);
    connect(consumer, &TcpMessagingWrapper::signalForwardMessage, this, &GunCoverageStream::onDataReceived);
}

void GunCoverageStream::onDataReceived(QByteArray data)
{
    try {
        QJsonObject respObj = Utils::byteArrayToJsonObject(data);
        GunCoverageModel model(respObj["max_range"].toDouble(),respObj["blind_arc"].toDouble(),respObj["orientation"].toDouble());

        qDebug()<<Q_FUNC_INFO<<"data gun coverage: max range ->"<<model.getMaxRange()<<"blind arc ->"<<model.getBlindArc()
                 <<"orientation ->"<<model.getOrientation();

        _repoGunCov->SetGunCoverage(GunCoverageEntity(
            model.getMaxRange(),
            model.getBlindArc(),
            model.getOrientation()
            ));

        emit signalDataProcessed(model);

    }catch(ErrJsonParse &e) {
        qDebug()<<Q_FUNC_INFO<<"caught error: "<<e.getMessage();
    }  catch (...) {
        qDebug()<<Q_FUNC_INFO<<"caught unkbnown error";
    }
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



