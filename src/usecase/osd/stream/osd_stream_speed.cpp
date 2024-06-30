#include "osd_stream_speed.h"
#include "src/shared/common/errors/err_json_parse.h"
#include "src/shared/common/errors/err_object_creation.h"
#include "src/shared/utils/utils.h"

OSDStreamSpeed* OSDStreamSpeed::speedStream = nullptr;


OSDStreamSpeed::OSDStreamSpeed(TcpMessagingOpts *config): cfg(config)
{
    if(config == nullptr) {
        throw ErrObjectCreation();
    }

    consumer = new TcpMessagingWrapper(this, config);
    connect(consumer, &TcpMessagingWrapper::signalForwardMessage, this, &OSDStreamSpeed::onDataReceived);

}

OSDStreamSpeed *OSDStreamSpeed::getInstance(TcpMessagingOpts *config = nullptr)
{
    if (speedStream == nullptr) {
        if(config == nullptr) {
            throw ErrObjectCreation();
        }

        speedStream = new OSDStreamSpeed(config);
    }

    return speedStream;
}

BaseError OSDStreamSpeed::check()
{
    return consumer->checkConnection();
}

void OSDStreamSpeed::onDataReceived(QByteArray data)
{
    try {
        QJsonObject respObj = Utils::byteArrayToJsonObject(data);
        SpeedModel model(respObj["speed"].toDouble(),respObj["course"].toDouble());

        qDebug()<<Q_FUNC_INFO<<"data speed: sog ->"<<model.getSpeed_OG()<<"cog ->"<<model.getCourse_OG();

        //check source mode manual
        if (respObj.contains("source")) {
            if (respObj["source"].toString().contains("manual")) {
                return;
            }
        }

        emit signalDataProcessed(model);
    } catch (ErrJsonParse &e) {
        qDebug()<<Q_FUNC_INFO<<"caught error: "<<e.getMessage();
    }  catch (...) {
        qDebug()<<Q_FUNC_INFO<<"caught unkbnown error";
    }
}
