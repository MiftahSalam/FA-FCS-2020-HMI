#include "framewap.h"
#include "ui_framewap.h"
#include "global.h"
#include "unistd.h"
#include "frametda.h"

#include <QMessageBox>
#include <QDebug>
#include <QRegExp>
#include <QRegularExpressionValidator>
#include <algorithm>
#include <iterator>
#include <vector>
#include <iostream>
#include<bits/stdc++.h>


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
        //        redisGun->hset("gun_op_status","assign_mode","-");
        redisGun->hset("gun_op_status","operational","");
        redisGun->hset("engagement", "azimuth", "0.0");
        redisGun->hset("engagement", "elevation", "0.0");
        redisGun->hset("engagement", "azimuth_status", "engageable");
        redisGun->hset("engagement", "elevation_status", "engageable");
        redisGun->hset("engagement", "azimuth_corr", "0.0");
        redisGun->hset("engagement", "elevation_corr", "0.0");
    }
    catch (Error e)
    {
        qDebug() << Q_FUNC_INFO <<e.what();
    }

    QTableWidgetItem *corr_status_Az = ui->tableWidgetCorrection->item(0,1);
    corr_status_Az->setText("-");
    QTableWidgetItem *corr_status_El = ui->tableWidgetCorrection->item(0,2);
    corr_status_El->setText("-");
}

