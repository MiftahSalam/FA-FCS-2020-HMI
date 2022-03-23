#include "framegun.h"
#include "ui_framegun.h"
#include "global.h"
#include "unistd.h"

#include <QMessageBox>
#include <QDebug>
#include <QRegExpValidator>

#define NUMBER_RX "[0-9.-]+$"

FrameGun::FrameGun(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::FrameGun)
{
    ui->setupUi(this);

    QRegExp rxNumber(NUMBER_RX);
    QRegExpValidator *valiNumber = new QRegExpValidator(rxNumber, this);

    // ==== validator ==== //
    ui->lineEditAz->setValidator(valiNumber);
    ui->lineEditEl->setValidator(valiNumber);

    // ==== Label Kosong ==== //
    ui->labelGunStatTech->setText("");
    ui->labelGunStatAccess->setText("");
    ui->labelGunStatReadyToStart->setText("");
    ui->labelGunStatReady->setText("");
    ui->labelGunStatFollow->setText("");
    ui->labelGunStatAz->setText("");
    ui->labelGunStatEl->setText("");
    ui->labelGunStatOp->setText("");

#ifdef WIN32
    redisClient = nullptr;
#endif
}

FrameGun::~FrameGun()
{
    delete ui;
}

QString FrameGun::getAccessStatus() const
{
    return gunstatus.access_status;
}

QString FrameGun::getReadyStatus() const
{
    return gunstatus.gun_ready_status;
}

QStringList FrameGun::getGundata() const
{
    QStringList datagunbalistic;
    datagunbalistic <<gunbalistic.orientation <<gunbalistic.blind_arc <<gunbalistic.max_range <<gunstatus.azimuth <<gunstatus.elevation;

//    qDebug() << "hasil get" <<datagunbalistic ;
    return datagunbalistic;
}

void FrameGun::reconnecRedis()
{
#ifdef WIN32
    if(redisClient->isConnected()) return;

    if (redisClient->openConnection())
    {
        qDebug() << "Connected to server GUN...";

        if(redisClient->set("engagement_mode", "Manual"))
        {
            redisClient->hset("gun_op_status", "assign_mode", "-");
            QStringList gunbalisticdata = redisClient->hmget("gun_balistic_data", "orientation blind_arc max_range");

            if(gunbalisticdata.size() == 3)
            {
                gunbalistic.orientation = gunbalisticdata.at(0);
                gunbalistic.blind_arc = gunbalisticdata.at(1);
                gunbalistic.max_range = gunbalisticdata.at(2);
            }
            operationalstatus.assign_mode = "";
            operationalstatus.operational = "";

            //qDebug() << Q_FUNC_INFO << gunbalistic.orientation <<gunbalistic.blind_arc <<gunbalistic.max_range;

            ui->pushButtonGunBarControlApply->setDisabled(true);
            ui->comboBoxGunBarControlMode->setDisabled(true);
            ui->lineEditAz->setDisabled(true);
            ui->lineEditEl->setDisabled(true);
        }
        else qDebug() << Q_FUNC_INFO << "Cannot set engage mode manual";
    }
#endif
}

