#ifndef OSDCMS_H
#define OSDCMS_H

#include <QObject>

#include "src/model/osd/cms/osd_set_position_reqeust.h"

class IOSDCMS {
public:
    virtual void setPosition(const OSDSetPositionReqeust request) = 0;
};

class OSDCMS : public QObject, public IOSDCMS
{
    Q_OBJECT
public:
    explicit OSDCMS(QObject *parent = nullptr);

signals:


    // IOSDCMS interface
public:
    void setPosition(const OSDSetPositionReqeust request) override;
};

#endif // OSDCMS_H
