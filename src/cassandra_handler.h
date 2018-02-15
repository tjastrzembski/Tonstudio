#ifndef CASSANDRA_HANDLER_H
#define CASSANDRA_HANDLER_H

#include <string>
#include <vector>
#include <cassandra.h>

class Cassandra_Handler
{
public:
    ~Cassandra_Handler();
    static Cassandra_Handler *instance();

    void initCassandra();
    void terminateCassandra();

    bool saveSoundComponent(std::vector<std::pair<std::string, std::string>>* sounds );
    bool loadSoundComponent(std::string& pName,std::vector<std::vector<std::pair<std::string, std::string>>*>& sounds);

private:
    Cassandra_Handler() {}
    Cassandra_Handler(const Cassandra_Handler &) {}
    Cassandra_Handler &operator=(const Cassandra_Handler &)
    {
        return *cassandra;
    }

    static Cassandra_Handler *cassandra;

    static CassCluster *cluster;
    static CassSession *session;
    static CassFuture *connect_future;

};
#endif // CASSANDRA_HANDLER_H
