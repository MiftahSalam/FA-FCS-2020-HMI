//#include "mainwindow.h"
//#include "global.h"

//#include <QApplication>
//#include <QFile>
//#include <QDebug>
//#include <QSettings>

//QSplashScreen *splash;
//bool verbose;

//int main(int argc, char *argv[])
//{
//    QApplication a(argc, argv);

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

////    w.showFullScreen();
//    w.show();

//}

#include <QObject>
#include <QTest>

//#include "qgridlayout.h"
#include "src/di/di.h"
#include "src/view/mainwindow.h"
//#include "src/view/panel/osd/frame_osd_gyro.h"
//#include "src/view/panel/osd/frame_osd_position.h"
//#include "src/view/panel/osd/frameosd.h"
//#include "test/testsuit.h"

#ifdef USE_LOG4QT
#include <log4qt/logger.h>
#include <log4qt/propertyconfigurator.h>
#include <log4qt/loggerrepository.h>

static void initializeRootLogger();
static void shutdownRootLogger();
static void logStartup();
static void logShutdown();
static void setupRootLogger(const QString &introMessage);
static void shutDownRootLogger(const QString &extroMessage);
#endif

QString loadStylesheetFile( const QString &path );

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

#ifdef USE_LOG4QT
    initializeRootLogger();
    logStartup();
#endif

    QString appStyle = loadStylesheetFile( ":/styles/HMI_Style.css" );
    a.setStyle("plastique");
    a.setStyleSheet( appStyle );

    DI *di = DI::getInstance();

    MainWindow w(nullptr, di);
    // w.showMaximized();
    w.showFullScreen();
    //    w.show();

    //    QWidget *testWidget = new QWidget();
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

    /*
    FrameOSD *osd = new FrameOSD(testWidget);
    QHBoxLayout *testLayout = new QHBoxLayout(testWidget);

    testLayout->addWidget(osd);
    testWidget->setLayout(testLayout);
    testWidget->show();
    */

    //    int status = 0;
    //    auto runTest = [&status, argc, argv](QObject *obj) {
    //        status |= QTest::qExec(obj, argc, argv);
    //    };

    //    auto &suite = TestSuit::suits();
    //    for (auto it = suite.begin(); it != suite.end(); ++it) {
    //        runTest(*it);
    //    }

    int ret;
    {
        ret = a.exec();
    }

#ifdef USE_LOG4QT
    logShutdown();
    shutdownRootLogger();
#endif

    return ret;
}

#ifdef USE_LOG4QT
void initializeRootLogger()
{
    setupRootLogger(QStringLiteral("Root logger is setup."));
}

void shutdownRootLogger()
{
    shutDownRootLogger(QStringLiteral("Root logger was shutdown."));
}

void logStartup()
{
    auto logger = Log4Qt::Logger::rootLogger();

    logger->info(QStringLiteral("################################################################"));
    logger->info(QStringLiteral("#                          START                               #"));
    logger->info(QStringLiteral("################################################################"));
}

void logShutdown()
{
    auto logger = Log4Qt::Logger::rootLogger();

    logger->info(QStringLiteral("################################################################"));
    logger->info(QStringLiteral("#                          STOP                                #"));
    logger->info(QStringLiteral("################################################################"));
}

void setupRootLogger(const QString &introMessage)
{
    QString configFile = QApplication::applicationDirPath() + QDir::separator() + QStringLiteral(".log4qt.properties");
    qDebug()<<configFile;
    if (QFile::exists(configFile))
        Log4Qt::PropertyConfigurator::configureAndWatch(configFile);
    if (!introMessage.isEmpty())
        Log4Qt::Logger::rootLogger()->info(introMessage);
}

void shutDownRootLogger(const QString &extroMessage)
{
    auto logger = Log4Qt::Logger::rootLogger();

    if (!extroMessage.isEmpty())
        logger->info(extroMessage);
    logger->removeAllAppenders();
    logger->loggerRepository()->shutdown();
}
#endif
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
