#ifndef OSD_SERVICE_H
#define OSD_SERVICE_H

#include <QObject>

class OSDService : public QObject
{
    Q_OBJECT
public:
    OSDService(OSDService &other) = delete;
    void operator=(const OSDService&) = delete;
    ~OSDService() override;

    static OSDService* getInstance();

protected:
    OSDService(QObject *parent = nullptr);

signals:

private:
    static OSDService *service;
};

#endif // OSD_SERVICE_H
