#ifndef DATETIME_MODEL_H
#define DATETIME_MODEL_H

#include <QJsonObject>

class DateTimeModel
{
public:
    DateTimeModel(const std::string &dateTimeLocal,  const std::string dateTimeUTC);

    static DateTimeModel FromJsonObject(QJsonObject obj);

    std::string getDateTimeLocal() const;
    std::string getDateTimeUTC() const;

private:
    std::string _dateTimeLocal, _dateTimeUTC;
};

#endif // DATETIME_MODEL_H
