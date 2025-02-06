#ifndef BASEMODEL_H
#define BASEMODEL_H

#include <QByteArray>

class BaseModel
{
public:
    virtual QByteArray toJSON() = 0;
};

#endif // BASEMODEL_H