void FrameGun::setConfig(QString Config)
{
    this->Config = Config;
    qDebug() <<Q_FUNC_INFO <<"Redis config" <<this->Config;

#ifdef WIN32
    QStringList args = Config.split(":");
    if(args.size() != 2) {
        qDebug()<<Q_FUNC_INFO<<"Invalid arguments for redis. Program exit";
        exit(1);
    }
    redisClient = new QtRedis(args.at(0), args.at(1).toInt());

//    if(!redisClient->isConnected())
//    {
//        splash->showMessage("Gun Setup error\n\nServer connection error: Cannot connect to server" + Config + "\n\nApplication now will clossing ",Qt::AlignCenter);
//        sleep(3);
//        splash->finish(this);
//        qApp->exit();
//    }
    reconnecRedis();
#else
    std::vector<std::string> gunbalisticdata;

    try
    {
        redisClient = new Redis(this->Config.toStdString());

        redisClient->set("engagement_mode", "Manual");
        /*
        redisClient->hset("engagement", "azimuth", "0.0");
        redisClient->hset("engagement", "elevation", "0.0");
        redisClient->hset("engagement", "azimuth_status", "engageable");
        redisClient->hset("engagement", "elevation_status", "engageable");
        redisClient->hset("engagement", "azimuth_corr", "0.0");
        redisClient->hset("engagement", "elevation_corr", "0.0");
        */
        redisClient->hset("gun_op_status", "assign_mode", "-");
        redisClient->hmget("gun_balistic_data", {"orientation", "blind_arc", "max_range"}, std::back_inserter(gunbalisticdata));

        gunbalistic.orientation = QString::fromStdString(gunbalisticdata.at(0));
        gunbalistic.blind_arc = QString::fromStdString(gunbalisticdata.at(1));
        gunbalistic.max_range = QString::fromStdString(gunbalisticdata.at(2));
        operationalstatus.assign_mode = "";
        operationalstatus.operational = "";

        //qDebug() << Q_FUNC_INFO << gunbalistic.orientation <<gunbalistic.blind_arc <<gunbalistic.max_range;

        ui->pushButtonGunBarControlApply->setDisabled(true);
        ui->comboBoxGunBarControlMode->setDisabled(true);
        ui->lineEditAz->setDisabled(true);
        ui->lineEditEl->setDisabled(true);
    }
    catch (Error e)
    {
        splash->showMessage("GUN Setup error\n\nDatabase error: " + QString(e.what()) + "\n\nApplication now will clossing ",Qt::AlignCenter);
        sleep(3);
        splash->finish(this);
        qApp->exit();
    }
#endif
}

