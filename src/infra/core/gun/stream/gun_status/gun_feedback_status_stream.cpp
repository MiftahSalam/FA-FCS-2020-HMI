#include "gun_feedback_status_stream.h"
#include "src/shared/common/errors/err_json_parse.h"
#include "src/shared/common/errors/err_object_creation.h"
#include "src/shared/utils/utils.h"

#ifdef USE_LOG4QT
#include <log4qt/logger.h>
LOG4QT_DECLARE_STATIC_LOGGER(logger, GunFeedbackStatusStream)
#else
#include <QDebug>
#endif

GunFeedbackStatusStream *GunFeedbackStatusStream::gunFeedbackStream = nullptr;

GunFeedbackStatusStream::GunFeedbackStatusStream(
    TcpMessagingOpts *config): cfg(config), currentErr(NoError())
{
    consumer = new TcpMessagingWrapper(this, config);
    connect(consumer, &TcpMessagingWrapper::signalForwardMessage, this, &GunFeedbackStatusStream::onDataReceived);
}

void GunFeedbackStatusStream::onDataReceived(QByteArray data)
{
    GunFeedbackStatusModel model;
    try {
        /*
         * {"op_mode":true,"remote":false,"mount":false,"btemp":false,"grtst":false,"grtfr":false,"fire_mode":false,"blarc":false,"misalgn":false,"magazine":false}
         */
        QJsonObject respObj = Utils::byteArrayToJsonObject(data);
        model = GunFeedbackStatusModel::fromJsonObject(respObj);

#ifdef USE_LOG4QT
        logger()->trace()<<Q_FUNC_INFO<<" -> Gun Stats Data."
                        <<"op mode: "<<model.getOpMode()
                       <<", remote: "<<model.getRemote()
                      <<", mount: "<<model.getMount()
                     <<", btemp: "<<model.getBarrelTemperature()
                    <<", grtst: "<<model.getGunReadyToStart()
                   <<", grtfr: "<<model.getGunReadyToFire()
                  <<", fire mode: "<<model.getFireMode()
                 <<", blarc: "<<model.getBlindArc()
                <<", misalrgn: "<<model.getMissAlignment()
               <<", mag: "<<model.getMagazine()
                 ;
#else
        qDebug()<<Q_FUNC_INFO<<"data gun status: op mode ->"<<model.getOpMode()
               <<"remote ->"<<model.getRemote()
              <<"mount ->"<<model.getMount()
             <<"btemp ->"<<model.getBarrelTemperature()
            <<"grtst ->"<<model.getGunReadyToStart()
           <<"grtfr ->"<<model.getGunReadyToFire()
          <<"fire mode ->"<<model.getFireMode()
         <<"blarc ->"<<model.getBlindArc()
        <<"misalrgn ->"<<model.getMissAlignment()
        <<"mag ->"<<model.getMagazine();
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

void GunFeedbackStatusStream::handleError(const QString &err)
{
    Q_UNUSED(err);
}

GunFeedbackStatusStream *GunFeedbackStatusStream::getInstance(
    TcpMessagingOpts *config = nullptr)
{
    if(gunFeedbackStream == nullptr)
    {
        if(config == nullptr)
        {
            throw ErrObjectCreation();
        }

        gunFeedbackStream = new GunFeedbackStatusStream(config);
    }

    return gunFeedbackStream;
}

BaseError GunFeedbackStatusStream::check()
{
    auto connError = consumer->checkConnection();
    if (connError.getCode() != 0) {
        currentErr = static_cast<BaseError>(connError);
        return currentErr;
    }
    currentErr = connError;

    return currentErr;
}
