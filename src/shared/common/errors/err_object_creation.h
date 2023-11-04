#ifndef ERROBJECTCREATION_H
#define ERROBJECTCREATION_H

#include "err_base.h"

#include <QException>

class ErrObjectCreation: public BaseError
{
public:
    ErrObjectCreation();
};

#endif // ERROBJECTCREATION_H
