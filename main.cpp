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

    MainWindow w;

    QPixmap pixmap(":/logofcs.png");
    pixmap = pixmap.scaled(300,100);
    splash = new QSplashScreen(pixmap);

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
