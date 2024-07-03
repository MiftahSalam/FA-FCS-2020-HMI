#include "osd_stream_gyro.h"
#include "src/shared/common/errors/err_json_parse.h"
#include "src/shared/common/errors/err_object_creation.h"
#include "src/shared/utils/utils.h"

OSDStreamGyro* OSDStreamGyro::gyroStream = nullptr;

OSDStreamGyro::OSDStreamGyro(TcpMessagingOpts *config, OSDInertiaRepository *repoInertia)
    :cfg(config), _repoInertia(repoInertia)
{
    consumer = new TcpMessagingWrapper(this, config);
    connect(consumer, &TcpMessagingWrapper::signalForwardMessage, this, &OSDStreamGyro::onDataReceived);
}

OSDStreamGyro *OSDStreamGyro::getInstance(TcpMessagingOpts *config, OSDInertiaRepository *repoInertia)
{
    if (gyroStream == nullptr) {
        if(config == nullptr) {
            throw ErrObjectCreation();
        }

        if(repoInertia == nullptr) {
            throw ErrObjectCreation();
        }

        gyroStream = new OSDStreamGyro(config, repoInertia);
    }

    return gyroStream;

}

BaseError OSDStreamGyro::check()
{
    //TODO: check no data error, invalid data error, etc
    return consumer->checkConnection();
}

void OSDStreamGyro::onDataReceived(QByteArray data)
{
    try {
        QJsonObject respObj = Utils::byteArrayToJsonObject(data);
        GyroModel model(respObj["heading"].toDouble(),respObj["pitch"].toDouble(),respObj["roll"].toDouble());

        qDebug()<<Q_FUNC_INFO<<"data gyro: heading ->"<<model.getHeading()<<"pitch ->"<<model.getPicth()<<"roll ->"<<model.getRoll();

        //TODO: update repo
        _repoInertia->SetInertia(OSDInertiaEntity(
                                  model.getHeading(),
                                  model.getPicth(),
                                  model.getRoll(),
                                  respObj["source"].toString().toStdString(),
                                  respObj["status"].toString().toStdString(),
                                  OSD_MODE::AUTO
                              ));

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
