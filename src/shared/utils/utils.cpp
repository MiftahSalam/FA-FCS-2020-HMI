#include "utils.h"
#include "qjsondocument.h"

#include "src/shared/common/errors/err_json_parse.h"
#include "src/shared/common/errors/err_osd_data.h"
#include <math.h>

QJsonObject Utils::byteArrayToJsonObject(QByteArray raw)
{
    QJsonParseError parseError;
    QJsonObject obj = QJsonDocument::fromJson(raw,&parseError).object();
    if (parseError.error != QJsonParseError::NoError) throw ErrJsonParse();

    return obj;

}

QString Utils::lonDecToStringDegree(const float lon)
{
    // ==== Float Longitude ====
    float long_float = lon;
    QString tanda_longitude;
    if (long_float < 0)
        tanda_longitude = "W";
    else
        tanda_longitude = "E";

    long_float = fabs(long_float);
    float degg = floor(long_float);
    float minn = long_float - degg ;
    minn = minn * 60;
    minn = floor(minn);
    float secc = (long_float - degg - (minn / 60.0)) * 3600.0;

    //               qDebug() << degg << long_float << gpsdata.longitude << minn << secc << tanda_longitude;

    QString degg_string = QString::number(degg,'f', 0);
    if (degg_string.size() < 3)
        degg_string.prepend("0");
    if (degg_string.size() < 3)
        degg_string.prepend("0");

    QString minn_string = QString::number(minn,'f', 0);
    if (minn_string.size() < 2)
        minn_string.prepend("0");

    QString secc_string = QString::number(secc, 'f', 0);
    if (secc_string.size() <2)
        secc_string.prepend("0");

    return degg_string + "-" + minn_string + "'" + secc_string + "''" + tanda_longitude;
}

float Utils::lonStrToDegree(const QString lon)
{
    QString long_str_trimmed = lon;
    long_str_trimmed.remove(" ");

    QString str1 = long_str_trimmed;
    QStringList long_list1 = str1.split("-");

    if(long_list1.size()<2)
    {
        throw ErrOSDDataInvalidFormat();
    }

    QString degg = long_list1.at(0);
    QStringList long_list2 = long_list1.at(1).split("'");

    if(long_list2.size()!=4)
    {
        throw ErrOSDDataInvalidFormat();
    }

    QString minn = long_list2.at(0);
    QString secc = long_list2.at(1);
    QString signn = long_list2.at(3);

//    qDebug() <<degg  <<minn <<secc <<signn;

    bool ok;
    float valuedegg = degg.toFloat(&ok);
    if (!ok)
    {
        throw ErrOSDDataConversion();
    }

    float valueminn = minn.toFloat()/60.0;
    if ((!ok) || (valueminn >= 1))
    {
        throw ErrOSDDataOutOfRange();
    }

    float valuesecc = secc.toFloat()/3600.0;
    if ((!ok) || (valuesecc > (1.0/60.0)))
    {
        throw ErrOSDDataOutOfRange();
    }

    float valueLong = valuedegg+valueminn+valuesecc;

    if(signn == "W")
        valueLong *= -1.0;
    else if ((signn != "W") && (signn != "E"))
    {
        throw ErrOSDDataInvalidFormat();
    }

    if ((valueLong < -180) || (valueLong > 180) )
    {
        throw ErrOSDDataOutOfRange();
    }

    return valueLong;

    //    qDebug() << Q_FUNC_INFO<<valuedegg << valueminn <<valuesecc <<valueLong;

    
}

int Utils::range2Pixel(double range, double max_range, int vp_width, int vp_height)
{
    int side = qMin(vp_width, vp_height) / 2;
    return static_cast<int>(range*(side/(max_range)));
}

double Utils::pixel2Range(int pixel, double max_range, int vp_width, int vp_height)
{
    int side = qMin(vp_width, vp_height) / 2;
//    qDebug()<<pixel<<tdaScale<<width();
    return max_range*pixel/side;
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

float Utils::latStrToDegree(const QString lat)
{
    QString lat_str_trimmed = lat;
    lat_str_trimmed.remove(" ");

    QString str = lat_str_trimmed;
    QStringList list1 = str.split("-");

    if(list1.size()<2)
    {
        throw ErrOSDDataInvalidFormat();
    }

    QString deg = list1.at(0);
    QStringList list2 = list1.at(1).split("'");

    if(list2.size()!=4)
    {
        throw ErrOSDDataInvalidFormat();
    }

    QString min = list2.at(0);
    QString sec = list2.at(1);
    QString sign = list2.at(3);

//    qDebug() << deg  <<min <<sec <<sign;

    bool ok;
    float valuedeg = deg.toFloat(&ok);
    if (!ok)
    {
        throw ErrOSDDataConversion();
    }

    float valuemin = min.toFloat(&ok)/60.0;
    if ((!ok) || (valuemin >= 1))
    {
        throw ErrOSDDataOutOfRange();
    }

    float valuesec = sec.toFloat(&ok)/3600.0;
    if ((!ok) || (valuesec > (1.0/60.0)))
    {
        throw ErrOSDDataOutOfRange();
    }

    float valueLat = valuedeg+valuemin+valuesec;

    if(sign == "S")
        valueLat *= -1.0;
    else if((sign != "S") && (sign != "N"))
    {
        throw ErrOSDDataInvalidFormat();
    }

    if ((valueLat < -90) || (valueLat > 90) )
    {
        throw ErrOSDDataOutOfRange();
    }

//    qDebug() << Q_FUNC_INFO<<valuedeg << valuemin <<valuesec <<valueLat;

    return valueLat;
}
