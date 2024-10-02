#ifndef DATETIME_MODEL_H
#define DATETIME_MODEL_H

#include <QJsonObject>

class DateTimeModel
{
public:
    DateTimeModel(double date, double time);

    static DateTimeModel FromJsonObject(QJsonObject obj);

    double getDate() const;
    double getTime() const;

private:
    double _date, _time;
};

#endif // DATETIME_MODEL_H
