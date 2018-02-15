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

bool Cassandra_Handler::saveSoundComponent(std::vector<std::pair<std::string, std::string> > *sounds)
{
    return true;
}

bool Cassandra_Handler::loadSoundComponent(std::string &pName, std::vector<std::vector<std::pair<std::string, std::string> > *> &sounds)
{
    //for(all entries

    return true;
}
