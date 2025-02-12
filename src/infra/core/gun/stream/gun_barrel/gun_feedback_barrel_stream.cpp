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
    TcpMessagingOpts *config): cfg(config), currentErr(NoError())
{
    consumer = new TcpMessagingWrapper(this, config);
    connect(consumer, &TcpMessagingWrapper::signalForwardMessage, this, &GunFeedbackBarrelStream::onDataReceived);
}

void GunFeedbackBarrelStream::onDataReceived(QByteArray data)
{
    GunFeedbackBarrelModel model;
    try {
        QJsonObject respObj = Utils::byteArrayToJsonObject(data);
        model = GunFeedbackBarrelModel::fromJsonObject(respObj);

#ifdef USE_LOG4QT
        logger()->trace()<<Q_FUNC_INFO<<" -> Gun barrel."
                        <<" Azimuth: "<<model.getAzimuth()
                       <<", Elevation: "<<model.getElevation()
                         ;
#else
        qDebug()<<Q_FUNC_INFO<<"Gun Barrel Data. Azimuth"<<model.getAzimuth()<<"Elevation"<<model.getElevation();
#endif

        currentErr = NoError();

        emit signalDataProcessed(model);
    }catch(ErrJsonParse &e) {
#ifdef USE_LOG4QT
        logger()->error()<<Q_FUNC_INFO<<" -> caught error: "<<e.getMessage();
#else
        qWarning()<<Q_FUNC_INFO<<"caught error: "<<e.getMessage();
#endif
        currentErr = e;
    }  catch (...) {
#ifdef USE_LOG4QT
        logger()->error()<<Q_FUNC_INFO<<" -> caught unkbnown error";
#else
        qWarning()<<Q_FUNC_INFO<<"caught unkbnown error";
#endif
        currentErr = ErrUnknown();
    }
}

void GunFeedbackBarrelStream::handleError(const QString &err)
{
    Q_UNUSED(err);
}

GunFeedbackBarrelStream *GunFeedbackBarrelStream::getInstance(
    TcpMessagingOpts *config = nullptr)
{
    if(gunBarrelStream == nullptr)
    {
        if(config == nullptr)
        {
            throw ErrObjectCreation();
        }

        gunBarrelStream = new GunFeedbackBarrelStream(config);
    }

    return gunBarrelStream;
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

