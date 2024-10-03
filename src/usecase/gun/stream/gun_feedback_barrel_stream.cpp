#include "gun_feedback_barrel_stream.h"
#include "src/shared/common/errors/err_json_parse.h"
#include "src/shared/common/errors/err_object_creation.h"
#include "src/shared/utils/utils.h"

#ifdef USE_LOG4QT
#include <log4qt/logger.h>
LOG4QT_DECLARE_STATIC_LOGGER(logger, GunFeedbackBarrelStream)
#else
#include <QDebug>
#endif

GunFeedbackBarrelStream *GunFeedbackBarrelStream::gunBarrelStream = nullptr;

GunFeedbackBarrelStream::GunFeedbackBarrelStream(
        TcpMessagingOpts *config,
        GunFeedbackRepository *repoGunFback
        ): cfg(config), repoGunFback(repoGunFback), currentErr(NoError())
{
    consumer = new TcpMessagingWrapper(this, config);
    connect(consumer, &TcpMessagingWrapper::signalForwardMessage, this, &GunFeedbackBarrelStream::onDataReceived);

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &GunFeedbackBarrelStream::periodUpdate);
    timer->start(1000);
}

void GunFeedbackBarrelStream::onDataReceived(QByteArray data)
{
    try {
        QJsonObject respObj = Utils::byteArrayToJsonObject(data);
        GunFeedbackBarrelModel model(respObj["azimuth"].toDouble(),
                respObj["elevation"].toDouble()
                );

#ifdef USE_LOG4QT
        logger()->trace()<<Q_FUNC_INFO<<" -> Gun barrel."
                        <<" Azimuth: "<<model.getAzimuth()
                       <<", Elevation: "<<model.getElevation()
                         ;
#else
        qDebug()<<Q_FUNC_INFO<<"Gun Barrel Data. Azimuth"<<model.getAzimuth()<<"Elevation"<<model.getElevation();
#endif

        repoGunFback->SetBarrel(
                    model.getAzimuth(),
                    model.getElevation()
                    );

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

void GunFeedbackBarrelStream::periodUpdate()
{
    check();
}

void GunFeedbackBarrelStream::handleError(const QString &err)
{

}

GunFeedbackBarrelStream *GunFeedbackBarrelStream::getInstance(
        TcpMessagingOpts *config = nullptr,
        GunFeedbackRepository *repoGunFback = nullptr
        )
{
    if(gunBarrelStream == nullptr)
    {
        if(config == nullptr)
        {
            throw ErrObjectCreation();
        }

        if(repoGunFback == nullptr) {
            throw ErrObjectCreation();
        }

        gunBarrelStream = new GunFeedbackBarrelStream(config, repoGunFback);
    }

    return gunBarrelStream;
}

void GunFeedbackBarrelStream::resetBarrel()
{
    repoGunFback->SetBarrel(0.,0.);
}

BaseError GunFeedbackBarrelStream::check()
{
    auto connError = consumer->checkConnection();
    if (connError.getCode() != 0) {
        currentErr = static_cast<BaseError>(connError);
        return currentErr;
    }
    currentErr = connError;

    return currentErr;
}

