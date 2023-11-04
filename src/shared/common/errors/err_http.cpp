#include "err_http.h"

ErrHttpUnknown::ErrHttpUnknown(): BaseError(ERROR_CODE_HTTP_RESPONSE_UNKNOWN.first, ERROR_CODE_HTTP_RESPONSE_UNKNOWN.second)
{

}

ErrHttpConnRefused::ErrHttpConnRefused(): BaseError(ERROR_CODE_HTTP_RESPONSE_CONNECTION_REFUSED.first, ERROR_CODE_HTTP_RESPONSE_CONNECTION_REFUSED.second)
{

}
