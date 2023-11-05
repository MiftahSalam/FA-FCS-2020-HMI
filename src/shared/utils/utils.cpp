#include "utils.h"
#include "qjsondocument.h"

#include "src/shared/common/errors/err_json_parse.h"
#include <math.h>

QJsonObject Utils::byteArrayToJsonObject(QByteArray raw)
{
    QJsonParseError parseError;
    QJsonObject obj = QJsonDocument::fromJson(raw,&parseError).object();
    if (parseError.error != QJsonParseError::NoError) throw ErrJsonParse();

    return obj;

}

QString Utils::latDecToStringDegree(const float lat)
{
    // ==== Float latitude ====
    float lat_float = lat;
    QString tanda_latitude;
    if (lat_float < 0)
        tanda_latitude = "S";
    else
       tanda_latitude = "N";

    lat_float = fabs(lat_float);
    float deg = floor(lat_float);
    float min = lat_float - deg ;
          min = min * 60;
          min = floor(min);
    float sec = (lat_float - deg - (min / 60.0)) * 3600.0;

    QString deg_string = QString::number(deg,'f', 0);
    if (deg_string.size() < 2)
        deg_string.prepend("0");

    QString min_string = QString::number(min,'f', 0);
    if (min_string.size() < 2)
        min_string.prepend("0");

    QString sec_string = QString::number(sec, 'f', 0);
    if (sec_string.size() <2)
        sec_string.prepend("0");

    return deg_string + "-" + min_string + "'" + sec_string + "''" + tanda_latitude;
}
