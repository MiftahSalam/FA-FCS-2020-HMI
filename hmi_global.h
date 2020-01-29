#ifndef HMI_GLOBAL_H
#define HMI_GLOBAL_H

#include <hiredis/hiredis.h>
#include <QString>

struct HMIRedis {
    redisContext *context;
    redisReply *reply;
    QString hostname;
    int port = 6379;
    struct timeval connect_timeout;
};

HMIRedis *HMI_redisInit(QString ip = "127.0.0.1", int port = 6379, uint timeout = 100);
void HMI_redisDeInit(HMIRedis *redis);
void HMI_redisConnect(HMIRedis *redis);
bool HMI_isRedisValid (HMIRedis *redis);
int HMI_RedisExecute (HMIRedis *redis, const char *command);

#endif // HMI_GLOBAL_H
