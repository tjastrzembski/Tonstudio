#ifndef GENERALCONFIGS_H
#define GENERALCONFIGS_H

#include <string>

#define NUM_CHANNELS (2)
#define SAMPLE_RATE (44100)
#define REDIS_HOST ("127.0.0.1")
#define REDIS_PORT (7000)

#define CASSANDRA_HOST ("localhost")
#define CASSANDRA_PORT ( 32774 )

enum Operation {
    SUCCESS_ = 0,
    ERROR_ = 1,
};
#endif // GENERALCONFIGS_H
