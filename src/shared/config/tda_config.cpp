#include "tda_config.h"
#include "src/shared/common/errors/err_messaging.h"
#include "src/shared/common/errors/err_open_file.h"

#include <QDebug>
#include <QFile>
#include <QSettings>
#include <QDir>

const QString CONFIG_COMPASS_STATUS = "tda/show_compass";
const QString CONFIG_HEADING_MARKER_STATUS = "tda/show_heading_marker";
const QString CONFIG_GUN_COVERAGE_STATUS = "tda/show_gun_coverage";
const QString CONFIG_GUN_BARREL_STATUS = "tda/show_gun_barrel";

TDAConfig *TDAConfig::config = nullptr;

TDAConfig::TDAConfig()
{

}

TDAConfig::~TDAConfig()
{

}

TDAConfig *TDAConfig::getInstance(const QString path)
{
    qDebug()<<Q_FUNC_INFO<<"path"<<path;

    if(config == nullptr) {
        if (!QFile::exists(path)) {
            throw ErrFileNotFound();
        }

        config = new TDAConfig();
        config->setup(path);
    }

    return config;
}

QString TDAConfig::getCompassStatus() const
{
    return compassStatus;
}

void TDAConfig::setCompassStatus(QString newCompasStatus)
{
    compassStatus = newCompasStatus;
}

QString TDAConfig::getHeadingMarkerStatus() const
{
    return headingMarkerStatus;
}

void TDAConfig::setHeadingMarkerStatus(QString newHeadingMarkerStatus)
{
    headingMarkerStatus = newHeadingMarkerStatus;
}

QString TDAConfig::getGunCoverageStatus() const
{
    return gunCoverageStatus;
}

void TDAConfig::setGunCoverageStatus(QString newGunCoverageStatus)
{
    gunCoverageStatus = newGunCoverageStatus;
}

QString TDAConfig::getGunBarrelStatus() const
{
    return gunBarrelStatus;
}

void TDAConfig::setGunBarrelStatus(QString newGunBarrelStatus)
{
    gunBarrelStatus = newGunBarrelStatus;
}

void TDAConfig::setup(const QString path)
{
    QSettings configFile(path,QSettings::IniFormat);

    compassStatus = configFile.value(CONFIG_COMPASS_STATUS, "").toString();
    headingMarkerStatus = configFile.value(CONFIG_HEADING_MARKER_STATUS, "").toString();
    gunCoverageStatus = configFile.value(CONFIG_GUN_COVERAGE_STATUS, "").toString();
    gunBarrelStatus = configFile.value(CONFIG_GUN_BARREL_STATUS, "").toString();
}

void TDAConfig::saveTDAConfig()
{
    QString COMMON_CONFIG_PATH = QDir::homePath()+QDir::separator()+".fa-hmi.conf";
    QSettings configFile(COMMON_CONFIG_PATH,QSettings::IniFormat);

    configFile.beginGroup("tda");
    configFile.setValue("show_compass", compassStatus);
    configFile.setValue("show_heading_marker", headingMarkerStatus);
    configFile.setValue("show_gun_coverage", gunCoverageStatus);
    configFile.setValue("show_gun_barrel", gunBarrelStatus);
    configFile.endGroup();

    // qDebug()<<"Save TDA Config";
}
