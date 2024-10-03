#include "serial_messaging_wrapper.h"
#include "src/shared/common/errors/err_messaging.h"
#include "src/shared/common/errors/err_object_creation.h"

#ifdef USE_LOG4QT
#include <log4qt/logger.h>
LOG4QT_DECLARE_STATIC_LOGGER(logger, SerialMessagingWrapper)
#else
#include <QDebug>
#endif

SerialMessagingWrapper::SerialMessagingWrapper(QObject *parent, SerialMessagingOpts *cfg)
    : QObject{parent}, config(cfg), currentError(0, "")
{
    if (cfg == nullptr) {
        throw ErrObjectCreation();
    }

    currentError = NoError();

    serialPort = new QSerialPort(this);
    connect(serialPort, &QSerialPort::readyRead, this, &SerialMessagingWrapper::onReadyRead);

    setupSerial();
}

BaseError SerialMessagingWrapper::checkConnection()
{
    if (!serialPort->isOpen()) {
        setupSerial();
    }

    QDateTime now = QDateTime::currentDateTime();
    auto deltaDelay = timestampDelay.msecsTo(now);
    auto deltaHB = timestampHeartBeat.msecsTo(now);
    if (!serialPort->isOpen()) {
        return ErrMessagingNotConnected();
    } else if (deltaHB > config->timeout) {
        return ErrMessagingDataNoData();
    } else if (deltaDelay > config->timeout) {
        return ErrMessagingDataInvalidFormat();
    } else {
        return NoError();
    }
}

void SerialMessagingWrapper::sendActivateButtonMessage(QByteArray data)
{
    serialPort->write(data);
}

void SerialMessagingWrapper::onReadyRead()
{
    QDateTime now = QDateTime::currentDateTime();
    timestampHeartBeat = now;

    QByteArray data = serialPort->readAll();
    if (data.startsWith("{") && data.endsWith("}")) {
        auto delta = timestampDelay.msecsTo(now);
#ifdef USE_LOG4QT
        logger()->trace()<<Q_FUNC_INFO<<" -> delta; "<<delta<<", delay: "<<config->delay;
#else
        qDebug()<<Q_FUNC_INFO<<"delta"<<delta<<"delay"<<config->delay;
#endif
        if ( delta > config->delay) {
            timestampDelay = now;
            emit signalForwardMessage(data);
        }
    }

#ifdef USE_LOG4QT
    logger()->trace()<<Q_FUNC_INFO<<" -> data; "<<data;
#else
    qDebug()<<Q_FUNC_INFO<<"data"<<data;
#endif
}

void SerialMessagingWrapper::setupSerial()
{
    serialPort->setPortName(config->portname);
    if (!serialPort->open(QIODevice::ReadWrite)) {
#ifdef USE_LOG4QT
        logger()->warn()<<Q_FUNC_INFO<<" -> error open serial port: "<<config->portname<<" with error: "<<serialPort->errorString();
#else
        qWarning()<<Q_FUNC_INFO<<"error open serial port"<<config->portname<<"with error"<<serialPort->errorString();
#endif
        return;
    }

    serialPort->setBaudRate(cfgToBaudrate());
    serialPort->setDataBits(cfgToDatabits());
    serialPort->setStopBits(cfgToStopbits());
    serialPort->setParity(cfgToParity());
    serialPort->setFlowControl(cfgToFlowControl());

    QDateTime now = QDateTime::currentDateTime();
    timestampDelay = now;
    timestampHeartBeat = now;

}

