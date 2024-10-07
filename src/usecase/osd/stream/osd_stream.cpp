#include "osd_stream.h"
#include "src/shared/common/errors/err_object_creation.h"

// OSDStream::OSDStream(QObject *parent, AMQPConfig *config): QObject(parent), cfgAmqp(config)
//{
//     if(cfgAmqp == nullptr) {
//         throw ErrObjectCreation();
//     }

//    serviceOSDStreamPosition = OSDStreamPosition::getInstance(cfgAmqp);
//}

OSDStream::OSDStream(
        QObject *parent,
        MessagingTcpConfig *config,
        OSDRepository *repoOSD,
        OSDCMS *osdService
        ): QObject(parent), consumerConfig(config), serviceOSDCMS(osdService)
{
    if (config == nullptr)
    {
        throw ErrObjectCreation();
    }

    if (repoOSD == nullptr)
    {
        throw ErrObjectCreation();
    }

    if (osdService == nullptr)
    {
        throw ErrObjectCreation();
    }
// ambil nilai dari config
    TcpMessagingOpts *posStreamVal = config->getInstance("")->getContent().value("position");
    serviceOSDStreamPosition = OSDStreamPosition::getInstance(
                posStreamVal,
                repoOSD->getRepoOSDPosition(),
                osdService->getServiceOSDCMSMode()
                );

    TcpMessagingOpts *gyroStreamVal = config->getInstance("")->getContent().value("inertia");
    serviceOSDStreamGyro = OSDStreamGyro::getInstance(
                gyroStreamVal,
                repoOSD->getRepoOSDInertia(),
                osdService->getServiceOSDCMSMode()
                );

    TcpMessagingOpts *waterspeedStreamVal = config->getInstance("")->getContent().value("water_speed");
    serviceOSDStreamWaterSpeed = OSDStreamWaterSpeed::getInstance(
                waterspeedStreamVal,
                repoOSD->getRepoOSDWaterSpeed(),
                osdService->getServiceOSDCMSMode()
                );

    TcpMessagingOpts *speedStreamVal = config->getInstance("")->getContent().value("speed");
    serviceOSDStreamSpeed = OSDStreamSpeed::getInstance(
                speedStreamVal,
                repoOSD->getRepoOSDSpeed(),
                osdService->getServiceOSDCMSMode()
                );

    TcpMessagingOpts *windStreamVal = config->getInstance("")->getContent().value("wind");
    serviceOSDStreamWind = OSDStreamWind::getInstance(
                windStreamVal,
                repoOSD->getRepoOSDWind(),
                osdService->getServiceOSDCMSMode()
                );

    TcpMessagingOpts *weatherStreamVal = config->getInstance("")->getContent().value("weather");
    serviceOSDStreamWeather = OSDStreamWeather::getInstance(
        weatherStreamVal,
        repoOSD->getRepoOSDWeather(),
        osdService->getServiceOSDCMSMode()
        );

    TcpMessagingOpts *datetimeStreamVal = config->getInstance("")->getContent().value("date_time");
    serviceStreamDateTime = OSDStreamDateTime::getInstance(
        datetimeStreamVal,
        repoOSD->getRepoDateTime()
        );
}

OSDStreamPosition *OSDStream::getServiceOSDStreamPosition() const
{
    return serviceOSDStreamPosition;
}

OSDStreamGyro *OSDStream::getServiceOSDStreamGyro() const
{
    return serviceOSDStreamGyro;
}

OSDStreamWaterSpeed *OSDStream::getServiceOSDStreamWaterSpeed() const
{
    return serviceOSDStreamWaterSpeed;
}

OSDStreamSpeed *OSDStream::getServiceOSDStreamSpeed() const
{
    return serviceOSDStreamSpeed;
}

OSDStreamWind *OSDStream::getServiceOSDStreamWind() const
{
    return serviceOSDStreamWind;
}

OSDStreamWeather *OSDStream::getServiceOSDStreamWeather() const
{
    return serviceOSDStreamWeather;
}

OSDStreamDateTime *OSDStream::getServiceStreamDateTime() const
{
    return serviceStreamDateTime;
}
