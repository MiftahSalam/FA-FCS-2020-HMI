#include "mainwindow.h"
#include "global.h"

#include <QApplication>
#include <QFile>
#include <QDebug>
#include <QSettings>

QString loadStylesheetFile( const QString &path );
QSplashScreen *splash;

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

        config.setValue("Redis/osd","tcp://192.168.1.240:6379");
    }

    MainWindow w;

    QPixmap pixmap(":/logofcs.png");
    pixmap = pixmap.scaled(600,200);
    splash = new QSplashScreen(pixmap);
    splash->setStyleSheet("font-size: 20px;");

    w.setConfig(config);
    w.showFullScreen();

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