QSerialPort::BaudRate SerialMessagingWrapper::cfgToBaudrate()
{
    if (config->baudrate == "1200") {
        return QSerialPort::Baud1200;
    } else if (config->baudrate == "2400") {
        return QSerialPort::Baud2400;
    } else if (config->baudrate == "4800") {
        return QSerialPort::Baud4800;
    } else if (config->baudrate == "9600") {
        return QSerialPort::Baud9600;
    }  else if (config->baudrate == "19200") {
        return QSerialPort::Baud19200;
    }  else if (config->baudrate == "38400") {
        return QSerialPort::Baud38400;
    }  else if (config->baudrate == "57600") {
        return QSerialPort::Baud57600;
    }  else if (config->baudrate == "115200") {
        return QSerialPort::Baud115200;
    }

#ifdef USE_LOG4QT
    logger()->warn()<<Q_FUNC_INFO<<" -> invalid baudrate: "<<config->baudrate;
#else
    qWarning()<<Q_FUNC_INFO<<"invalid baudrate"<<config->baudrate;
#endif

#if QT_VERSION_MAJOR < 6
    return QSerialPort::UnknownBaud;
#else
    return (QSerialPort::BaudRate)-1;
#endif
}

QSerialPort::DataBits SerialMessagingWrapper::cfgToDatabits()
{
    if (config->databits == "8") {
        return QSerialPort::Data8;
    } else if (config->databits == "7") {
        return QSerialPort::Data7;
    } else if (config->databits == "6") {
        return QSerialPort::Data6;
    } else if (config->databits == "5") {
        return QSerialPort::Data5;
    }

#ifdef USE_LOG4QT
    logger()->warn()<<Q_FUNC_INFO<<" -> invalid databit: "<<config->databits;
#else
    qWarning()<<Q_FUNC_INFO<<"invalid databit"<<config->databits;
#endif

#if QT_VERSION_MAJOR < 6
    return QSerialPort::UnknownDataBits;
#else
    return (QSerialPort::DataBits)-1;
#endif
}

QSerialPort::StopBits SerialMessagingWrapper::cfgToStopbits()
{
    if (config->stopbits == "1") {
        return QSerialPort::OneStop;
    } else if (config->stopbits == "1/2") {
        return QSerialPort::OneAndHalfStop;
    } else if (config->stopbits == "2") {
        return QSerialPort::TwoStop;
    }

#ifdef USE_LOG4QT
    logger()->warn()<<Q_FUNC_INFO<<" -> invalid stopbits: "<<config->stopbits;
#else
    qWarning()<<Q_FUNC_INFO<<"invalid stopbits"<<config->stopbits;
#endif

#if QT_VERSION_MAJOR < 6
    return QSerialPort::UnknownStopBits;
#else
    return (QSerialPort::StopBits)-1;
#endif
}

QSerialPort::Parity SerialMessagingWrapper::cfgToParity()
{
    if (config->parity == "no") {
        return QSerialPort::NoParity;
    } else if (config->parity == "even") {
        return QSerialPort::EvenParity;
    } else if (config->parity == "odd") {
        return QSerialPort::OddParity;
    } else if (config->parity == "space") {
        return QSerialPort::SpaceParity;
    } else if (config->parity == "mark") {
        return QSerialPort::MarkParity;
    }

#ifdef USE_LOG4QT
    logger()->warn()<<Q_FUNC_INFO<<" -> invalid parity: "<<config->parity;
#else
    qWarning()<<Q_FUNC_INFO<<"invalid parity"<<config->parity;
#endif

#if QT_VERSION_MAJOR < 6
    return QSerialPort::UnknownParity;
#else
    return (QSerialPort::Parity)-1;
#endif
}

QSerialPort::FlowControl SerialMessagingWrapper::cfgToFlowControl()
{
    if (config->flowControls == "no") {
        return QSerialPort::NoFlowControl;
    } else if (config->flowControls == "hw") {
        return QSerialPort::HardwareControl;
    } else if (config->flowControls == "sw") {
        return QSerialPort::SoftwareControl;
    }

#ifdef USE_LOG4QT
    logger()->warn()<<Q_FUNC_INFO<<" -> invalid flowControls: "<<config->flowControls;
#else
    qWarning()<<Q_FUNC_INFO<<"invalid flowControls"<<config->flowControls;
#endif

#if QT_VERSION_MAJOR < 6
    return QSerialPort::UnknownFlowControl;
#else
    return (QSerialPort::FlowControl)-1;
#endif
}
