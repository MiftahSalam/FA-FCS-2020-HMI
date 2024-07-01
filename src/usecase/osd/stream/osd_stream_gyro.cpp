#include "osd_stream_gyro.h"
#include "src/shared/common/errors/err_json_parse.h"
#include "src/shared/common/errors/err_object_creation.h"
#include "src/shared/utils/utils.h"

OSDStreamGyro* OSDStreamGyro::gyroStream = nullptr;

OSDStreamGyro::OSDStreamGyro(TcpMessagingOpts *config)
    :cfg(config)
{
    if(config == nullptr) {
        throw ErrObjectCreation();
    }

    consumer = new TcpMessagingWrapper(this, config);
    connect(consumer, &TcpMessagingWrapper::signalForwardMessage, this, &OSDStreamGyro::onDataReceived);
}

OSDStreamGyro *OSDStreamGyro::getInstance(TcpMessagingOpts *config)
{
    if (gyroStream == nullptr) {
        if(config == nullptr) {
            throw ErrObjectCreation();
        }

        gyroStream = new OSDStreamGyro(config);
    }

    return gyroStream;

}

BaseError OSDStreamGyro::check()
{
    return consumer->checkConnection();
}

void OSDStreamGyro::onDataReceived(QByteArray data)
{
    try {
        QJsonObject respObj = Utils::byteArrayToJsonObject(data);
        GyroModel model(respObj["heading"].toDouble(),respObj["pitch"].toDouble(),respObj["roll"].toDouble());

        qDebug()<<Q_FUNC_INFO<<"data gyro: heading ->"<<model.getHeading()<<"pitch ->"<<model.getPicth()<<"roll ->"<<model.getRoll();

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