#ifdef WIN32
void FrameGun::on_gunTimerTimeOut()
{
    if(redisClient == nullptr) return;

    QMap<QString, QVariant> data_gun = redisClient->hgetall("gun_feedback_status");

        gunstatus.technical_status = data_gun.value("technical_status","0").toString();
        gunstatus.access_status = data_gun.value("access_status","0").toString();
        gunstatus.azimuth = data_gun.value("azimuth","0").toString();
        gunstatus.elevation = data_gun.value("elevation","0").toString();
        gunstatus.gun_ready_status = data_gun.value("gun_ready_status","0").toString();
        gunstatus.gun_follow_status = data_gun.value("gun_follow_status","0").toString();
        gunstatus.ready_to_start_status = data_gun.value("ready_to_start_status","0").toString();

//        qDebug() <<"technical_status"<<gunstatus.technical_status
//                 <<"acces_status"<<gunstatus.access_status
//                 <<"ready_to_status"<<gunstatus.ready_to_start_status
//                 <<"gun_status"<<gunstatus.gun_ready_status
//                 <<"gun_follow"<<gunstatus.gun_follow_status
//                 <<"azimuth"<<gunstatus.azimuth
//                 <<"elevation"<<gunstatus.elevation;

        if (gunstatus.technical_status == "1")
        {
            ui->labelGunStatTech->setStyleSheet("color:rgb(0, 255, 0);");
            ui->labelGunStatTech->setText("Online");

            // ==== Access Status ==== //
            if(gunstatus.access_status == "1")
            {
                ui->labelGunStatAccess->setStyleSheet("color:rgb(0, 255, 0);");
                ui->labelGunStatAccess->setText("Remote");

                // ==== Ready To Status ==== //
                if(gunstatus.ready_to_start_status == "1")
                {
                    ui->labelGunStatReadyToStart->setStyleSheet("color:rgb(0, 255, 0);");
                    ui->labelGunStatReadyToStart->setText("Yes");
                }
                else if (gunstatus.ready_to_start_status == "0")
                {
                    ui->labelGunStatReadyToStart->setStyleSheet("color: rgb(255, 0, 0);");
                    ui->labelGunStatReadyToStart->setText("No");
                }

                // ==== Gun Ready ==== //
                if(gunstatus.gun_ready_status == "1")
                {
                    ui->labelGunStatReady->setStyleSheet("color:rgb(0, 255, 0);");
                    ui->labelGunStatReady->setText("Yes");
                }
                else if (gunstatus.gun_ready_status == "0")
                {
                    ui->labelGunStatReady->setStyleSheet("color: rgb(255, 0, 0);");
                    ui->labelGunStatReady->setText("No");
                }

                // ==== Gun Follow ==== //
                if(gunstatus.gun_follow_status == "1")
                {
                    ui->labelGunStatFollow->setStyleSheet("color:rgb(0, 255, 0);");
                    ui->labelGunStatFollow->setText("Yes");
                }
                else if (gunstatus.gun_follow_status == "0")
                {
                    ui->labelGunStatFollow->setStyleSheet("color: rgb(255, 0, 0);");
                    ui->labelGunStatFollow->setText("No");
                }

                if(gunstatus.gun_ready_status == "1")
                {
                    // ==== Azimuth ==== //
                    ui->labelGunStatAz->setText(gunstatus.azimuth);
                    ui->labelGunStatAz->setStyleSheet("color:rgb(0, 255, 0);");
                    ui->labelGunStatEl->setText(gunstatus.elevation);
                    ui->labelGunStatEl->setStyleSheet("color:rgb(0, 255, 0);");

                    // ==== Gun Op Status ===== //

                        QMap<QString, QVariant> data_operational = redisClient->hgetall("gun_op_status");
                        QString engagement_mode = redisClient->get("engagement_mode");

                        operationalstatus.operational = data_operational.value("operational").toString();
                        QString cur_assign_mode = data_operational.value("assign_mode").toString();

                        qDebug() <<"operationalstatus"<<operationalstatus.operational <<"cur_assign_mode" <<cur_assign_mode<<operationalstatus.assign_mode;
                        if(operationalstatus.assign_mode != cur_assign_mode)
                        {
                            if(cur_assign_mode != "-")
                            {
                                if(engagement_mode=="Manual")
                                {
                                    QStringList data_engagement = redisClient->hmget("engagement","azimuth elevation");
                                    if(data_engagement.size() == 2)
                                    {
                                        engagementstatus.azimuth = data_engagement.at(0);
                                        engagementstatus.elevation =data_engagement.at(1);
                                    } else qDebug()<<Q_FUNC_INFO<<"cannot get az, el manual mode";

                                    ui->lineEditAz->setText(engagementstatus.azimuth);
                                    ui->lineEditEl->setText(engagementstatus.elevation);
                                    ui->lineEditAz->setEnabled(true);
                                    ui->lineEditEl->setEnabled(true);
                                    ui->pushButtonGunBarControlApply->setEnabled(true);
                                    ui->comboBoxGunBarControlMode->setCurrentIndex(0);
                                }
                                else
                                {
                                    ui->lineEditAz->setText("");
                                    ui->lineEditEl->setText("");
                                    ui->lineEditAz->setDisabled(true);
                                    ui->lineEditEl->setDisabled(true);
                                    ui->pushButtonGunBarControlApply->setDisabled(true);
                                    ui->comboBoxGunBarControlMode->setCurrentIndex(1);
                                }
                                ui->comboBoxGunBarControlMode->setEnabled(true);
                            }
                            else
                            {
                                ui->lineEditAz->setDisabled(true);
                                ui->lineEditEl->setDisabled(true);
                                ui->comboBoxGunBarControlMode->setEnabled(false);
                                ui->pushButtonGunBarControlApply->setDisabled(true);

                                redisClient->set("engagement_mode", "Manual");
                            }
                            operationalstatus.assign_mode = cur_assign_mode;
                        }

//                        qDebug() <<"operationalstatus"<<operationalstatus.operational <<"engagement" <<QString::fromStdString(data_engagement);

                        if (operationalstatus.operational == "Assigned")
                            ui->labelGunStatOp->setStyleSheet("color:rgb(0, 255, 0);");
                        else if (operationalstatus.operational == "Standby")
                            ui->labelGunStatOp->setStyleSheet("color:rgb(255, 255, 255);");
                        else if (operationalstatus.operational.isEmpty())
                            ui->labelGunStatOp->setStyleSheet("color:rgb(0, 0, 0);");

                        ui->labelGunStatOp->setText(operationalstatus.operational);

                }
                else
                {
                    ui->labelGunStatOp->setText("");
                    ui->labelGunStatAz->setText("");
                    ui->labelGunStatEl->setText("");
                    ui->pushButtonGunBarControlApply->setDisabled(true);
                    ui->comboBoxGunBarControlMode->setDisabled(true);
                    ui->lineEditAz->setDisabled(true);
                    ui->lineEditEl->setDisabled(true);

                    redisClient->set("engagement_mode", "Manual");
                    redisClient->hset("gun_op_status", "assign_mode", "-");
                }
            }
            else
            {
                ui->labelGunStatAccess->setStyleSheet("color: rgb(255, 0, 0);");
                ui->labelGunStatAccess->setText("Local");
                ui->lineEditAz->setDisabled(true);
                ui->lineEditEl->setDisabled(true);
                ui->lineEditAz->setText("");
                ui->lineEditEl->setText("");
                ui->comboBoxGunBarControlMode->setDisabled(true);
                ui->pushButtonGunBarControlApply->setDisabled(true);

                ui->labelGunStatAz->setText("");
                ui->labelGunStatEl->setText("");
                ui->labelGunStatReadyToStart->setText("");
                ui->labelGunStatReady->setText("");
                ui->labelGunStatFollow->setText("");
                ui->labelGunStatOp->setText("");

                redisClient->set("engagement_mode", "Manual");
                redisClient->hset("gun_op_status", "assign_mode", "-");
            }
        }
        else if (gunstatus.technical_status == "0")
        {
            ui->labelGunStatTech->setStyleSheet("color: rgb(255, 0, 0);");
            ui->labelGunStatTech->setText("Offline");

            // ==== Label Kosong ==== //
            ui->labelGunStatAccess->setText("");
            ui->labelGunStatReadyToStart->setText("");
            ui->labelGunStatReady->setText("");
            ui->labelGunStatFollow->setText("");
            ui->labelGunStatAz->setText("");
            ui->labelGunStatEl->setText("");
            ui->labelGunStatOp->setText("");
            ui->lineEditAz->setDisabled(true);
            ui->lineEditEl->setDisabled(true);
            ui->comboBoxGunBarControlMode->setDisabled(true);
            ui->pushButtonGunBarControlApply->setDisabled(true);

            redisClient->set("engagement_mode", "Manual");
            redisClient->hset("gun_op_status", "assign_mode", "-");
        }
}
#else
void FrameGun::on_gunTimerTimeOut()
{
    std::vector<std::string> data_gun;
    try
    {
        redisClient->hgetall("gun_feedback_status",std::back_inserter(data_gun));

        gunstatus.technical_status = QString::fromStdString(data_gun.at(1));
        gunstatus.access_status = QString::fromStdString(data_gun.at(3));
        gunstatus.azimuth = QString::fromStdString(data_gun.at(5));
        gunstatus.elevation = QString::fromStdString(data_gun.at(7));
        gunstatus.gun_ready_status = QString::fromStdString(data_gun.at(9));
        gunstatus.gun_follow_status = QString::fromStdString(data_gun.at(11));
        gunstatus.ready_to_start_status = QString::fromStdString(data_gun.at(13));

//        qDebug() <<"technical_status"<<gunstatus.technical_status
//                 <<"acces_status"<<gunstatus.access_status
//                 <<"ready_to_status"<<gunstatus.ready_to_start_status
//                 <<"gun_status"<<gunstatus.gun_ready_status
//                 <<"gun_follow"<<gunstatus.gun_follow_status
//                 <<"azimuth"<<gunstatus.azimuth
//                 <<"elevation"<<gunstatus.elevation;

        if (gunstatus.technical_status == "1")
        {
            ui->labelGunStatTech->setStyleSheet("color:rgb(0, 255, 0);");
            ui->labelGunStatTech->setText("Online");

            // ==== Access Status ==== //
            if(gunstatus.access_status == "1")
            {
                ui->labelGunStatAccess->setStyleSheet("color:rgb(0, 255, 0);");
                ui->labelGunStatAccess->setText("Remote");

                // ==== Ready To Status ==== //
                if(gunstatus.ready_to_start_status == "1")
                {
                    ui->labelGunStatReadyToStart->setStyleSheet("color:rgb(0, 255, 0);");
                    ui->labelGunStatReadyToStart->setText("Yes");
                }
                else if (gunstatus.ready_to_start_status == "0")
                {
                    ui->labelGunStatReadyToStart->setStyleSheet("color: rgb(255, 0, 0);");
                    ui->labelGunStatReadyToStart->setText("No");
                }

                // ==== Gun Ready ==== //
                if(gunstatus.gun_ready_status == "1")
                {
                    ui->labelGunStatReady->setStyleSheet("color:rgb(0, 255, 0);");
                    ui->labelGunStatReady->setText("Yes");
                }
                else if (gunstatus.gun_ready_status == "0")
                {
                    ui->labelGunStatReady->setStyleSheet("color: rgb(255, 0, 0);");
                    ui->labelGunStatReady->setText("No");
                }

                // ==== Gun Follow ==== //
                if(gunstatus.gun_follow_status == "1")
                {
                    ui->labelGunStatFollow->setStyleSheet("color:rgb(0, 255, 0);");
                    ui->labelGunStatFollow->setText("Yes");
                }
                else if (gunstatus.gun_follow_status == "0")
                {
                    ui->labelGunStatFollow->setStyleSheet("color: rgb(255, 0, 0);");
                    ui->labelGunStatFollow->setText("No");
                }

                if(gunstatus.gun_ready_status == "1")
                {
                    // ==== Azimuth ==== //
                    ui->labelGunStatAz->setText(gunstatus.azimuth);
                    ui->labelGunStatAz->setStyleSheet("color:rgb(0, 255, 0);");
                    ui->labelGunStatEl->setText(gunstatus.elevation);
                    ui->labelGunStatEl->setStyleSheet("color:rgb(0, 255, 0);");

                    // ==== Gun Op Status ===== //
                    try
                    {
                        std::vector<std::string> data_operational;
                        std::string engagement_mode;

                        redisClient->hgetall("gun_op_status",std::back_inserter(data_operational));
                        engagement_mode = redisClient->get("engagement_mode").value();

                        operationalstatus.operational = QString::fromStdString(data_operational.at(1));
                        QString cur_assign_mode = QString::fromStdString(data_operational.at(5));

                        qDebug() <<"operationalstatus"<<operationalstatus.operational <<"cur_assign_mode" <<cur_assign_mode<<operationalstatus.assign_mode;
                        if(operationalstatus.assign_mode != cur_assign_mode)
                        {
                            if(cur_assign_mode != "-")
                            {
                                if(engagement_mode=="Manual")
                                {
                                    std::vector<std::string> data_engagement;
                                    redisClient->hmget("engagement",{"azimuth", "elevation"}, std::back_inserter(data_engagement));
                                    engagementstatus.azimuth = QString::fromStdString(data_engagement.at(0));
                                    engagementstatus.elevation = QString::fromStdString(data_engagement.at(1));
                                    ui->lineEditAz->setText(engagementstatus.azimuth);
                                    ui->lineEditEl->setText(engagementstatus.elevation);
                                    ui->lineEditAz->setEnabled(true);
                                    ui->lineEditEl->setEnabled(true);
                                    ui->pushButtonGunBarControlApply->setEnabled(true);
                                    ui->comboBoxGunBarControlMode->setCurrentIndex(0);
                                }
                                else
                                {
                                    ui->lineEditAz->setText("");
                                    ui->lineEditEl->setText("");
                                    ui->lineEditAz->setDisabled(true);
                                    ui->lineEditEl->setDisabled(true);
                                    ui->pushButtonGunBarControlApply->setDisabled(true);
                                    ui->comboBoxGunBarControlMode->setCurrentIndex(1);
                                }
                                ui->comboBoxGunBarControlMode->setEnabled(true);
                            }
                            else
                            {
                                ui->lineEditAz->setDisabled(true);
                                ui->lineEditEl->setDisabled(true);
                                ui->comboBoxGunBarControlMode->setEnabled(false);
                                ui->pushButtonGunBarControlApply->setDisabled(true);

                                redisClient->set("engagement_mode", "Manual");
                            }
                            operationalstatus.assign_mode = cur_assign_mode;
                        }

//                        qDebug() <<"operationalstatus"<<operationalstatus.operational <<"engagement" <<QString::fromStdString(data_engagement);

                        if (operationalstatus.operational == "Assigned")
                            ui->labelGunStatOp->setStyleSheet("color:rgb(0, 255, 0);");
                        else if (operationalstatus.operational == "Standby")
                            ui->labelGunStatOp->setStyleSheet("color:rgb(255, 255, 255);");
                        else if (operationalstatus.operational.isEmpty())
                            ui->labelGunStatOp->setStyleSheet("color:rgb(0, 0, 0);");

                        ui->labelGunStatOp->setText(operationalstatus.operational);
                    }
                    catch (Error e)
                    {
                        curStatusString = e.what();
                        qDebug() << Q_FUNC_INFO <<  curStatusString;
                    }
                }
                else
                {
                    ui->labelGunStatOp->setText("");
                    ui->labelGunStatAz->setText("");
                    ui->labelGunStatEl->setText("");
                    ui->pushButtonGunBarControlApply->setDisabled(true);
                    ui->comboBoxGunBarControlMode->setDisabled(true);
                    ui->lineEditAz->setDisabled(true);
                    ui->lineEditEl->setDisabled(true);

                    redisClient->set("engagement_mode", "Manual");
                    redisClient->hset("gun_op_status", "assign_mode", "-");
                }
            }
            else
            {
                ui->labelGunStatAccess->setStyleSheet("color: rgb(255, 0, 0);");
                ui->labelGunStatAccess->setText("Local");
                ui->lineEditAz->setDisabled(true);
                ui->lineEditEl->setDisabled(true);
                ui->lineEditAz->setText("");
                ui->lineEditEl->setText("");
                ui->comboBoxGunBarControlMode->setDisabled(true);
                ui->pushButtonGunBarControlApply->setDisabled(true);

                ui->labelGunStatAz->setText("");
                ui->labelGunStatEl->setText("");
                ui->labelGunStatReadyToStart->setText("");
                ui->labelGunStatReady->setText("");
                ui->labelGunStatFollow->setText("");
                ui->labelGunStatOp->setText("");

                redisClient->set("engagement_mode", "Manual");
                redisClient->hset("gun_op_status", "assign_mode", "-");
            }
        }
        else if (gunstatus.technical_status == "0")
        {
            ui->labelGunStatTech->setStyleSheet("color: rgb(255, 0, 0);");
            ui->labelGunStatTech->setText("Offline");

            // ==== Label Kosong ==== //
            ui->labelGunStatAccess->setText("");
            ui->labelGunStatReadyToStart->setText("");
            ui->labelGunStatReady->setText("");
            ui->labelGunStatFollow->setText("");
            ui->labelGunStatAz->setText("");
            ui->labelGunStatEl->setText("");
            ui->labelGunStatOp->setText("");
            ui->lineEditAz->setDisabled(true);
            ui->lineEditEl->setDisabled(true);
            ui->comboBoxGunBarControlMode->setDisabled(true);
            ui->pushButtonGunBarControlApply->setDisabled(true);

            redisClient->set("engagement_mode", "Manual");
            redisClient->hset("gun_op_status", "assign_mode", "-");
        }
    }
    catch (Error e)
    {
        curStatusString = e.what();
        qDebug() << Q_FUNC_INFO <<  curStatusString;
    }
}
#endif
void FrameGun::on_comboBoxGunBarControlMode_activated(const QString &arg1)
{
    redisClient->set("engagement_mode", arg1.toUtf8().constData());
    redisClient->hset("engagement", "azimuth", "0.0");
    redisClient->hset("engagement", "elevation", "0.0");
    redisClient->hset("engagement", "azimuth_corr", "0.0");
    redisClient->hset("engagement", "elevation_corr", "0.0");
    redisClient->hset("engagement", "azimuth_status", "engageable");
    redisClient->hset("engagement", "elevation_status", "engageable");

    if (arg1 == "Manual")
    {
        ui->lineEditAz->setText("0.0");
        ui->lineEditEl->setText("0.0");
        ui->lineEditAz->setDisabled(false);
        ui->lineEditEl->setDisabled(false);
        ui->pushButtonGunBarControlApply->setDisabled(false);
        if(operationalstatus.assign_mode != "-")
            redisClient->hset("gun_op_status", "operational", "Assigned");
    }
    else if (arg1 == "Auto")
    {
        ui->lineEditAz->setText("");
        ui->lineEditEl->setText("");
        ui->lineEditAz->setDisabled(true);
        ui->lineEditEl->setDisabled(true);
        ui->pushButtonGunBarControlApply->setDisabled(true);

        redisClient->del("engagement");
        if(operationalstatus.assign_mode != "-")
            redisClient->hset("gun_op_status", "operational", "Standby");
    }
    //qDebug() << Q_FUNC_INFO <<arg1;
}

