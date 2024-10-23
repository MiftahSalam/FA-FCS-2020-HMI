#ifndef FRAME_TRACK_SELECTED_H
#define FRAME_TRACK_SELECTED_H

#include "src/domain/track/repository/track_base_repository.h"
#include "src/infra/store/track/track_repository_listener.h"
#include <QWidget>

namespace Ui {
class FrameTrackSelected;
}

class FrameTrackSelected : public QWidget, public TrackRepositoryListener
{
    Q_OBJECT

public:
    explicit FrameTrackSelected(QWidget *parent = nullptr);
    ~FrameTrackSelected();

    // TrackRepositoryListener interface
    void OnTracksAdded(std::list<TrackBaseEntity *> tnList) override;
    void OnTracksRemoved(std::list<int> tnIdList) override;
    void OnTrackPropertyChanged(int tn, TrackBaseEntity *track) override;
    void OnTrackSelectedChanged(int tn) override;

private slots:
    void on_pushButtonClear_clicked();

private:
    Ui::FrameTrackSelected *ui;
    TrackBaseRepository *arpaRepo;

    int currentTrackSelected;

};

#endif // FRAME_TRACK_SELECTED_H
