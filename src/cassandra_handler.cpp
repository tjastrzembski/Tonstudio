#include "cassandra_handler.h"

Cassandra_Handler *Cassandra_Handler::cassandra;

Cassandra_Handler::~Cassandra_Handler()
{
    auto tmp  = cassandra;
    cassandra = nullptr;
       if(tmp)
           delete tmp;
}

Cassandra_Handler* Cassandra_Handler::instance()
{
    if(!cassandra)
        cassandra = new Cassandra_Handler();
    return cassandra;
}
