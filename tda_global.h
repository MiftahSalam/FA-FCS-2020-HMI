#ifndef TDA_GLOBAL_H
#define TDA_GLOBAL_H

#include <QString>
#include <QDebug>

struct DBSettings
{
    QString host;
    QString name;
    QString username;
    QString password;
};

enum Identity {
    UNKNOWN = 0,
    FRIENDLY,
    NEUTRAL,
    HOSTILE,
    UNIDENTIFY,

    IDENTITY_COUNT
};

enum Environment {
    AIR = 0,
    SURFACE,

    ENVIRONMENT_COUNT
};
enum Desig {
    EOT = 0,
//    Direct,

    DESIG_COUNT
};
enum DesigDirect {
    M_40 = 0,

    DESIG_DIRECT_COUNT
};
enum trackSource {
    T_NAVRAD = 0,
    T_LIOD,

    SOURCE_COUNT
};

struct trackParam
{
    int tn;
    double range;
    double bearing;
    double speed;
    double course;

    Identity cur_identity;
    trackSource cur_source;
    Environment cur_env;
    QString weapon_assign;
};

extern Identity int2Identity(int identity);
extern int identity2Int(Identity identity);
extern Environment int2Environment(int env);
extern int environment2Int(Environment env);
extern Desig int2Desig(int desig);
extern int desig2Int(Desig desig);
extern DesigDirect int2DesigDirect(int desig_direct);
extern int desigDirect2Int(DesigDirect desig_direct);

extern trackSource int2TrackSource(int source);

extern double latstr2latval(QString lat);
extern double longstr2longval(QString longi);
extern QString readableLatitude(double lat);
extern QString readableLongitude(double longitude);

#endif // TDA_GLOBAL_H
