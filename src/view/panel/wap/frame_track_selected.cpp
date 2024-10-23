#include "frame_track_selected.h"
#include "src/di/di.h"
#include "src/infra/store/track/track_repository_publisher.h"
#include "ui_frame_track_selected.h"

FrameTrackSelected::FrameTrackSelected(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FrameTrackSelected),
    currentTrackSelected(-1)
{
    ui->setupUi(this);

    arpaRepo = DI::getInstance()->getRepoTrack()->getRepoTrackArpa();

    // add this to listener
    TrackRepositoryPublisher *publisher = dynamic_cast<TrackRepositoryPublisher *>(arpaRepo);
    publisher->AddListener(this);
}

FrameTrackSelected::~FrameTrackSelected()
{
    delete ui;
}


void FrameTrackSelected::OnTracksAdded(std::list<TrackBaseEntity *> tnList)
{
    Q_UNUSED(tnList);
}

void FrameTrackSelected::OnTracksRemoved(std::list<int> tnIdList)
{
    QList<int> tnList(tnIdList.begin(), tnIdList.end());
    foreach (int tn, tnList) {
        if (tn == currentTrackSelected) {
            currentTrackSelected = -1;
            ui->labelTn->setText("-");
            ui->labelRng->setText("-");
            ui->labelBrn->setText("-");
            ui->labelSpd->setText("-");
            ui->labelCrs->setText("-");
            ui->labelIdentity->setText("-");
        }
    }
}

void FrameTrackSelected::OnTrackPropertyChanged(int tn, TrackBaseEntity *track)
{
    if (tn == currentTrackSelected) {
        ui->labelTn->setText(QString::number(track->getId()));
        ui->labelRng->setText(QString::number(track->getRange(), 'f', 2));
        ui->labelBrn->setText(QString::number(track->getBearing(), 'f', 2));
        ui->labelSpd->setText(QString::number(track->getSpeed(), 'f', 2));
        ui->labelCrs->setText(QString::number(track->getCourse(), 'f', 2));
        ui->labelIdentity->setText(TrackUtils::identity2String(track->getCurrIdentity()));
    }
}

void FrameTrackSelected::OnTrackSelectedChanged(int tn)
{
    const TrackBaseEntity *findTrack = arpaRepo->FindOne(tn);
    if (findTrack) {
        currentTrackSelected = tn;

        ui->labelTn->setText(QString::number(findTrack->getId()));
        ui->labelRng->setText(QString::number(findTrack->getRange(), 'f', 2));
        ui->labelBrn->setText(QString::number(findTrack->getBearing(), 'f', 2));
        ui->labelSpd->setText(QString::number(findTrack->getSpeed(), 'f', 2));
        ui->labelCrs->setText(QString::number(findTrack->getCourse(), 'f', 2));
        ui->labelIdentity->setText(TrackUtils::identity2String(findTrack->getCurrIdentity()));
    }
}

void FrameTrackSelected::OnTrackDoubleClicked(int tn)
{
    OnTrackSelectedChanged(tn);
}

void FrameTrackSelected::on_pushButtonClear_clicked()
{
    currentTrackSelected = -1;
    ui->labelTn->setText("-");
    ui->labelRng->setText("-");
    ui->labelBrn->setText("-");
    ui->labelSpd->setText("-");
    ui->labelCrs->setText("-");
    ui->labelIdentity->setText("-");
}