void FrameWAP::setAccessStatus(QString access_status)
{
    currentAccessStatus = access_status;

    //    qDebug() <<Q_FUNC_INFO << "access_status"<<access_status;
    if (access_status == "1")
    {
        ui->groupBoxWAP->setEnabled(true);
        ui->groupBoxEng->setEnabled(true);
        ui->groupBoxTrackEng->setEnabled(true);
        ui->groupBoxCorr->setEnabled(true);
    }
    else if(access_status == "0")
    {
        ui->comboBoxWAPMode->setCurrentIndex(0);
        ui->pushButtonTrackEngAssign->setText("Assign/Break");

        ui->groupBoxWAP->setEnabled(false);
        ui->groupBoxEng->setEnabled(false);
        ui->groupBoxTrackEng->setEnabled(false);
        ui->groupBoxCorr->setEnabled(false);

        try
        {
            std::vector<std::string> trackListTn;

            redisTrack->keys("track:Data:*",std::back_inserter(trackListTn));
            for (int i=0;i<trackListTn.size();i++)
            {
                QString trackTn = QString::fromStdString(redisTrack->hget(trackListTn.at(i).data(), "id").value());
                redisTrack->hset(trackTn.toStdString(),"weapon_assigned","");
            }
        }
        catch(Error e)
        {
            qDebug() << Q_FUNC_INFO << "reset weapon assign. no track" << e.what();
        }

        // ==== Engagement Data  ==== //
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
    }


    try
    {
        std::vector<std::string> trackListTn;
        std::vector<std::string> track_id_identity;
        std::vector<std::string> filteredTrack;

        redisTrack->keys("track:Data:*",std::back_inserter(trackListTn));

        for (int i = 0; i < trackListTn.size(); i++)
        {
            try
            {
                redisTrack->hmget(trackListTn.at(i).data(), {"id","identity"}, std::back_inserter(track_id_identity));
            }
            catch (Error e)
            {
                qDebug() << Q_FUNC_INFO <<  "cannot get id, identity: "<<e.what();
            }

            std::string id = track_id_identity.size() > 1 ? track_id_identity.at(0) : "0";
            std::string identity = track_id_identity.size() > 1 ? track_id_identity.at(1) : "0";

            if (identity != "1" && identity != "2")
            {
                filteredTrack.push_back(id);

                int tn = QString::fromStdString(id).toInt();
                if(!tnList.contains(tn))
                {
                    tnList.append(tn);
                    qDebug()<<"new tnList" <<tnList.last();
                    ui->comboBoxTrackEngTN->addItem(QString::fromStdString(id));
                }
            }
            track_id_identity.clear();
        }
        qDebug()<<"filteredTrack.size()" <<filteredTrack.size()<<"tnList.size"<<tnList.size();

        if (filteredTrack.size() < tnList.size())
        {
            QList<int> tnListToDelete = tnList;

            for (int j = 0; j< filteredTrack.size(); j++)
            {
                tnListToDelete.removeOne(QString::fromStdString(filteredTrack.at(j)).toInt());
            }
//            qDebug()<<"tn list delete ="<<tnListToDelete;

            for (int i =0; i < tnListToDelete.size(); i++)
            {
                if(tnList.contains(tnListToDelete.at(i)))
                {
//                    qDebug()<< "proses remove"<<tnListToDelete.at(i);
                    int curIdxToDel = ui->comboBoxTrackEngTN->findText(QString::number(tnListToDelete.at(i)));
                    if(ui->comboBoxTrackEngTN->currentIndex()==curIdxToDel)
                    {
                        if(ui->pushButtonTrackEngAssign->text() == "Break")
                            ui->pushButtonTrackEngAssign->setText("Assign");
                    }
                    ui->comboBoxTrackEngTN->removeItem(curIdxToDel);
                    tnList.removeOne(tnListToDelete.at(i));
                }
            }
        }

        if(trackListTn.size() < 1)
        {
            ui->pushButtonTrackEngAssign->setText("Assign/Break");
            ui->pushButtonTrackEngAssign->setEnabled(false);
            ui->comboBoxTrackEngTN->clear();
        }
    }
    catch(Error e)
    {
        qDebug() << Q_FUNC_INFO << "update available track. no track" << e.what();
    }

    // ==== Engagement Data ==== //
    try
    {
        QString engage_mode = QString::fromStdString(redisGun->get("engagement_mode").value());

        if(engage_mode == "Auto")
        {
            // ==== Engage Tn ==== //
            QString engagetrackTnWeapon;
            std::vector<std::string> trackTn;

            try
            {
                redisTrack->keys("track:Data:*",std::back_inserter(trackTn));
                for (int i=0;i<trackTn.size();i++)
                {
                    std::vector<std::string> engageTnWeapon;

                    try
                    {
                        redisTrack->hmget(trackTn.at(i).data(),
                        {"id","weapon_assigned"},std::back_inserter(engageTnWeapon));

                        if(QString::fromStdString(engageTnWeapon.at(1)) == "40 mm")
                        {
                            engagetrackTnWeapon = QString::fromStdString(engageTnWeapon.at(0));
                        }
                    }
                    catch(Error e)
                    {
                        qDebug() << Q_FUNC_INFO << "weapon_assigned has not set"<<e.what();
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

                //                ui->groupBoxCorr->setEnabled(true);
                //                ui->groupBoxEng->setEnabled(true);

                // ==== Engage, El_status, & Az_status ==== //
                try
                {
                    std::vector<std::string> engagement;
                    redisGun->hmget("engagement",{"azimuth_status", "elevation_status"},std::back_inserter (engagement));
                    if((QString::fromStdString(engagement.at(1)) == "engageable") &&
                            (QString::fromStdString(engagement.at(0)) == "engageable"))
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
                catch(Error e)
                {
                    qDebug() << Q_FUNC_INFO <<"cannot get engagement status"<< e.what();
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
            }
            catch(Error e)
            {
                qDebug() << Q_FUNC_INFO <<"no track"<< e.what();
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
        }
        else if(engage_mode == "Manual")
        {
            ui->pushButtonTrackEngAssign->setText("Assign/Break");

            ui->groupBoxEng->setEnabled(false);
            ui->groupBoxTrackEng->setEnabled(false);
            ui->groupBoxCorr->setEnabled(false);

            try
            {
                std::vector<std::string> trackListTn;

                redisTrack->keys("track:Data:*",std::back_inserter(trackListTn));
                for (int i=0;i<trackListTn.size();i++)
                {
                    //                    qDebug() << Q_FUNC_INFO << "track" << trackListTn.at(i).data();
                    redisTrack->hset(trackListTn.at(i).data(),"weapon_assigned","");
                }
            }
            catch(Error e)
            {
                qDebug() << Q_FUNC_INFO << "reset weapon assign. no track" << e.what();
            }

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
        }
        //    qDebug() <<Q_FUNC_INFO << "hasil" <<access_status;
        //    qDebug() <<Q_FUNC_INFO << "wap current weapon" <<currentWapWeapon;
        //    qDebug() <<Q_FUNC_INFO << "wap current mode" <<currentWapMode;
        //    qDebug() <<Q_FUNC_INFO << "wap current weapon assign" <<currentTrackEngWeapon;
    }
    catch(Error e)
    {
        qDebug() << Q_FUNC_INFO << "cannot get engagement_mode"<<e.what();
    }
}
void FrameWAP::on_comboBoxWAPWeapon_activated(const QString &arg1)
{
    qDebug() <<Q_FUNC_INFO << arg1;
}

void FrameWAP::on_comboBoxTrackEngTN_activated(const QString &arg1)
{
    QString currentTrackAssign;

    std::vector<std::string> trackListTn;

    try
    {
        redisTrack->keys("track:Data:*",std::back_inserter(trackListTn));

        qDebug() <<Q_FUNC_INFO << "track:Data:*"<<trackListTn.size() ;

        QString cb = QString("track:Data:")+ui->comboBoxTrackEngTN->currentText();
        qDebug()<<"Data yang di pilih"<< cb;


        for (int i=0;i<trackListTn.size();i++)
        {
            if (std::find(trackListTn.begin(), trackListTn.end(), cb.toUtf8().constData()) != trackListTn.end())
            {
                qDebug()<<"data ditemukan";

            }
            else {
                qDebug()<<"data tidak ditemukan";
                ui->comboBoxTrackEngTN->removeItem(cb.toInt());
                ui->comboBoxTrackEngTN->clearFocus();
                ui->pushButtonTrackEngAssign->setEnabled(true);
                redisTrack->del(cb.toUtf8().constData());
            }

            qDebug()<<"track Lits"<<trackListTn[i].data();

            try
            {
                std::vector<std::string> trackTnWeapon;

                redisTrack->hmget(trackListTn.at(i).data(),
                {"id","weapon_assigned"},std::back_inserter(trackTnWeapon));


                if(QString::fromStdString(trackTnWeapon.at(1)) == "40 mm")
                {
                    currentTrackAssign = QString::fromStdString(trackTnWeapon.at(0));
                    qDebug() <<Q_FUNC_INFO <<"curren"<< currentTrackAssign <<QString::fromStdString(trackTnWeapon.at(1));
                }
            }
            catch(Error e)
            {
                qDebug() << Q_FUNC_INFO << "weapon_assigne has not set"<<e.what();
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

    qDebug() << Q_FUNC_INFO <<"trackkey" << trackKey << "assign"<< assign;

    std::vector<std::string> trackListTn;
    redisTrack->keys("track:Data:*",std::back_inserter(trackListTn));

    for (int i=0;i<trackListTn.size();i++)
    {
        if (std::find(trackListTn.begin(), trackListTn.end(), trackKey.toUtf8().constData()) != trackListTn.end())
        {
            //            qDebug()<<"data ditemukan";

            try
            {
                redisTrack->hset(trackKey.toStdString(),"weapon_assigned",assign.toStdString());
                ui->pushButtonTrackEngAssign->setText(assign.isEmpty() ? "Assign" : "Break");

                QString wap_mode = QString::fromStdString(redisGun->hget("gun_op_status", "assign_mode1").value());
                if(wap_mode != "-")
                {
                    QString engage_mode = QString::fromStdString(redisGun->get("engagement_mode").value());
                    if(engage_mode == "Auto")
                    {
                        if(assign.isEmpty())
                            redisGun->hset("gun_op_status","operational","Standby");
                        else
                            redisGun->hset("gun_op_status","operational","Assigned");
                    }
                }
                else
                    redisGun->hset("gun_op_status","operational","-");
            }
            catch (Error e)
            {
                qDebug() << Q_FUNC_INFO << "cannot set weapon assign" << e.what();
            }
        }
        else {
            qDebug()<<"data tidak ditemukan";
            ui->comboBoxTrackEngTN->removeItem(trackKey.toInt());
            ui->comboBoxTrackEngTN->clearFocus();
            ui->pushButtonTrackEngAssign->setText("Unavailable Track");
            ui->pushButtonTrackEngAssign->setEnabled(false);
            redisTrack->del(trackKey.toUtf8().constData());
        }
    }
}

void FrameWAP::on_comboBoxWAPMode_activated(const QString &arg1)
{
    QString wap_mode = arg1;
    /*
    std::vector<std::string> trackTnList;

    try
    {
        redisTrack->keys("track:Data:*",std::back_inserter(trackTnList));
        for (int i=0;i<trackTnList.size();i++)
        {
            std::string engageTnWeapon;

            try
            {
                engageTnWeapon = redisTrack->hget(trackTnList.at(i).data(), "weapon_assigned").value();

                if(QString::fromStdString(engageTnWeapon) == "40 mm")
                {
                    redisTrack->hset(trackTnList.at(i).data(),"weapon_assigned","");
                }
            }
            catch(Error e)
            {
                qDebug() << Q_FUNC_INFO << "no weapon_assigned has set"<<e.what();
            }
        }

    }
    catch(Error e)
    {
        qDebug() << Q_FUNC_INFO << "no track"<<e.what();
    }
    */
    on_pushButtonTrackEngAssign_clicked();
    try
    {
        qDebug() << Q_FUNC_INFO << wap_mode ;
        redisGun->hset("gun_op_status","assign_mode1",wap_mode.toStdString());
        /**/
        if(wap_mode != "-")
        {
            QString engage_mode = QString::fromStdString(redisGun->get("engagement_mode").value());
            if(engage_mode == "Manual")
            {
                try
                {
                    redisGun->hset("gun_op_status","operational","Assigned");
                    std::unordered_map<std::string, std::string> data_map =
                    {
                        {"azimuth", "0.0"},
                        {"elevation", "0.0"},
                        {"azimuth_status", "engageable"},
                        {"elevation_status", "engageable"},
                    };
                    redisGun->hmset("engagement",data_map.begin(), data_map.end());
                    curStatusString = "";
                }
                catch (Error e)
                {
                    curStatusString = e.what();
                    qDebug() << Q_FUNC_INFO <<  curStatusString;
                }
            }
            else if(engage_mode == "Auto")
            {
                redisGun->hset("gun_op_status","operational","Standby");
            }
        }
        else
        {
            redisGun->hset("gun_op_status","operational","");
        }

        qDebug()<<Q_FUNC_INFO<<"wap mode ="<<wap_mode;
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

    int pos = 0;

    float azimuth_float = corr_az. toFloat(&ok);
    QRegularExpression re("[A-Z]\\d [a-z]+$");
    QRegularExpressionValidator v(re, 0);

    re.setPattern("[A-Z]\\d [a-z]+$");
    v.setRegularExpression(re);

    if(!ok)
    {
        QMessageBox::critical(this, "Fatal Error Azimuth", "Invalid input value\nValid input number\nValid range : -20 to 20" );
        ui->tableWidgetCorrection->item(0,1)->setText("-");
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
        QMessageBox::critical(this, "Fatal Error Elevation", "Invalid input value\nValid input number\nValid range : -10 to 10" );
        ui->tableWidgetCorrection->item(0,2)->setText("-");

        return;
    }

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

