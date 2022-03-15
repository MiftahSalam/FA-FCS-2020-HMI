#ifndef REDISCONNECTIONNOTIFY_H
#define REDISCONNECTIONNOTIFY_H

#include <QWidget>
#include <QTcpSocket>
#include <QDebug>
#include <QTimer>
#include <QMessageBox>

class RedisConnectionNotify : public QWidget
{
    Q_OBJECT

public:
    RedisConnectionNotify(QWidget *parent = 0);
    ~RedisConnectionNotify();

    struct ServerConfig
    {
        QString ip;
        int port;
    };

    struct ServerHandler1
    {
        QTcpSocket *socket;
        ServerConfig config;
    };

    QList<ServerHandler1*>serverList;


public slots:
    void timerOut();

private:
    QMessageBox msgBox;
    QTimer *timer;

};

#endif // REDISCONNECTIONNOTIFY_H
