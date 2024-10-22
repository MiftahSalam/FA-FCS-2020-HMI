#include "frame_track_engage.h"
#include "src/di/di.h"
#include "src/infra/store/track/track_repository_publisher.h"
#include "ui_frame_track_engage.h"

#include <QMessageBox>


#ifdef USE_LOG4QT
#include <log4qt/logger.h>
LOG4QT_DECLARE_STATIC_LOGGER(logger, FrameTrackEngage)
#else
#include <QDebug>
#endif

FrameTrackEngage::FrameTrackEngage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FrameTrackEngage)
{
    ui->setupUi(this);

    setupDI();

    // add this to listener
    TrackBaseRepository *arpaRepo = DI::getInstance()->getRepoTrack()->getRepoTrackArpa();
    TrackRepositoryPublisher *publisher = dynamic_cast<TrackRepositoryPublisher *>(arpaRepo);
    publisher->AddListener(this);

    currentGunMode = _gunService->getBarrelMode();

    initWeaponList();
    resetMode();
}

FrameTrackEngage::~FrameTrackEngage()
{
    delete ui;
}

//{"id": 3,"range": 3.3,"bearing": 4.5,"bearing_type": "R","speed": 2.1,"course": 2.3}{"id": 4,"range": 2.3,"bearing": 40.5,"bearing_type": "R","speed": 2.1,"course": 2.3}{"id": 1,"range": 0.3,"bearing": 40.5,"bearing_type": "R","speed": 2.1,"course": 2.3}{"id": 2,"range": 0.3,"bearing": 240.5,"bearing_type": "R","speed": 2.1,"course": 2.3}
void FrameTrackEngage::OnTracksAdded(std::list<TrackBaseEntity *> tnList)
{
    QList<TrackBaseEntity*> track_list(tnList.begin(), tnList.end());
    QStringList trIds;
    foreach (auto tr, track_list) {
        trIds.append(QString::number(tr->getId()));
    }

    if (!trIds.isEmpty()) {
        ui->comboBoxTrackEngTN->insertItems(ui->comboBoxTrackEngTN->count(), trIds);
    }
    ui->comboBoxTrackEngTN->model()->sort(0);
}

void FrameTrackEngage::OnTracksRemoved(std::list<int> tnIdList)
{
    QList<int> track_list(tnIdList.begin(), tnIdList.end());
    for (int var = 0; var < track_list.size(); var++) {
        int idx = ui->comboBoxTrackEngTN->findText(QString::number(track_list.at(var)));
        if (idx > 0) {
            _wtaService->BreakEngagementTrack(track_list.at(var));
            ui->comboBoxTrackEngTN->removeItem(idx);
        }
    }
}

void FrameTrackEngage::OnTrackPropertyChanged(int tn, TrackBaseEntity *track)
{
    Q_UNUSED(tn);
    Q_UNUSED(track);
}

void FrameTrackEngage::OnTrackSelectedChanged(int tn)
{
    Q_UNUSED(tn);
}

void FrameTrackEngage::resetMode()
{
    QString weapon = ui->comboBoxTrackEngWeapon->currentText();
    if (weapon != "-") {
        _wtaService->BreakEngagementWeapon(weapon);
    }

    availableUiSetup(false);

    disconnect(ui->comboBoxTrackEngTN, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &FrameTrackEngage::onComboBoxTrackEngTNChange);
    disconnect(ui->comboBoxTrackEngWeapon, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &FrameTrackEngage::onComboBoxTrackEngWeaponChange);

    ui->comboBoxTrackEngTN->setCurrentIndex(0);
    ui->comboBoxTrackEngWeapon->setCurrentIndex(0);
    ui->pushButtonTrackEngAssign->setText("Assign/Break");

    connect(ui->comboBoxTrackEngTN, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &FrameTrackEngage::onComboBoxTrackEngTNChange);
    connect(ui->comboBoxTrackEngWeapon, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &FrameTrackEngage::onComboBoxTrackEngWeaponChange);
}

void FrameTrackEngage::setupDI()
{
    _waService = DI::getInstance()->getServiceWeaponAssign();
    _wtaService = DI::getInstance()->getServiceWeaponTrackAssign();
    _gunService = DI::getInstance()->getServiceGunManager();

    connect(_gunService, &GunManagerService::OnBarrelModeCheck, this, &FrameTrackEngage::onGunCheck);
    connect(_waService, &WeaponAssignService::OnAssignModeChange, this, &FrameTrackEngage::onAssignModeChange);
    connect(_wtaService, &WeaponTrackAssignService::signal_assignmentResponseData,
            this, &FrameTrackEngage::onAssignmentResponseData);
}

void FrameTrackEngage::availableUiSetup(bool available)
{
    ui->comboBoxTrackEngTN->setEnabled(available);
    ui->comboBoxTrackEngWeapon->setEnabled(available);
    ui->pushButtonTrackEngAssign->setEnabled(available);
}

void FrameTrackEngage::noWeaponUI()
{
    disconnect(ui->comboBoxTrackEngWeapon, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &FrameTrackEngage::onComboBoxTrackEngWeaponChange);
    ui->comboBoxTrackEngTN->setEnabled(false);
    ui->comboBoxTrackEngTN->setCurrentIndex(0);
    ui->pushButtonTrackEngAssign->setEnabled(false);
    ui->pushButtonTrackEngAssign->setText("Assign/Break");
    connect(ui->comboBoxTrackEngWeapon, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &FrameTrackEngage::onComboBoxTrackEngWeaponChange);
}

