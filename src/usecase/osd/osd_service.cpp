#include "osd_service.h"

OSDService* OSDService::service = nullptr;

OSDService::OSDService(QObject *parent)
    : QObject{parent}
{

}

OSDService* OSDService::getInstance() {
    if(service == nullptr) {
        service = new OSDService();
    }

    return service;
}

OSDService::~OSDService() {

}
