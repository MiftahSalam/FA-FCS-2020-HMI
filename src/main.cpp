//#include "mainwindow.h"
//#include "global.h"

//#include <QApplication>
//#include <QFile>
//#include <QDebug>
//#include <QSettings>

//QString loadStylesheetFile( const QString &path );
//QSplashScreen *splash;
//bool verbose;

//int main(int argc, char *argv[])
//{
//    QApplication a(argc, argv);

//    QString appStyle = loadStylesheetFile( ":/HMI_Syle.css" );
//    a.setStyle("plastique");
//    a.setStyleSheet( appStyle );

//    QSettings config(QSettings::IniFormat,QSettings::UserScope,"hmi_config");
//    QFile file(config.fileName());

//    if(!file.exists())
//    {
//        qDebug()<<"init config file";

//        config.setValue("Redis/osd","tcp://192.168.1.240:6379");
//        config.setValue("Apps/verbose",false);
//    }

//    MainWindow w;

//    QPixmap pixmap(":/logofcs.png");
//    pixmap = pixmap.scaled(600,200);
//    splash = new QSplashScreen(pixmap);
//    splash->setStyleSheet("font-size: 20px;");

//    w.setConfig(config);
////    w.showFullScreen();
//    w.show();

//    return a.exec();
//}

//QString loadStylesheetFile( const QString &path )
//{
//    /**
//     * Load application stylesheet file (.qss) from given path
//     * then return the contents to function caller
//     */

//    QFile styleFile( path );
//    if( styleFile.open( QFile::ReadOnly ) )
//    {
//        qDebug() << "loading stylesheet file...";
//        QString style = QLatin1String( styleFile.readAll() );
//        return style;
//    }
//    else
//    {
//        qDebug() << path << ": file not found!";
//        return "";
//    }
//}

#include <QObject>
#include <QTest>

#include "src/di/di.h"
#include "test/testsuit.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    DI *di = new DI();
    di->getOSDCMSService()->getServiceOSDCMSPosition()->setPosition(OSDSetPositionRequest(1.2,-32.1));
//    int status = 0;
//    auto runTest = [&status, argc, argv](QObject *obj) {
//        status |= QTest::qExec(obj, argc, argv);
//    };

//    auto &suite = TestSuit::suits();
//    for (auto it = suite.begin(); it != suite.end(); ++it) {
//        runTest(*it);
//    }

    return a.exec();
}