void FrameTrackEngage::initWeaponList()
{
    QStringList weapons = _waService->getAvailableWeapons();
    for (int var = 0; var < weapons.size(); var++) {
        ui->comboBoxTrackEngWeapon->insertItem(var+1, weapons.at(var));
    }
}

void FrameTrackEngage::on_pushButtonTrackEngAssign_clicked()
{
    const QString weapon = ui->comboBoxTrackEngWeapon->currentText();
    const int tn = ui->comboBoxTrackEngTN->currentText().toInt();

    if (weapon != "-" && tn > 0) {
        if (!_wtaService->IsEngage(weapon, tn)) {
            try {
                _wtaService->SetEngagement(weapon, tn);
            }  catch (BaseError &e) {
                QMessageBox::warning(this, "Warning Track Assign Control", QString("Failed to assign track with error: %1").arg(e.getMessage()));
            }
        } else {
            _wtaService->BreakEngagement(weapon, tn);
        }
    }
}

void FrameTrackEngage::onComboBoxTrackEngTNChange(int index)
{
    if (index > 0) {
        const QString weapon = ui->comboBoxTrackEngWeapon->currentText();
        const int tn = ui->comboBoxTrackEngTN->currentText().toInt();
        if (!_wtaService->IsWeaponEngaged(weapon)) {
            ui->pushButtonTrackEngAssign->setText("Assign");
            ui->pushButtonTrackEngAssign->setEnabled(true);
        } else {
            if(_wtaService->IsEngage(weapon, tn)) {
                ui->pushButtonTrackEngAssign->setText("Break");
                ui->pushButtonTrackEngAssign->setEnabled(true);
            } else {
                ui->pushButtonTrackEngAssign->setText("Assign/Break");
                ui->pushButtonTrackEngAssign->setEnabled(false);
            }
        }
    } else {
        ui->pushButtonTrackEngAssign->setText("Assign/Break");
        ui->pushButtonTrackEngAssign->setEnabled(false);
    }
}

void FrameTrackEngage::onComboBoxTrackEngWeaponChange(int index)
{
    if (index > 0) {
        auto wa = _waService->GetAssignment(ui->comboBoxTrackEngWeapon->currentText());
        auto wa_mode = wa->getMode();

        disconnect(ui->comboBoxTrackEngTN, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &FrameTrackEngage::onComboBoxTrackEngTNChange);
        ui->comboBoxTrackEngTN->setCurrentIndex(0);
        connect(ui->comboBoxTrackEngTN, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &FrameTrackEngage::onComboBoxTrackEngTNChange);
        ui->comboBoxTrackEngTN->setEnabled(wa_mode == WeaponAssign::DIRECT);
    } else {
        noWeaponUI();
    }
}

void FrameTrackEngage::onGunCheck()
{
    auto curMode = _gunService->getBarrelMode();
    if ((GunBarrelModeEntity::MODE)curMode != currentGunMode)
    {
        switch (curMode) {
        case GunBarrelModeEntity::NONE:
        case GunBarrelModeEntity::MANUAL:
            resetMode();
            break;
        case GunBarrelModeEntity::AUTO:
            availableUiSetup(true);
            onComboBoxTrackEngWeaponChange(0);
            break;
        default:
            break;
        }

        currentGunMode = curMode;
    }

    _wtaService->CheckUpdateEngagement(ui->comboBoxTrackEngWeapon->currentText());
}

void FrameTrackEngage::onAssignModeChange(const QString &weapon, const WeaponAssign::WeaponAssignMode &mode)
{
    if (ui->comboBoxTrackEngWeapon->currentText() == weapon) {
        disconnect(ui->comboBoxTrackEngTN, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &FrameTrackEngage::onComboBoxTrackEngTNChange);

        ui->comboBoxTrackEngTN->setCurrentIndex(0);
        ui->comboBoxTrackEngTN->setEnabled(mode == WeaponAssign::DIRECT);
        ui->pushButtonTrackEngAssign->setEnabled(mode == WeaponAssign::DIRECT);

        _wtaService->BreakEngagementWeapon(weapon);
        ui->pushButtonTrackEngAssign->setEnabled(false);
        ui->pushButtonTrackEngAssign->setText("Assign/Break");

        connect(ui->comboBoxTrackEngTN, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &FrameTrackEngage::onComboBoxTrackEngTNChange);
    }
}

void FrameTrackEngage::onAssignmentResponseData(BaseResponse<TrackAssignResponse> resp, bool assign)
{
#ifdef USE_LOG4QT
    logger()->debug() << Q_FUNC_INFO << " -> resp code: " << resp.getHttpCode()
                      << ", resp msg: " << resp.getMessage();
#else
    qDebug() << Q_FUNC_INFO << "resp code:" << resp.getHttpCode() << "resp msg:" << resp.getMessage();
#endif

    if (resp.getHttpCode() != 0)
    {
        if (assign) {
            QMessageBox::warning(this, "Warning Track Assign Control", QString("Failed to assign track with error: %1").arg(resp.getMessage()));
        } else {
            QMessageBox::warning(this, "Warning Track Assign Control", QString("Failed to break track with error: %1").arg(resp.getMessage()));
        }

        return;
    }

    if (assign) {
        ui->pushButtonTrackEngAssign->setText("Break");
    } else {
        ui->pushButtonTrackEngAssign->setText("Assign");
    }
}

