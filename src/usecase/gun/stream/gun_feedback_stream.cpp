#include "gun_feedback_stream.h"
#include "src/shared/common/errors/err_json_parse.h"
#include "src/shared/common/errors/err_object_creation.h"
#include "src/shared/common/errors/err_osd_data.h"
#include "src/shared/utils/utils.h"

GunFeedbackStream *GunFeedbackStream::gunFeedbackStream = nullptr;

GunFeedbackStream::GunFeedbackStream(
    TcpMessagingOpts *config,
    GunFeedbackRepository *repoGunFback
    ): cfg(config), repoGunFback(repoGunFback), currentErr(NoError())
{
    consumer = new TcpMessagingWrapper(this, config);
    connect(consumer, &TcpMessagingWrapper::signalForwardMessage, this, &GunFeedbackStream::onDataReceived);

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &GunFeedbackStream::periodUpdate);
    timer->start(1000);
}

void GunFeedbackStream::onDataReceived(QByteArray data)
{
    try {
        QJsonObject respObj = Utils::byteArrayToJsonObject(data);
        GunFeedbackModel model(respObj["op_mode"].toBool(),
                               respObj["remote"].toBool(),
                               respObj["mount"].toBool(),
                               respObj["btemp"].toBool(),
                               respObj["grtst"].toBool(),
                               respObj["grtfr"].toBool(),
                               respObj["fire_mode"].toBool(),
                               respObj["blarc"].toBool(),
                               respObj["misalgn"].toBool(),
                               respObj["magazine"].toBool()
                               );

        qDebug()<<Q_FUNC_INFO<<"data gun coverage: op mode ->"<<model.getOpMode()
                 <<"remote ->"<<model.getRemote()
                 <<"mount ->"<<model.getMount()
                 <<"btemp ->"<<model.getBarrelTemperature()
                 <<"grtst ->"<<model.getGunReadyToStart()
                 <<"grtfr ->"<<model.getGunReadyToFire()
                 <<"fire mode ->"<<model.getFireMode()
                 <<"blarc ->"<<model.getBlindArc()
                 <<"misalrgn ->"<<model.getMissAlignment()
                 <<"mag ->"<<model.getMagazine();

        repoGunFback->SetStatus(GunStatusFeedbackEntity(
            model.getOpMode(),
            model.getRemote(),
            model.getMount(),
            model.getBarrelTemperature(),
            model.getGunReadyToStart(),
            model.getGunReadyToFire(),
            model.getFireMode(),
            model.getBlindArc(),
            model.getMissAlignment(),
            model.getMagazine()
            ));

        emit signalDataProcessed(model);

    }catch(ErrJsonParse &e) {
        qDebug()<<Q_FUNC_INFO<<"caught error: "<<e.getMessage();
    }  catch (...) {
        qDebug()<<Q_FUNC_INFO<<"caught unkbnown error";
    }
}

void GunFeedbackStream::periodUpdate()
{
    check();
    qDebug() << Q_FUNC_INFO;
}

void GunFeedbackStream::handleError(const QString &err)
{

}

GunFeedbackStream *GunFeedbackStream::getInstance(
    TcpMessagingOpts *config = nullptr,
    GunFeedbackRepository *repoGunFback = nullptr
    )
{
    if(gunFeedbackStream == nullptr)
    {
        if(config == nullptr)
        {
            throw ErrObjectCreation();
        }

        if(repoGunFback == nullptr) {
            throw ErrObjectCreation();
        }

        gunFeedbackStream = new GunFeedbackStream(config, repoGunFback);
    }

    return gunFeedbackStream;
}

BaseError GunFeedbackStream::check()
{
    auto connError = consumer->checkConnection();
    if (connError.getCode() != 0) {
        currentErr = static_cast<BaseError>(connError);
        return currentErr;
    }

    return currentErr;
}
