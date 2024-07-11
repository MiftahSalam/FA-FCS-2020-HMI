#include "osd_stream_speed.h"
#include "src/shared/common/errors/err_json_parse.h"
#include "src/shared/common/errors/err_object_creation.h"
#include "src/shared/utils/utils.h"

OSDStreamSpeed* OSDStreamSpeed::speedStream = nullptr;

OSDStreamSpeed::OSDStreamSpeed(
        TcpMessagingOpts *config,
        OSDSpeedRepository *repoSpeed,
        OSDCMSInputMode *modeService
        )
    :cfg(config), _repoSpeed(repoSpeed), serviceMode(modeService)
{
    consumer = new TcpMessagingWrapper(this, config);
    connect(consumer, &TcpMessagingWrapper::signalForwardMessage, this, &OSDStreamSpeed::onDataReceived);
}

OSDStreamSpeed *OSDStreamSpeed::getInstance(
        TcpMessagingOpts *config,
        OSDSpeedRepository *repoSpeed,
        OSDCMSInputMode *modeService
        )
{
    if (speedStream == nullptr) {
        if(config == nullptr) {
            throw ErrObjectCreation();
        }

        if(repoSpeed == nullptr) {
            throw ErrObjectCreation();
        }

        if(modeService == nullptr) {
            throw ErrObjectCreation();
        }

        speedStream = new OSDStreamSpeed(config, repoSpeed, modeService);
    }

    return speedStream;
}

BaseError OSDStreamSpeed::check()
{
    //TODO: check no data error, invalid data error, etc
    return consumer->checkConnection();
}

void OSDStreamSpeed::onDataReceived(QByteArray data)
{
    try {
        QJsonObject respObj = Utils::byteArrayToJsonObject(data);
        SpeedModel model(respObj["sog"].toDouble(),respObj["cog"].toDouble());

        qDebug()<<Q_FUNC_INFO<<"data speed: SOG ->"<<model.getSpeed()<<"COG ->"<<model.getCourse();

        //check source mode manual
        if (respObj.contains("source")) {
            if (respObj["source"].toString().contains("manual")) {
                return;
            }
        }

        auto speedMode = serviceMode->getDataMode().getSpeed();
        if (!speedMode) {
            //TODO: update repo
            _repoSpeed->SetSpeed(OSDSpeedEntity(
                                      model.getSpeed(),
                                      model.getCourse(),
                                      respObj["source"].toString().toStdString(),
                                      respObj["status"].toString().toStdString(),
                                      OSD_MODE::AUTO
                                  ));
        }

        emit signalDataProcessed(model);
    } catch (ErrJsonParse &e) {
        qDebug()<<Q_FUNC_INFO<<"caught error: "<<e.getMessage();
    }  catch (...) {
        qDebug()<<Q_FUNC_INFO<<"caught unkbnown error";
    }
}

