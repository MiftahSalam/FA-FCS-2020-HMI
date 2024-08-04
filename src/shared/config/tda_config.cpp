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

bool TDAConfig::getCompassStatus() const
{
    return compassStatus;
}

void TDAConfig::setCompassStatus(bool newCompasStatus)
{
    compassStatus = newCompasStatus;
}

bool TDAConfig::getHeadingMarkerStatus() const
{
    return headingMarkerStatus;
}

void TDAConfig::setHeadingMarkerStatus(bool newHeadingMarkerStatus)
{
    headingMarkerStatus = newHeadingMarkerStatus;
}

bool TDAConfig::getGunCoverageStatus() const
{
    return gunCoverageStatus;
}

void TDAConfig::setGunCoverageStatus(bool newGunCoverageStatus)
{
    gunCoverageStatus = newGunCoverageStatus;
}

bool TDAConfig::getGunBarrelStatus() const
{
    return gunBarrelStatus;
}

void TDAConfig::setGunBarrelStatus(bool newGunBarrelStatus)
{
    gunBarrelStatus = newGunBarrelStatus;
}

void TDAConfig::setup(const QString path)
{
    QSettings configFile(path,QSettings::IniFormat);

    // compassStatus = configFile.value(CONFIG_COMPASS_STATUS, "").toString();
    // headingMarkerStatus = configFile.value(CONFIG_HEADING_MARKER_STATUS, "").toString();
    // gunCoverageStatus = configFile.value(CONFIG_GUN_COVERAGE_STATUS, "").toString();
    // gunBarrelStatus = configFile.value(CONFIG_GUN_BARREL_STATUS, "").toString();
}

void TDAConfig::saveTDAConfig()
{
    QString COMMON_CONFIG_PATH = QDir::homePath()+QDir::separator()+".fa-hmi.conf";
    QSettings configFile(COMMON_CONFIG_PATH,QSettings::IniFormat);

    configFile.beginGroup("tda");
    configFile.setValue("show_compass", QVariant(compassStatus).toString());
    configFile.setValue("show_heading_marker", QVariant(headingMarkerStatus).toString());
    configFile.setValue("show_gun_coverage", QVariant(gunCoverageStatus).toString());
    configFile.setValue("show_gun_barrel", QVariant(gunBarrelStatus).toString());
    configFile.endGroup();

    // qDebug()<<"Save TDA Config";
}
