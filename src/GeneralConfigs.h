#pragma once
#include <string>

#define NUM_CHANNELS (2)
#define SAMPLE_RATE (44100)
#define REDIS_HOST ("127.0.0.1")
#define REDIS_PORT (7000)

enum Operation {
    SUCCESS_ = 0,
    ERROR_ = 1,
};
