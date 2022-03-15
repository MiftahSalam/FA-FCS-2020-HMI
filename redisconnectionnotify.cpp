#include "redisconnectionnotify.h"

#include <QDebug>
#include <QHostAddress>
#include <QPushButton>
#include <QFile>
#include <QSettings>

RedisConnectionNotify::RedisConnectionNotify(QWidget *parent)
    : QWidget(parent)
{
    // ==== Configuration to Server ==== //
    QSettings settings(QSettings::IniFormat,QSettings::UserScope,"hmi_config");
    QFile file(settings.fileName());

    if(!file.exists())
    {
        QMessageBox::critical(this,"Fatal Error","Config file not found",QMessageBox::Ok);
        exit(1);
    }

    settings.sync();
    settings.beginGroup("Redis");

    serverList.clear();

    QStringList redis_child_settings = settings.childKeys();

    foreach (QString key, redis_child_settings)
    {
        QString settings_value = settings.value(key).toString();
        QStringList settings_value_split_1 = settings_value.split("//",QString::SkipEmptyParts);
        if(settings_value_split_1.size() < 2)
        {
            QMessageBox::critical(this,"Fatal Error","Config syntax error in first split",QMessageBox::Ok);
            exit(1);
        }
        QStringList settings_value_split_2 = settings_value_split_1.at(1).split(":",QString::SkipEmptyParts);
        if(settings_value_split_2.size() < 2)
        {
            QMessageBox::critical(this,"Fatal Error","Config syntax error in second split",QMessageBox::Ok);
            exit(1);
        }

        ServerHandler1 *buf_server_handler = new ServerHandler1;
        buf_server_handler->config.ip = settings_value_split_2.at(0);
        buf_server_handler->config.port = settings_value_split_2.at(1).toInt();
        serverList.append(buf_server_handler);

        qDebug()<<"buf_server_handler"<<buf_server_handler->config.ip<<buf_server_handler->config.port;

    }

    // ==== Connecting to Server ==== //
    foreach (ServerHandler1 *buf_server, serverList)
    {
        buf_server->socket = new QTcpSocket(this);
        buf_server->socket->connectToHost(buf_server->config.ip,buf_server->config.port);
        buf_server->socket->waitForConnected(1000);
        qDebug()<<"server"<<buf_server->config.ip
               <<buf_server->config.port<<buf_server->socket->state();
    }
    settings.endGroup();

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()),this, SLOT(timerOut()));
    timer->start(1000);
    qDebug()<<"test2";
}

void RedisConnectionNotify::timerOut()
{
    foreach (ServerHandler1 *buf_server, serverList)
    {
        if(buf_server->socket->state() == QAbstractSocket::ConnectedState)
        {
            qDebug()<<"server connected"<<buf_server->config.ip<<buf_server->config.port;
            buf_server->socket->write("ping \n");
            buf_server->socket->waitForBytesWritten(1000);
            buf_server->socket->waitForReadyRead(3000);
            QByteArray data_feedback =buf_server->socket->readAll();
             qDebug() << "Reading: " << data_feedback;

            if(data_feedback.size() <= 0)
            {
                QMessageBox::critical(this,"Fatal Error",
                                      QString("No reply from database %1").arg(buf_server->config.ip),
                                      QMessageBox::Ok);
                buf_server->socket->close();
            }
        }
        else
        {
            buf_server->socket->connectToHost(buf_server->config.ip,buf_server->config.port);
            QMessageBox::critical(this,"Fatal Error",
                                  QString("Cannot connect to database %1").arg(buf_server->config.ip),
                                  QMessageBox::Ok);
        }
    }
}
RedisConnectionNotify::~RedisConnectionNotify()
{

}