void FrameGun::on_pushButtonGunBarControlApply_clicked()
{
    QString azimuth = ui->lineEditAz->text();
    QString elevation = ui->lineEditEl->text();

    bool ok;

    float azimuth_float = azimuth. toFloat(&ok);
    if(!ok)
    {
        QMessageBox::critical(this, "Fatal Error Azimuth", "Invalid input value\nValid input range : -180 to 180" );
        return;
    }
    if ((azimuth_float < -180.0) || (azimuth_float > 180.0) )
    {
        QMessageBox::critical(this, "Fatal Error", "Invalid Azimuth input\nValid input range : -180 to 180" );
        return;
    }

    float elevation_float = elevation. toFloat(&ok);
    if(!ok)
    {
        QMessageBox::critical(this, "Fatal Error Elevation", "Invalid input value\nValid input range : -5 to 80" );
        return;
    }
    qDebug()<<Q_FUNC_INFO<<(elevation_float < -5.0)<<(elevation_float > 80.0);
    if ((elevation_float < -5.0) || (elevation_float > 80.0) )
    {
        QMessageBox::critical(this, "Fatal Error", "Invalid Elevation input\nValid input range : -5 to 80" );
        return;
    }

#ifdef WIN32
    QMap<QString, QVariant> data_map;
    data_map.insert("azimuth", azimuth);
    data_map.insert("elevation", elevation);

    if(redisClient->hmset("engagement",data_map)) curStatusString = "";
    else
    {
        curStatusString = "Cannot set engagement";
        qDebug() << Q_FUNC_INFO <<  curStatusString;
    }
#else
    std::unordered_map<std::string, std::string> data_map =
    {
        {"azimuth", azimuth.toStdString()},
        {"elevation", elevation.toStdString()},
    };

    try
    {
        redisClient->hmset("engagement",data_map.begin(), data_map.end());
        curStatusString = "";
    }
    catch (Error e)
    {
        curStatusString = e.what();
        qDebug() << Q_FUNC_INFO <<  curStatusString;
    }
#endif
}
