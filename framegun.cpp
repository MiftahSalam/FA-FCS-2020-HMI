#include "framegun.h"
#include "ui_framegun.h"
#include "global.h"
#include "unistd.h"

#include <QMessageBox>
#include <QDebug>

FrameGun::FrameGun(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::FrameGun)
{
    ui->setupUi(this);

    /*
    ui->labelGunStatTech->setStyleSheet("item:disabled: black;");
    ui->labelGunStatAccess->setStyleSheet("item:disabled: black;");
    ui->labelGunStatReadyToStart->setStyleSheet("item:disabled: black;");
    ui->labelGunStatReady->setStyleSheet("item:disabled: black;");
    ui->labelGunStatFollow->setStyleSheet("item:disabled: black;");
    ui->labelGunStatAz->setStyleSheet("item:disabled: black;");
    ui->labelGunStatEl->setStyleSheet("item:disabled: black;");
    */

    // ==== Label Kosong ==== //
    ui->labelGunStatTech->setText("");
    ui->labelGunStatAccess->setText("");
    ui->labelGunStatReadyToStart->setText("");
    ui->labelGunStatReady->setText("");
    ui->labelGunStatFollow->setText("");
    ui->labelGunStatAz->setText("");
    ui->labelGunStatEl->setText("");
    ui->labelGunStatOp->setText("");
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

void FrameGun::setConfig(QString Config)
{
    this->Config = Config;
    qDebug() <<Q_FUNC_INFO <<"Redis config" <<this->Config;

    std::vector<std::string> gunbalisticdata;

    try
    {
        redisClient = new Redis(this->Config.toStdString());

        redisClient->set("engagement_mode", "Manual");
        redisClient->hset("engagement", "azimuth", "0.0");
        redisClient->hset("engagement", "elevation", "0.0");
        redisClient->hset("engagement", "azimuth_status", "engageable");
        redisClient->hset("engagement", "elevation_status", "engageable");
        redisClient->hset("engagement", "azimuth_corr", "0.0");
        redisClient->hset("engagement", "elevation_corr", "0.0");
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

//        ui->lineEditAz->setStyleSheet("color:rgb(255, 255, 255);");
//        ui->lineEditEl->setStyleSheet("color:rgb(255, 255, 255);");
    }
    catch (Error e)
    {
        splash->showMessage("GUN Setup error\n\nDatabase error: " + QString(e.what()) + "\n\nApplication now will clossing ",Qt::AlignCenter);
        sleep(3);
        splash->finish(this);
        qApp->exit();
    }
}

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
                    std::vector<std::string> data_operational;
                    std::string engagement_mode;
                    try
                    {
                        redisClient->hgetall("gun_op_status",std::back_inserter(data_operational));
                        engagement_mode = redisClient->get("engagement_mode").value();

                        QString cur_op_stat = QString::fromStdString(data_operational.at(1));
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

                                }
                                else
                                {
                                    ui->lineEditAz->setText("");
                                    ui->lineEditEl->setText("");
                                    ui->lineEditAz->setDisabled(true);
                                    ui->lineEditEl->setDisabled(true);
                                    ui->pushButtonGunBarControlApply->setDisabled(true);

                                }
                                ui->comboBoxGunBarControlMode->setEnabled(true);
                            }
                            else
                            {
                                ui->lineEditAz->setDisabled(true);
                                ui->lineEditEl->setDisabled(true);
                                ui->comboBoxGunBarControlMode->setEnabled(false);
                                ui->pushButtonGunBarControlApply->setDisabled(true);
                            }
                            operationalstatus.assign_mode = cur_assign_mode;
                        }
                        operationalstatus.operational = cur_op_stat;

//                        qDebug() <<"operationalstatus"<<operationalstatus.operational <<"engagement" <<QString::fromStdString(data_engagement);

                        if (operationalstatus.operational == "Assigned")
                        {
//                            ui->groupBoxGunBarrCon->setEnabled(true);
                            ui->labelGunStatOp->setStyleSheet("color:rgb(0, 255, 0);");
                            ui->labelGunStatOp->setText(operationalstatus.operational);
                        }
                        else if (operationalstatus.operational == "Standby")
                        {
//                            ui->groupBoxGunBarrCon->setEnabled(true);
                            ui->labelGunStatOp->setStyleSheet("color:rgb(255, 255, 255);");
                            ui->labelGunStatOp->setText(operationalstatus.operational);
                        }
                        else if (operationalstatus.operational.isEmpty())
                        {
//                            ui->groupBoxGunBarrCon->setDisabled(true);
                            ui->labelGunStatOp->setStyleSheet("color:rgb(0, 0, 0);");
                            ui->labelGunStatOp->setText(operationalstatus.operational);
                        }
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
//            ui->groupBoxGunBarrCon->setEnabled(false);
            ui->lineEditAz->setDisabled(true);
            ui->lineEditEl->setDisabled(true);
            ui->comboBoxGunBarControlMode->setDisabled(true);
            ui->pushButtonGunBarControlApply->setDisabled(true);
        }
    }
    catch (Error e)
    {
        curStatusString = e.what();
        qDebug() << Q_FUNC_INFO <<  curStatusString;
    }
}

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
}


