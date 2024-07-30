#ifndef FRAMETDA_H
#define FRAMETDA_H

#include <QFrame>
#include <QList>
#include <QTimer>
#include <QPainter>
#include <QStatusBar>
#include <QObject>
#include <QPoint>
#include <QSettings>
#include <QMenu>

#include "src/infra/store/osd/osd_repository.h"
#include "src/view/tda/components/tda_object_base.h"
#include "src/infra/store/gun/gun_repository.h"


// ==== TDA ==== //
namespace Ui {
class FrameTDA;
}

class FrameTDA : public QFrame
{
    Q_OBJECT
public:
    explicit FrameTDA(QWidget *parent = 0);
    ~FrameTDA();

    // QWidget interface
 protected:
    void paintEvent(QPaintEvent *event) override;

private slots:
    void timeOut();

private:
   Ui::FrameTDA *ui;

   QList<TDAObjectBase*> objectItems;
   OSDRepository *osdRepo;
   GunRepository *gunRepo;
   QTimer *timer;

   float currentRange;
};

#endif // FRAMETDA_H
