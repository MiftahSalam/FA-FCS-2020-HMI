#include "tda_fire_triangle_object.h"

#include <QTextStream>
#include <cmath>
#include <QDebug>
#include "qmath.h"
#include "src/shared/utils/utils.h"

TDAFireTriangleObject::TDAFireTriangleObject(
        QObject *parent,
        FireTriangleBaseRepository *repoFireTriangle,
        TrackBaseRepository *repoArpa,
        WeaponTrackAssignService *serviceWTA,
        TDAConfig *configTDA
        ):
    TDAObjectBase (parent),
    fireTriangleRepo(repoFireTriangle),
    arpaRepo(repoArpa),
    wtaSercie(serviceWTA),
    tdaConfig(configTDA),
    currAssignedTrack(0)

{
    connect(wtaSercie, &WeaponTrackAssignService::signal_assignmentResponseData,
            this, &TDAFireTriangleObject::OnWeaponAssignment);

    fireTriangleRepo->SetFireTriangle(FireTriangleEntity(60,6820.2734375,7313.2666015625));
}

void TDAFireTriangleObject::Draw(QPainter *painter, const int &side, const int &width, const int &height, const QPoint &off_center)
{
    if (currAssignedTrack > 0) {
        auto track = arpaRepo->FindOne(currAssignedTrack);
        if (track) {

            tdaScale = tdaConfig->getZoomScale();

            QPoint center_point = QPoint(width/2,height/2);
            //        float ttlf = fireTriangleRepo->GetFireTriangle()->getTTLF();
            float ttlf_x = fireTriangleRepo->GetFireTriangle()->getTTLFX();
            float ttlf_y = fireTriangleRepo->GetFireTriangle()->getTTLFY();
            //konversi ke NM
            float ttlf_x_NM = ttlf_x/1852;
            float ttlf_y_NM = ttlf_y/1852;

            int ttlf_x_Pixel = Utils::range2Pixel(ttlf_x_NM, tdaScale, width, height);
            int ttlf_y_Pixel = Utils::range2Pixel(ttlf_y_NM, tdaScale, width, height);
            //        int ttlf_x_Pixel =  (int)(ttlf_x_NM*(side/tdaScale));
            //        int ttlf_y_Pixel = (int)(ttlf_y_NM*(side/tdaScale));

            //posisi track
            //            float range = 3704/1852; // konversi m to NM
            //            int rangePixel = (int)(range*(side/tdaScale));
            //            float bearing = 23;

            float range = track->getRange();
            int rangePixel = (int)(range*(side/tdaScale));
            float bearing = track->getBearing();

            QSize trackIconSize(15,10);
            const double rad2deg = (bearing - 90) * M_PI / 180;
            int track_x_Pixel = (rangePixel * qCos(rad2deg)) + trackIconSize.width();
            int track_y_Pixel = (rangePixel * qSin(rad2deg)) + trackIconSize.height();

            // qDebug()<<"range"<<rangePixel<<"track X"<<track_x_Pixel<<"track Y"<<track_y_Pixel;

            painter->translate(center_point);
            painter->setPen(QColor(30,144,255,255));
            painter->drawLine(0,0, ttlf_x_Pixel, -ttlf_y_Pixel);
            painter->drawLine(ttlf_x_Pixel, -ttlf_y_Pixel, track_x_Pixel, track_y_Pixel);
            painter->drawLine(track_x_Pixel, track_y_Pixel, 0, 0);
            painter->translate(-center_point);
        }
    }
}

void TDAFireTriangleObject::OnWeaponAssignment(BaseResponse<TrackAssignResponse> resp, bool assign)
{
    if (resp.getHttpCode() == 0)
    {
        if (assign) {
            currAssignedTrack = resp.getData().getTrackId();
        } else {
            currAssignedTrack = 0;
        }
    }
}

