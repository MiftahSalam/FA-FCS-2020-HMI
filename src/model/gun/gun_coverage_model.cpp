#include "gun_coverage_model.h"

GunCoverageModel::GunCoverageModel(double max_range, double blind_arc, double _orientation):
    maxRange(max_range), blindArc(blind_arc), orientation(_orientation)
{

}

GunCoverageModel GunCoverageModel::fromJsonObject(QJsonObject obj)
{
    GunCoverageModel model(
        obj.value("max_range").toDouble(),
        obj.value("blind_arc").toDouble(),
        obj.value("orientation").toDouble()
        );
    return model;

}

double GunCoverageModel::getMaxRange() const
{
    return maxRange;
}

double GunCoverageModel::getBlindArc() const
{
    return blindArc;
}

double GunCoverageModel::getOrientation() const
{
    return orientation;
}
