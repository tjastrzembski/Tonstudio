#include "cassandra_handler.h"
#include "GeneralConfigs.h"
#include <iostream>

Cassandra_Handler *Cassandra_Handler::cassandra;
CassCluster *Cassandra_Handler::cluster;
CassSession *Cassandra_Handler::session;
CassFuture *Cassandra_Handler::connect_future;

Cassandra_Handler::~Cassandra_Handler()
{
    auto tmp  = cassandra;
    cassandra = nullptr;
       if(tmp)
           delete tmp;
}

void Cassandra_Handler::initCassandra()
{
    std::cout << "init Cassandra" << std::endl;
    // Setup and connect to cluster
    connect_future = nullptr;
    cluster = cass_cluster_new();
    session = cass_session_new();
    // Add contact points
    cass_cluster_set_contact_points(cluster, "127.0.0.1");

    // Provide the cluster object as configuration to connect the session
    connect_future = cass_session_connect(session, cluster);

    // This operation will block until the result is ready
    CassError rc = cass_future_error_code(connect_future);

    std::cout << "Connect result: " << cass_error_desc(rc) << std::endl;

}

void Cassandra_Handler::terminateCassandra()
{
    cass_future_free(connect_future);
    cass_session_free(session);
    cass_cluster_free(cluster);
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
