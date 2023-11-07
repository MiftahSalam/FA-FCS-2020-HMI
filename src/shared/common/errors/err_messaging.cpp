#include "err_messaging.h"

ErrMessagingTcpConfig::ErrMessagingTcpConfig(): BaseError(ERROR_CODE_MESSAGING_INVALID_TCP_CONFIG.first, ERROR_CODE_MESSAGING_INVALID_TCP_CONFIG.second)
{

}

ErrMessagingDataInvalidFormat::ErrMessagingDataInvalidFormat(): BaseError(ERROR_CODE_MESSAGING_DATA_INVALID_FORMAT.first, ERROR_CODE_MESSAGING_DATA_INVALID_FORMAT.second)
{

}

ErrMessagingDataNoData::ErrMessagingDataNoData(): BaseError(ERROR_CODE_MESSAGING_NO_DATA.first, ERROR_CODE_MESSAGING_NO_DATA.second)
{

}

ErrMessagingNotConnected::ErrMessagingNotConnected(): BaseError(ERROR_CODE_MESSAGING_NOT_CONNECTED.first, ERROR_CODE_MESSAGING_NOT_CONNECTED.second)
{

}
