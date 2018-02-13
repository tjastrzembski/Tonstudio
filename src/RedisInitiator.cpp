#include "RedisInitiator.h"
#include "GeneralConfigs.h"
#include <cpp_redis/cpp_redis>

bool RedisInitiator::initRedis()
{
#if _WIN32
    WSADATA wsaData;
    // Initialize Winsock
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed with error: %d\n", iResult);
        return false;
    }
#endif // _WIN32
    std::cout << "check for Redis availability" << std::endl;
    // check, if redis is available
    try {
        cpp_redis::client rClient;
        rClient.connect(REDIS_HOST, REDIS_PORT);
        rClient.disconnect();
        std::cout << "Redis ready." << std::endl;
    } catch (const std::exception &ex) {
        std::cerr << "Can't connect to Redis: " << ex.what() << std::endl;
        std::cerr << "Make sure, that Redis is Running at " << REDIS_HOST << ":"
                  << REDIS_PORT << std::endl;
        return false;
    }
    return true;
}
