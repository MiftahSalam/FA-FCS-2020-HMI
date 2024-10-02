#ifndef DATETIME_ENTITY_H
#define DATETIME_ENTITY_H

class DateTimeEntity
{
public:
    DateTimeEntity(float date, float time);

    float date() const;
    void setDate(float newDate);
    void time() const;
    void setTime(float newTime);

private:
    float _date;
    float _time;
};

#endif // DATETIME_ENTITY_H
