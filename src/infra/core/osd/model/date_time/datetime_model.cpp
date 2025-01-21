#include "datetime_model.h"
#include <string>

DateTimeModel::DateTimeModel(const std::string &dateTimeLocal, const std::string dateTimeUTC):
    _dateTimeLocal(dateTimeLocal), _dateTimeUTC(dateTimeUTC)
{

}

DateTimeModel DateTimeModel::FromJsonObject(QJsonObject obj)
{
    DateTimeModel model(
        obj.value("date_time_local").toString().toStdString(),
        obj.value("date_time_utc").toString().toStdString()
        );
    return model;
}

std::string DateTimeModel::getDateTimeLocal() const
{
    return _dateTimeLocal;
}

std::string DateTimeModel::getDateTimeUTC() const
{
    return _dateTimeUTC;
}

