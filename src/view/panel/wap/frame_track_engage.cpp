#include "frame_track_engage.h"
#include "ui_frame_track_engage.h"

FrameTrackEngage::FrameTrackEngage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FrameTrackEngage)
{
    ui->setupUi(this);

    resetMode();
}

FrameTrackEngage::~FrameTrackEngage()
{
    delete ui;
}

void FrameTrackEngage::resetMode()
{
    availableUiSetup(false);

    disconnect(ui->comboBoxTrackEngTN, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &FrameTrackEngage::onComboBoxTrackEngTNChange);
    disconnect(ui->comboBoxTrackEngWeapon, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &FrameTrackEngage::onComboBoxTrackEngWeaponChange);

    ui->comboBoxTrackEngTN->setCurrentIndex(0);
    ui->comboBoxTrackEngWeapon->setCurrentIndex(0);

    connect(ui->comboBoxTrackEngTN, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &FrameTrackEngage::onComboBoxTrackEngTNChange);
    connect(ui->comboBoxTrackEngWeapon, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &FrameTrackEngage::onComboBoxTrackEngWeaponChange);
}

void FrameTrackEngage::availableUiSetup(bool available)
{
    ui->comboBoxTrackEngTN->setEnabled(available);
    ui->comboBoxTrackEngWeapon->setEnabled(available);
    ui->pushButtonTrackEngAssign->setEnabled(available);
}

void FrameTrackEngage::on_pushButtonTrackEngAssign_clicked()
{

}

void FrameTrackEngage::onComboBoxTrackEngTNChange(int index)
{

}

void FrameTrackEngage::onComboBoxTrackEngWeaponChange(int index)
{

}

