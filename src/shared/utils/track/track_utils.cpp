#include "track_utils.h"

TrackUtils::Identity TrackUtils::int2Identity(int identity)
{
    switch (identity) {
    case 1:
        return FRIENDLY;
        break;
    case 2:
        return NEUTRAL;
        break;
    case 3:
        return HOSTILE;
        break;
    case 4:
        return UNIDENTIFY;
        break;
    default:
        return UNKNOWN;
    }
}

int TrackUtils::identity2Int(Identity identity)
{
    return (int)identity;
}

TrackUtils::Environment TrackUtils::int2Environment(int env)
{

}

int TrackUtils::environment2Int(Environment env)
{

}

TrackUtils::Desig TrackUtils::int2Desig(int desig)
{

}

int TrackUtils::desig2Int(Desig desig)
{

}

TrackUtils::DesigDirect TrackUtils::int2DesigDirect(int desig_direct)
{

}

int TrackUtils::desigDirect2Int(DesigDirect desig_direct)
{

}

TrackUtils::trackSource TrackUtils::int2TrackSource(int source)
{

}

QString TrackUtils::identity2String(TrackUtils::Identity identity)
{
    if(identity==TrackUtils::UNKNOWN)
        return "Unknown";
    else if(identity==TrackUtils::FRIENDLY)
        return "Friend";
    else if(identity==TrackUtils::NEUTRAL)
        return "Neutral";
    else if(identity==TrackUtils::HOSTILE)
        return "Hostile";
    else
        return "Unidentify";
}

QString TrackUtils::env2String(TrackUtils::Environment env)
{
    if(env==TrackUtils::AIR)
        return "Air";
    else if(env==TrackUtils::SURFACE)
        return "Surface";
    else
        return "Unknown Environment";
}

