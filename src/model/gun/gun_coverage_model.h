#ifndef GUN_COVERAGE_MODEL_H
#define GUN_COVERAGE_MODEL_H

#include <QJsonObject>

class GunCoverageModel
{
public:
    GunCoverageModel(double max_range, double blind_arc, double _orientation);
    GunCoverageModel(){}

    static GunCoverageModel fromJsonObject(QJsonObject obj);

    double getMaxRange() const;
    double getBlindArc() const;
    double getOrientation() const;

private:
    double maxRange, blindArc, orientation;
};

#endif // GUN_COVERAGE_MODEL_H
