#ifndef DATE_TIME_STREAM_MODEL_H
#define DATE_TIME_STREAM_MODEL_H

#include <QJsonObject>

#include "src/infra/core/osd/model/osd_base_model.h"

class DateTimeStreamModel: public OSDBaseModel
{
public:
    DateTimeStreamModel();
    DateTimeStreamModel(OSDBaseModel base_data, QString local, QString utc);

    static DateTimeStreamModel fromJsonObject(QJsonObject obj);

    QString getDateTimeLocal() const;
    QString getDateTimeUTC() const;

private:
    QString _dateTimeLocal, _dateTimeUTC;
};

#endif // DATE_TIME_STREAM_MODEL_H
