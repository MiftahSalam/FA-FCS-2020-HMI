#include "tda_global.h"
#include <math.h>

Identity int2Identity(int identity)
{
    if(identity==0)
        return UNKNOWN;
    else if(identity==1)
        return FRIENDLY;
    else if(identity==2)
        return NEUTRAL;
    else if(identity==3)
        return HOSTILE;
    else if(identity==4)
        return UNIDENTIFY;
}
Environment int2Environment(int env)
{
    if(env==0)
        return AIR;
    else if(env==1)
        return SURFACE;
    else
        return ENVIRONMENT_COUNT;
}
Desig int2Desig(int desig)
{
    if(desig==0)
        return EOT;
    else
        return DESIG_COUNT;
}
DesigDirect int2DesigDirect(int desig_direct)
{
    if(desig_direct==0)
        return M_40;
    else
        return DESIG_DIRECT_COUNT;
}
int identity2Int(Identity identity)
{
    if(identity==UNKNOWN)
        return 0;
    else if(identity==FRIENDLY)
        return 1;
    else if(identity==NEUTRAL)
        return 2;
    else if(identity==HOSTILE)
        return 3;
    else if(identity==UNIDENTIFY)
        return 4;
    else
        return 100;
}
int environment2Int(Environment env)
{
    if(env==AIR)
        return 0;
    else if(env==SURFACE)
        return 1;
    else
        return 100;
}
int desig2Int(Desig desig)
{
    if(desig==EOT)
        return 0;
}
int desigDirect2Int(DesigDirect desig_direct)
{
    if(desig_direct==M_40)
        return 0;
}
trackSource int2TrackSource(int source)
{
    if(source==0)
        return T_NAVRAD;
    else if(source==1)
        return T_LIOD;
    else
        return SOURCE_COUNT;
}
