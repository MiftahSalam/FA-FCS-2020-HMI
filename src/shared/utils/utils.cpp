#include "utils.h"
#include "qjsondocument.h"

#include "src/shared/common/errors/err_json_parse.h"

QJsonObject Utils::byteArrayToJsonObject(QByteArray raw)
{
    QJsonParseError parseError;
    QJsonObject obj = QJsonDocument::fromJson(raw,&parseError).object();
    if (parseError.error != QJsonParseError::NoError) throw ErrJsonParse();

    return obj;

}
