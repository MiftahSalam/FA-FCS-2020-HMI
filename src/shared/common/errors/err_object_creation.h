#ifndef ERROBJECTCREATION_H
#define ERROBJECTCREATION_H

#include "base_error.h"

#include <QException>

class ErrObjectCreation: public BaseError
{
public:
    ErrObjectCreation();
};

#endif // ERROBJECTCREATION_H
