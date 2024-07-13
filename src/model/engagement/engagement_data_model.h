#ifndef ENGAGEMENTDATAMODEL_H
#define ENGAGEMENTDATAMODEL_H

#include <QJsonObject>
#include <string>

class EngagementDataModel
{
public:
    EngagementDataModel();
    EngagementDataModel(const std::string &weapon, float azimuth, float elevation, int status);

    static EngagementDataModel fromJsonObject(QJsonObject obj);

    std::string weapon() const;
    float azimuth() const;
    float elevation() const;
    int getStatus() const;

private:
    std::string _weapon;
    float _azimuth;
    float _elevation;
    int status;
};

#endif // ENGAGEMENTDATAMODEL_H
