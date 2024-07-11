#ifndef TRACKUTILS_H
#define TRACKUTILS_H


class TrackUtils
{
public:
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

    static Identity int2Identity(int identity);
    static int identity2Int(Identity identity);
    static Environment int2Environment(int env);
    static int environment2Int(Environment env);
    static Desig int2Desig(int desig);
    static int desig2Int(Desig desig);
    static DesigDirect int2DesigDirect(int desig_direct);
    static int desigDirect2Int(DesigDirect desig_direct);
    static trackSource int2TrackSource(int source);

};

#endif // TRACKUTILS_H
