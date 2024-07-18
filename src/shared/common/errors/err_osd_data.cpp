#include "err_osd_data.h"

ErrOSDDataInvalidFormat::ErrOSDDataInvalidFormat(): BaseError(ERROR_CODE_OSD_DATA_INVALID_FORMAT.first, ERROR_CODE_OSD_DATA_INVALID_FORMAT.second)
{

}

ErrOSDDataConversion::ErrOSDDataConversion(): BaseError(ERROR_CODE_OSD_DATA_CONVERSION.first, ERROR_CODE_OSD_DATA_CONVERSION.second)
{

}

ErrOSDDataOutOfRange::ErrOSDDataOutOfRange(): BaseError(ERROR_CODE_OSD_DATA_OUT_OF_RANGE.first, ERROR_CODE_OSD_DATA_OUT_OF_RANGE.second)
{

}

ErrOSDDataPartiallyInvalid::ErrOSDDataPartiallyInvalid(): BaseError(ERROR_CODE_OSD_DATA_PARTIALLY_INVALID.first, ERROR_CODE_OSD_DATA_PARTIALLY_INVALID.second)
{

}

ErrOSDDataInvalid::ErrOSDDataInvalid(): BaseError(ERROR_CODE_OSD_DATA_PARTIALLY_INVALID.first, ERROR_CODE_OSD_DATA_PARTIALLY_INVALID.second)
{

}
