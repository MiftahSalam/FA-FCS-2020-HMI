#ifndef BASEMODEL_H
#define BASEMODEL_H

#include <QString>

template <typename T> class BaseModel
{
public:
    virtual QByteArray toJSON() = 0;
};

#endif // BASEMODEL_H
