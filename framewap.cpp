#include "framewap.h"
#include "ui_framewap.h"
#include "global.h"
#include "unistd.h"

#include <QMessageBox>
#include <QDebug>


FrameWAP::FrameWAP(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::FrameWAP)
{
    ui->setupUi(this);

    ui->tableWidgetCorrection->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidgetEngData->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

FrameWAP::~FrameWAP()
{
    delete ui;
}

void FrameWAP::setConfig(QString ConfigTrack, QString ConfigGun)
{
    this->ConfigTrack = ConfigTrack;
    this->ConfigGun = ConfigGun;
    qDebug() <<Q_FUNC_INFO <<"Redis configTrack" <<this->ConfigTrack <<"Redis configGun" <<this->ConfigGun;

    try
    {
        redisTrack = new Redis(this->ConfigTrack.toStdString());
        redisGun = new Redis(this->ConfigGun.toStdString());
    }
    catch (Error e)
    {
        splash->showMessage("WAP Setup error\n\nDatabase error: " + QString(e.what()) + "\n\nApplication now will clossing ",Qt::AlignCenter);
        sleep(3);
        splash->finish(this);
        qApp->exit();
    }

    try
    {
        redisGun->hset("gun_op_status","assign_mode","");
    }
    catch (Error e)
    {
        qDebug() << Q_FUNC_INFO << "Gun redis error" <<e.what();
    }

    redisGun->hset("engagement","azimuth_corr","0.0");
    redisGun->hset("engagement","elevation_corr","0.0");

    QTableWidgetItem *corr_status_Az = ui->tableWidgetCorrection->item(0,1);
    corr_status_Az->setText("-");
    QTableWidgetItem *corr_status_El = ui->tableWidgetCorrection->item(0,2);
    corr_status_El->setText("-");
}

void FrameWAP::setAccessStatus(QString access_status)
{
    currentAccessStatus = access_status;

    if (access_status == "1")
    {
        ui->comboBoxWAPMode->setEnabled(true);
        ui->comboBoxWAPWeapon->setEnabled(true);
    }
    else if(access_status == "0")
    {
        ui->comboBoxWAPMode->setDisabled(true);
        ui->comboBoxWAPWeapon->setDisabled(true);
    }

    if(!ui->comboBoxTrackEngTN->hasFocus())
    {
        try
        {
            std::vector<std::string> trackListTn;

            redisTrack->keys("track:Data:*",std::back_inserter(trackListTn));

//            qDebug() <<Q_FUNC_INFO << "track:Data:*"<<trackListTn.size() ;

            if(trackListTn.size() <1 )
            {
                ui->pushButtonTrackEngAssign->setText("Assign/Break");
                ui->pushButtonTrackEngAssign->setEnabled(false);
            }

            for (int i=0;i<trackListTn.size();i++)
            {
                QString trackTn = QString::fromStdString(redisTrack->hget(trackListTn.at(i).data(), "id").value());

                if(ui->comboBoxTrackEngTN->findText(trackTn) < 0)
                    ui->comboBoxTrackEngTN->addItem(trackTn);

//                qDebug() <<Q_FUNC_INFO << trackTn;
            }
        }
        catch(Error e)
        {
            qDebug() << Q_FUNC_INFO << e.what();

        }
    }

    // ==== Engagement Data ==== //
    std::vector<std::string> engagement;
    redisGun->hmget("engagement",{"azimuth_status", "elevation_status"},std::back_inserter (engagement));

    QString engage_mode = QString::fromStdString(redisGun->hget("engagement", "mode").value());

    // ==== Engage Azimuth, Elevation, & Mode ==== //
    if(engage_mode != curEngStatus)
    {
        QTableWidgetItem *engage_corr_itemAz = ui->tableWidgetCorrection->item(0,1);
        engage_corr_itemAz->setText("0.0");
        QTableWidgetItem *engage_corr_itemEl = ui->tableWidgetCorrection->item(0,2);
        engage_corr_itemEl->setText("0.0");

        curEngStatus = engage_mode;
    }

    if(engage_mode == "Auto")
    {
        // ==== Engage Tn ==== //
        QString engagetrackTnWeapon;
        std::vector<std::string> trackTn;
        redisTrack->keys("track:Data:*",std::back_inserter(trackTn));

        for (int i=0;i<trackTn.size();i++)
        {
            std::vector<std::string> engageTnWeapon;

            redisTrack->hmget(trackTn.at(i).data(),
            {"id","weapon_assigned"},std::back_inserter(engageTnWeapon));

            if(QString::fromStdString(engageTnWeapon.at(1)) == "40 mm")
            {
                engagetrackTnWeapon = QString::fromStdString(engageTnWeapon.at(0));
            }
        }

        QString engage_az = QString::fromStdString(redisGun->hget("engagement", "azimuth").value());
        QString engage_el = QString::fromStdString(redisGun->hget("engagement", "elevation").value());

        QTableWidgetItem *engage_status_itemTn = ui->tableWidgetEngData->item(0,2);
        engage_status_itemTn->setText(engagetrackTnWeapon);
        QTableWidgetItem *engage_status_itemEl = ui->tableWidgetEngData->item(0,4);
        engage_status_itemEl->setText(engage_el);
        QTableWidgetItem *engage_status_itemAz = ui->tableWidgetEngData->item(0,3);
        engage_status_itemAz->setText(engage_az);

        ui->groupBoxCorr->setEnabled(true);
        ui->groupBoxEng->setEnabled(true);

        // ==== Engage, El_status, & Az_status ==== //
        if((QString::fromStdString(engagement.at(1)) == "Eng") &&
           (QString::fromStdString(engagement.at(0)) == "Eng"))
        {
            QTableWidgetItem *engage_status_itemElStatus = ui->tableWidgetEngData->item(0,1);
            engage_status_itemElStatus->setText("Eng");
        }
        else
        {
            QTableWidgetItem *engage_status_itemElStatus = ui->tableWidgetEngData->item(0,1);
            engage_status_itemElStatus->setText("Not Eng");
        }
    }
    else if(engage_mode == "Manual")
    {
        QTableWidgetItem *engage_status_itemElStatus = ui->tableWidgetEngData->item(0,1);
        engage_status_itemElStatus->setText("-");
        QTableWidgetItem *engage_status_itemTn = ui->tableWidgetEngData->item(0,2);
        engage_status_itemTn->setText("-");
        QTableWidgetItem *engage_status_itemEl = ui->tableWidgetEngData->item(0,4);
        engage_status_itemEl->setText("-");
        QTableWidgetItem *engage_status_itemAz = ui->tableWidgetEngData->item(0,3);
        engage_status_itemAz->setText("-");

        // ==== Correction ==== //
        QTableWidgetItem *corr_status_Az = ui->tableWidgetCorrection->item(0,1);
        corr_status_Az->setText("-");
        QTableWidgetItem *corr_status_El = ui->tableWidgetCorrection->item(0,2);
        corr_status_El->setText("-");
        ui->groupBoxCorr->setDisabled(true);
        ui->groupBoxEng->setDisabled(true);
    }


//    qDebug() <<Q_FUNC_INFO << "hasil" <<access_status;
//    qDebug() <<Q_FUNC_INFO << "wap current weapon" <<currentWapWeapon;
//    qDebug() <<Q_FUNC_INFO << "wap current mode" <<currentWapMode;
//    qDebug() <<Q_FUNC_INFO << "wap current weapon assign" <<currentTrackEngWeapon;

}
void FrameWAP::on_comboBoxWAPWeapon_activated(const QString &arg1)
{

    qDebug() <<Q_FUNC_INFO << arg1;
}

void FrameWAP::on_comboBoxTrackEngTN_activated(const QString &arg1)
{
    QString currentTrackAssign;
    try
    {
        std::vector<std::string> trackListTn;

        redisTrack->keys("track:Data:*",std::back_inserter(trackListTn));

        qDebug() <<Q_FUNC_INFO << "track:Data:*"<<trackListTn.size() ;

        for (int i=0;i<trackListTn.size();i++)
        {
            try
            {
                std::vector<std::string> trackTnWeapon;

                redisTrack->hmget(trackListTn.at(i).data(),
                {"id","weapon_assigned"},std::back_inserter(trackTnWeapon));

                if(QString::fromStdString(trackTnWeapon.at(1)) == "40 mm")
                {
                    currentTrackAssign = QString::fromStdString(trackTnWeapon.at(0));
                    qDebug() <<Q_FUNC_INFO << currentTrackAssign
                        <<QString::fromStdString(trackTnWeapon.at(1));
                }
            }
            catch(Error e)
            {
                qDebug() << Q_FUNC_INFO << e.what();

            }
        }

        if(currentTrackAssign.isEmpty())
        {
            ui->pushButtonTrackEngAssign->setEnabled(true);
            ui->pushButtonTrackEngAssign->setText("Assign");
        }
        else
        {
            if(currentTrackAssign == arg1)
            {
                ui->pushButtonTrackEngAssign->setEnabled(true);
                ui->pushButtonTrackEngAssign->setText("Break");
            }
            else
            {
                ui->pushButtonTrackEngAssign->setEnabled(false);
                ui->pushButtonTrackEngAssign->setText("Assign/Break");
            }
        }
    }
    catch(Error e)
    {
        qDebug() << Q_FUNC_INFO << e.what();

    }

}

void FrameWAP::on_pushButtonTrackEngAssign_clicked()
{
    QString trackKey = QString("track:Data:")+ui->comboBoxTrackEngTN->currentText();
    QString assign = ui->pushButtonTrackEngAssign->text() == "Assign" ? "40 mm" : "";

    qDebug() << Q_FUNC_INFO << trackKey << assign;

    try
    {
        redisTrack->hset(trackKey.toStdString(),"weapon_assigned",assign.toStdString());

        ui->pushButtonTrackEngAssign->setText(assign.isEmpty() ? "Assign" : "Break");
    }
    catch (Error e)
    {
        qDebug() << Q_FUNC_INFO << e.what();
    }

}

void FrameWAP::on_comboBoxWAPMode_activated(const QString &arg1)
{
    QString wap_mode = arg1 == "-" ? "" : arg1;

    try
    {
        redisGun->hset("gun_op_status","assign_mode",wap_mode.toStdString());
    }
    catch (Error e)
    {
        qDebug() << Q_FUNC_INFO << e.what();
    }

}

void FrameWAP::on_pushButtonCorrectionApply_clicked()
{
    QString corr_az = ui->tableWidgetCorrection->item(0,1)->text();
    QString corr_el = ui->tableWidgetCorrection->item(0,2)->text();

    bool ok;

    float azimuth_float = corr_az. toFloat(&ok);
    if(!ok)
    {
        QMessageBox::critical(this, "Fatal Error Azimuth", "Invalid input value\nValid input range : -20 to 20" );
        return;
    }
    if ((azimuth_float < -20.0) || (azimuth_float > 20.0) )
    {
        QMessageBox::critical(this, "Fatal Error", "Invalid Azimuth input\nValid input range : -20 to 20" );
        return;
    }

    float elevation_float = corr_el. toFloat(&ok);
    if(!ok)
    {
        QMessageBox::critical(this, "Fatal Error Elevation", "Invalid input value\nValid input range : -10 to 10" );
        return;
    }
//
    qDebug()<<Q_FUNC_INFO<<(elevation_float < -5.0)<<(elevation_float > 80.0);
    if ((elevation_float < -10.0) || (elevation_float > 10.0) )
    {
        QMessageBox::critical(this, "Fatal Error", "Invalid Elevation input\nValid input range : -10 to 10" );
        return;
    }

    std::unordered_map<std::string, std::string> data_map =
    {
        {"azimuth_corr", corr_az.toStdString()},
        {"elevation_corr", corr_el.toStdString()},
    };

    try
    {
        redisGun->hmset("engagement",data_map.begin(), data_map.end());
        curStatusString = "";
    }
    catch (Error e)
    {
        curStatusString = e.what();
        qDebug() << Q_FUNC_INFO <<  curStatusString;
    }
}
