#include "mainwindow.h"
#include "global.h"

#include <QApplication>
#include "redisconnectionnotify.h"
#include <QFile>
#include <QDebug>
#include <QSettings>

QString loadStylesheetFile( const QString &path );
QSplashScreen *splash;
bool verbose;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv); 

    QString appStyle = loadStylesheetFile( ":/HMI_Syle.css" );
    a.setStyle("plastique");
    a.setStyleSheet( appStyle );

    QSettings config(QSettings::IniFormat,QSettings::UserScope,"hmi_config");
    QFile file(config.fileName());

    if(!file.exists())
    {
        qDebug()<<"init config file";
#ifdef WIN32
        config.setValue("Redis/osd","127.0.0.1:6379");
        config.setValue("Redis/track","127.0.0.1:6379");
        config.setValue("Redis/gun","127.0.0.1:6379");
#else
        config.setValue("Redis/osd","tcp://192.168.1.240:6379");
#endif
        config.setValue("Apps/verbose",false);

    }
    RedisConnectionNotify z;

    MainWindow w;

    QPixmap pixmap(":/logofcs.png");
    pixmap = pixmap.scaled(600,200);
    splash = new QSplashScreen(pixmap);
    splash->setStyleSheet("font-size: 20px;");

    w.setConfig(config);
    w.showFullScreen();
//    w.show();

    return a.exec();
}

QString loadStylesheetFile( const QString &path )
{
    /**
     * Load application stylesheet file (.qss) from given path
     * then return the contents to function caller
     */

    QFile styleFile( path );
    if( styleFile.open( QFile::ReadOnly ) )
    {
        qDebug() << "loading stylesheet file...";
        QString style = QLatin1String( styleFile.readAll() );
        return style;
    }
    else
    {
        qDebug() << path << ": file not found!";
        return "";
    }
}
