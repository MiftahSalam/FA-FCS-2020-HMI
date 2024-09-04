#ifndef SERIALMESSAGINGWRAPPER_H
#define SERIALMESSAGINGWRAPPER_H

#include "qdatetime.h"
#include "src/shared/common/errors/err_base.h"
#include "src/shared/config/messaging_serial_config.h"
#include <QObject>

class SerialMessagingWrapper : public QObject
{
    Q_OBJECT
public:
    explicit SerialMessagingWrapper(QObject *parent = nullptr, SerialMessagingOpts *cfg = nullptr);

    BaseError checkConnection();

signals:
    void signalForwardMessage(QByteArray msg);

private slots:
    void onReadyRead();

private:
    QSerialPort *serialPort;
    SerialMessagingOpts *config;
    QDateTime timestampHeartBeat;
    QDateTime timestampDelay;
    BaseError currentError;

    void setupSerial();
    QSerialPort::BaudRate cfgToBaudrate();
    QSerialPort::DataBits cfgToDatabits();
    QSerialPort::StopBits cfgToStopbits();
    QSerialPort::Parity cfgToParity();
    QSerialPort::FlowControl cfgToFlowControl();

};

#endif // SERIALMESSAGINGWRAPPER_H
