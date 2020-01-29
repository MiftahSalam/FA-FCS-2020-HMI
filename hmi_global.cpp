#include "hmi_global.h"

#include <string.h>

int HMI_RedisExecute(HMIRedis *redis, const char *command)
{
    if(!HMI_isRedisValid(redis))
        return 1; //redis context error

    redis->reply = (redisReply*)redisCommand(redis->context,command);

    if(redis->reply == NULL)
        return 2; //redis reply error

    return 0;
}

HMIRedis *HMI_redisInit(QString ip, int port, uint timeout)
{
    HMIRedis *redis = new HMIRedis;

    redis->hostname = ip;
    redis->port = port;
    redis->connect_timeout.tv_sec = 0;
    redis->connect_timeout.tv_usec = 1000*timeout;

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
