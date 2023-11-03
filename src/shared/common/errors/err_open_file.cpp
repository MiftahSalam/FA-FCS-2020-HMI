#include "err_open_file.h"

ErrFileNotFound::ErrFileNotFound(): BaseError(ERROR_CODE_FILE_NOT_FOUND.first, ERROR_CODE_FILE_NOT_FOUND.second)
{}

ErrFileCannotOpened::ErrFileCannotOpened(): BaseError(ERROR_CODE_FILE_CANNOT_BE_OPENED.first, ERROR_CODE_FILE_CANNOT_BE_OPENED.second)
{}
