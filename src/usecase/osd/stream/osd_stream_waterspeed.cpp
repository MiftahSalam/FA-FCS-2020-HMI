#include "osd_stream_waterspeed.h"
#include "src/shared/common/errors/err_json_parse.h"
#include "src/shared/common/errors/err_object_creation.h"
#include "src/shared/common/errors/err_osd_data.h"
#include "src/shared/utils/utils.h"

#ifdef USE_LOG4QT
#include <log4qt/logger.h>
LOG4QT_DECLARE_STATIC_LOGGER(logger, OSDStreamWaterSpeed)
#else
#include <QDebug>
#endif

OSDStreamWaterSpeed* OSDStreamWaterSpeed::waterspeedStream = nullptr;

OSDStreamWaterSpeed::OSDStreamWaterSpeed(TcpMessagingOpts *config,
                                         OSDWaterSpeedRepository *repoWP,
                                         OSDCMSInputMode *modeService)
    : cfg(config), _repoWP(repoWP), serviceMode(modeService), currentErr(NoError())
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
    auto connError = consumer->checkConnection();
    if (connError.getCode() != 0) {
        currentErr = static_cast<BaseError>(connError);
        return currentErr;
    }

    return currentErr;
}

void OSDStreamWaterSpeed::onDataReceived(QByteArray data)
{
    try {
        QJsonObject respObj = Utils::byteArrayToJsonObject(data);
        WaterSpeedModel model(respObj["speed"].toDouble(),respObj["course"].toDouble());

#ifdef USE_LOG4QT
        logger()->trace()<<Q_FUNC_INFO<<" -> speed: "<<model.getSpeed()
                        <<", course: "<<model.getCourse()
                          ;
#else
        qDebug()<<Q_FUNC_INFO<<"data Water Speed: water speed ->"<<model.getSpeed()<<"water course ->"<<model.getCourse();
#endif

        //check source mode manual
        if (respObj.contains("source")) {
            if (respObj["source"].toString().contains("manual")) {
                return;
            }
        }

        auto waterspeedMode = serviceMode->getDataMode().getWaterSpeed();
        if (!waterspeedMode && check().getCode() == ERROR_NO.first) {
            _repoWP->SetWaterSpeed(OSDWaterSpeedEntity(
                                       model.getSpeed(),
                                       model.getCourse(),
                                       respObj["source"].toString().toStdString(),
                                   respObj["status"].toString().toStdString(),
                    OSD_MODE::AUTO
                    ));
        }

        handleError(respObj["status"].toString());

        emit signalDataProcessed(model);
    } catch (ErrJsonParse &e) {
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

void OSDStreamWaterSpeed::handleError(const QString &err)
{
    if (err.toStdString().empty()) {
        currentErr = NoError();
    } else if (err.contains("Partially")) {
        currentErr = ErrOSDDataPartiallyInvalid();
    } else if (err.contains("Range")) {
        currentErr = ErrOSDDataOutOfRange();
    } else {
        currentErr = ErrOSDDataInvalid();
    }
}
