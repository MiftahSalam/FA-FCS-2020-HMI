#include "hmi_global.h"

#include <string.h>

HMIRedis *HMI_redisInit(QString ip, int port, uint timeout)
{
    HMIRedis *redis = new HMIRedis;

    redis->hostname = ip;
    redis->port = port;
    redis->connect_timeout.tv_sec = timeout;
    redis->connect_timeout.tv_usec = 0;

    return redis;
}

void HMI_redisDeInit(HMIRedis *redis)
{
    redisFree(redis->context);
}

bool HMI_isRedisValid (HMIRedis *redis)
{
    return (redis->context != NULL) && (redis->context->err == REDIS_OK);
}

void HMI_redisConnect(HMIRedis *redis)
{
    redis->context = redisConnectWithTimeout(redis->hostname.toUtf8().constData(),redis->port,redis->connect_timeout);

    if(HMI_isRedisValid(redis))
        redisEnableKeepAlive(redis->context);

}
