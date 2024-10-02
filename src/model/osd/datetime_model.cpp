#include "datetime_model.h"

DateTimeModel::DateTimeModel(double date, double time):
    _date(date), _time(time)
{

}

DateTimeModel DateTimeModel::FromJsonObject(QJsonObject obj)
{
    DateTimeModel model(
        obj.value("date").toDouble(),
        obj.value("time").toDouble()
        );
    return model;
}

double DateTimeModel::getDate() const
{
    return _date;
}

double DateTimeModel::getTime() const
{
    return _time;
}
