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

#include "src/view/tda/components/tda_object_base.h"

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
   QTimer *timer;

   float currentRange;
};

#endif // FRAMETDA_H
