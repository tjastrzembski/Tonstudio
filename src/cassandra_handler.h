#ifndef CASSANDRA_HANDLER_H
#define CASSANDRA_HANDLER_H

#include <string>

class Cassandra_Handler
{
public:
    ~Cassandra_Handler();
    static Cassandra_Handler *instance();

private:
    Cassandra_Handler() {}
    Cassandra_Handler(const Cassandra_Handler &) {}
    Cassandra_Handler &operator=(const Cassandra_Handler &)
    {
        return *cassandra;
    }

    static Cassandra_Handler *cassandra;

};
#endif // CASSANDRA_HANDLER_H
