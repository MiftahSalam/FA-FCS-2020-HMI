#include "osd_stream_wind.h"
#include "src/shared/common/errors/err_json_parse.h"
#include "src/shared/common/errors/err_object_creation.h"
#include "src/shared/common/errors/err_osd_data.h"
#include "src/shared/utils/utils.h"

#ifdef USE_LOG4QT
#include <log4qt/logger.h>
LOG4QT_DECLARE_STATIC_LOGGER(logger, OSDStreamWind)
#else
#include <QDebug>
#endif

OSDStreamWind* OSDStreamWind::windStream = nullptr;

OSDStreamWind::OSDStreamWind(TcpMessagingOpts *config,
        OSDWindRepository *repoWind,
        OSDCMSInputMode *modeService)
    : cfg(config), _repoWind(repoWind), serviceMode(modeService), currentErr(NoError())
{
    consumer = new TcpMessagingWrapper(this, config);
    connect(consumer, &TcpMessagingWrapper::signalForwardMessage, this, &OSDStreamWind::onDataReceived);
}

OSDStreamWind *OSDStreamWind::getInstance(
        TcpMessagingOpts *config = nullptr,
        OSDWindRepository* repoWind = nullptr,
        OSDCMSInputMode *modeService = nullptr
        )
{
    if (windStream == nullptr) {
        if(config == nullptr) {
            throw ErrObjectCreation();
        }
        if(repoWind == nullptr) {
            throw ErrObjectCreation();
        }
        if(modeService == nullptr) {
            throw ErrObjectCreation();
        }
        windStream = new OSDStreamWind(config, repoWind, modeService);
    }
    return windStream;
}

BaseError OSDStreamWind::check()
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

void OSDStreamWind::onDataReceived(QByteArray data)
{
    try {
        QJsonObject respObj = Utils::byteArrayToJsonObject(data);
        WindModel model(respObj["speed"].toDouble(),respObj["direction"].toDouble());

#ifdef USE_LOG4QT
        logger()->trace()<<Q_FUNC_INFO<<" -> speed: "<<model.getSpeed()
                       <<", dir: "<<model.getDirection()
                          ;
#else
        qDebug()<<Q_FUNC_INFO<<"data Wind: speed ->"<<model.getSpeed()<<"direction ->"<<model.getDirection();
#endif

        //check source mode manual
        if (respObj.contains("source")) {
            if (respObj["source"].toString().contains("manual")) {
                return;
            }
        }

        auto windMode = serviceMode->getDataMode().getWind();
        if (!windMode && check().getCode() == ERROR_NO.first) {
            _repoWind->SetWind(OSDWindEntity(
                                   model.getSpeed(),
                                   model.getDirection(),
                                   respObj["source"].toString().toStdString(),
                                   respObj["status"].toString().toStdString(),
                                   OSD_MODE::AUTO
                    ));
        }

        handleError(respObj["status"].toString());

        emit signalDataProcessed(model);
    } catch (ErrJsonParse &e) {
        qDebug()<<Q_FUNC_INFO<<"caught error: "<<e.getMessage();
    }  catch (...) {
        qDebug()<<Q_FUNC_INFO<<"caught unkbnown error";
    }
}

void OSDStreamWind::handleError(const QString &err)
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
