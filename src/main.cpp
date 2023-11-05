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

#include "qgridlayout.h"
#include "src/di/di.h"
#include "src/view/panel/osd/frame_osd_gyro.h"
#include "src/view/panel/osd/frame_osd_position.h"
#include "src/view/panel/osd/frameosd.h"
//#include "test/testsuit.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    DI *di = new DI();
    di->getOSDCMSService()->getServiceOSDCMSPosition()->set(OSDSetPositionRequest(1.2,-32.1)); //test

    QWidget *testWidget = new QWidget();
    /*
    FrameOSDGyro *testGyo = new FrameOSDGyro(testWidget);
    testGyo->modify(
                OSDGyroProp{
                    "Gyro",
                    TextInputProp{
                        "Heading:", "deg", "headingInput", "0.0"
                    },
                    TextInputProp{
                        "Pitch:", "deg", "pitchInput", "0.0"
                    },
                    TextInputProp{
                        "Roll:", "deg", "rollInput", "0.0"
                    },
                });

    FrameOSDPosition *testPos = new FrameOSDPosition(testWidget);
    testPos->modify(
                OSDPositionProp{
                    "Position",
                    TextInputProp{
                        "Latitude:", "deg", "latInput", "0.0"
                    },
                    TextInputProp{
                        "Longitude:", "deg", "latInput", "0.0"
                    },
                });

//        QHBoxLayout *testLayout = new QHBoxLayout(testWidget);
    testLayout->addWidget(testGyo);
    testLayout->addWidget(testPos);
    */

    FrameOSD *osd = new FrameOSD(testWidget);
    QHBoxLayout *testLayout = new QHBoxLayout(testWidget);

    testLayout->addWidget(osd);
    testWidget->setLayout(testLayout);
    testWidget->show();

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
