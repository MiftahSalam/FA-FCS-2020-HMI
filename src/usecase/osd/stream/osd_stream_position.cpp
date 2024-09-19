#include "osd_stream_position.h"
#include "src/shared/common/errors/err_json_parse.h"
#include "src/shared/common/errors/err_object_creation.h"
#include "src/shared/common/errors/err_osd_data.h"
#include "src/shared/utils/utils.h"

OSDStreamPosition* OSDStreamPosition::positionStream = nullptr;

OSDStreamPosition::OSDStreamPosition(TcpMessagingOpts *config,
        OSDPositionRepository *repoPos,
        OSDCMSInputMode *modeService)
//OSDStreamPosition::OSDStreamPosition(AMQPConfig *config)
    : cfg(config), _repoPos(repoPos), serviceMode(modeService), currentErr(NoError())
{
    /*
    AMQPOptions *opt = new AMQPOptions(
                config->getInstance("")->getServerAddress(),
                "fa-fcs-hmi:position",
                "position",
                "topic",
                "*.position",
                QStringList()
                );
    consumer = new AmqpConsumerWrapper(this, opt);
    connect(consumer, &AmqpConsumerWrapper::signalForwardMessage, this, &OSDStreamPosition::onDataReceived);
    consumer->Connect();
    */
    consumer = new TcpMessagingWrapper(this, config);
    connect(consumer, &TcpMessagingWrapper::signalForwardMessage, this, &OSDStreamPosition::onDataReceived);
}

OSDStreamPosition *OSDStreamPosition::getInstance(
        TcpMessagingOpts *config = nullptr,
        OSDPositionRepository* repoPos = nullptr,
        OSDCMSInputMode *modeService = nullptr
        )
//OSDStreamPosition *OSDStreamPosition::getInstance(AMQPConfig *config = nullptr)
{
    if (positionStream == nullptr) {
        if(config == nullptr) {
            throw ErrObjectCreation();
        }

        if(repoPos == nullptr) {
            throw ErrObjectCreation();
        }

        if(modeService == nullptr) {
            throw ErrObjectCreation();
        }

        positionStream = new OSDStreamPosition(config, repoPos, modeService);
    }

    return positionStream;
}

BaseError OSDStreamPosition::check()
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

void OSDStreamPosition::onDataReceived(QByteArray data)
{
    try {
        QJsonObject respObj = Utils::byteArrayToJsonObject(data);
        PositionModel model(respObj["latitude"].toDouble(-91),respObj["longitude"].toDouble(-181));

        qDebug()<<Q_FUNC_INFO<<"data position: lat ->"<<model.getLatitude()<<"lon ->"<<model.getLongitude();

        //check source mode manual
        if (respObj.contains("source")) {
            if (respObj["source"].toString().contains("manual")) {
                return;
            }
        }

        auto positionMode = serviceMode->getDataMode().getPosition();
        if (!positionMode && check().getCode() == ERROR_NO.first) {
            _repoPos->SetPosition(OSDPositionEntity(
                                      model.getLatitude(),
                                      model.getLongitude(),
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

void OSDStreamPosition::handleError(const QString &err)
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
