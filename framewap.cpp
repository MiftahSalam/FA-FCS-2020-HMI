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

}

void FrameWAP::setAccessStatus(QString access_status)
{
    currentAccessStatus = access_status;

    QString currentWapWeapon = ui->comboBoxWAPWeapon->currentText();
    QString currentWapMode = ui->comboBoxWAPMode->currentText();
    QString currentTrackEngWeapon = ui->comboBoxTrackEngWeapon->currentText();

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

            qDebug() <<Q_FUNC_INFO << "track:Data:*"<<trackListTn.size() ;

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

                qDebug() <<Q_FUNC_INFO << trackTn;
            }
        }
        catch(Error e)
        {
            qDebug() << Q_FUNC_INFO << e.what();

        }
    }


    //qDebug() <<Q_FUNC_INFO <<QString::fromStdString(trackTn);

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
