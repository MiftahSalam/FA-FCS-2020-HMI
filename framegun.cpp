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

void FrameGun::setConfig(QString Config)
{
    this->Config = Config;
    qDebug() <<Q_FUNC_INFO<<"Redis config"<<this->Config;

    try
    {
        redisClient = new Redis(this->Config.toStdString());

        redisClient->hset("engagement", "mode", "Manual");
        redisClient->hset("engagement", "azimuth", "0");
        redisClient->hset("engagement", "elevation", "0");

        ui->lineEditAz->setText("0");
        ui->lineEditEl->setText("0");
        ui->lineEditAz->setStyleSheet("color:rgb(255, 255, 255);");
        ui->lineEditEl->setStyleSheet("color:rgb(255, 255, 255);");


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

                    std::vector<std::string> data_operational;
                    std::string data_engagement;
                    try
                    {
                        redisClient->hgetall("gun_op_status",std::back_inserter(data_operational));
                        operationalstatus.operational = QString::fromStdString(data_operational.at(1));

                        data_engagement = redisClient->hget("engagement", "mode").value();

                        if(data_engagement == "Auto")
                        {
                            ui->lineEditAz->setDisabled(true);
                            ui->lineEditEl->setDisabled(true);
                            ui->pushButtonGunBarControlApply->setDisabled(true);
                        }
                        else if(data_engagement == "Manual")
                        {
                            ui->lineEditAz->setEnabled(true);
                            ui->lineEditEl->setEnabled(true);
                            ui->pushButtonGunBarControlApply->setEnabled(true);
                        }

//                        qDebug() <<"operationalstatus"<<operationalstatus.operational <<"engagement" <<QString::fromStdString(data_engagement);

                        if (operationalstatus.operational == "Assigned")
                        {
                            ui->groupBoxGunBarrCon->setEnabled(true);
                            ui->labelGunStatOp->setStyleSheet("color:rgb(0, 255, 0);");
                            ui->labelGunStatOp->setText(operationalstatus.operational);
                        }
                        else if (operationalstatus.operational == "Standby")
                        {
                            ui->groupBoxGunBarrCon->setEnabled(true);
                            ui->labelGunStatOp->setStyleSheet("color:rgb(255, 255, 255);");
                            ui->labelGunStatOp->setText(operationalstatus.operational);
                        }
                        else if (operationalstatus.operational == "-")
                        {
                            ui->groupBoxGunBarrCon->setDisabled(true);
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
                }

            }
            else
            {

                ui->labelGunStatAccess->setStyleSheet("color: rgb(255, 0, 0);");
                ui->labelGunStatAccess->setText("Local");

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
    redisClient->hset("engagement", "mode", arg1.toUtf8().constData());
    redisClient->hset("engagement", "azimuth", "0.0");
    redisClient->hset("engagement", "elevation", "0.0");

    if (arg1 == "Manual")
    {
        ui->lineEditAz->setText("0.0");
        ui->lineEditEl->setText("0.0");
    }
    else if (arg1 == "Auto")
    {
        ui->lineEditAz->setText("");
        ui->lineEditEl->setText("");
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
