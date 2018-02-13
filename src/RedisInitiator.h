#ifndef REDISINITIATOR_H
#define REDISINITIATOR_H

class RedisInitiator
{
public:
    RedisInitiator() = default;
    ~RedisInitiator() = default;

    static bool initRedis();
};

#endif // REDISINITIATOR_H
