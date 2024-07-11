#include "osd_stream_waterspeed.h"
#include "src/shared/common/errors/err_json_parse.h"
#include "src/shared/common/errors/err_object_creation.h"
#include "src/shared/utils/utils.h"

OSDStreamWaterSpeed* OSDStreamWaterSpeed::waterspeedStream = nullptr;

OSDStreamWaterSpeed::OSDStreamWaterSpeed(TcpMessagingOpts *config,
        OSDWaterSpeedRepository *repoWP,
        OSDCMSInputMode *modeService)
    : cfg(config), _repoWP(repoWP), serviceMode(modeService)
{
    consumer = new TcpMessagingWrapper(this, config);
    connect(consumer, &TcpMessagingWrapper::signalForwardMessage, this, &OSDStreamWaterSpeed::onDataReceived);
}

OSDStreamWaterSpeed *OSDStreamWaterSpeed::getInstance(
        TcpMessagingOpts *config = nullptr,
        OSDWaterSpeedRepository* repoWP = nullptr,
        OSDCMSInputMode *modeService = nullptr
        )
{
    if (waterspeedStream == nullptr) {
        if(config == nullptr) {
            throw ErrObjectCreation();
        }
        if(repoWP == nullptr) {
            throw ErrObjectCreation();
        }
        if(modeService == nullptr) {
            throw ErrObjectCreation();
        }
        waterspeedStream = new OSDStreamWaterSpeed(config, repoWP, modeService);
    }
    return waterspeedStream;
}

BaseError OSDStreamWaterSpeed::check()
{
    //    qDebug()<<Q_FUNC_INFO;
    //TODO: check no data error, invalid data error, etc
    return consumer->checkConnection();
}

void OSDStreamWaterSpeed::onDataReceived(QByteArray data)
{
    try {
        QJsonObject respObj = Utils::byteArrayToJsonObject(data);
        WaterSpeedModel model(respObj["speed"].toDouble(),respObj["course"].toDouble());

        qDebug()<<Q_FUNC_INFO<<"data Water Speed: water speed ->"<<model.getW_Speed()<<"water course ->"<<model.getW_Course();

        //check source mode manual
        if (respObj.contains("source")) {
            if (respObj["source"].toString().contains("manual")) {
                return;
            }
        }

        auto waterspeedMode = serviceMode->getDataMode().getWaterSpeed();
        if (!waterspeedMode) {
            //TODO: update repo
            _repoWP->SetWaterSpeed(OSDWaterSpeedEntity(
                                      model.getW_Speed(),
                                      model.getW_Course(),
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
