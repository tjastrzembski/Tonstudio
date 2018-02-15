#include "mongodb_handler.h"

MongoDB_Handler *MongoDB_Handler::mongoDB;
bool MongoDB_Handler::authenticated ;
std::string MongoDB_Handler::userID;

MongoDB_Handler::~MongoDB_Handler()
{
    auto tmp  = mongoDB;
    mongoDB = nullptr;
       if(tmp)
           delete tmp;
}

MongoDB_Handler* MongoDB_Handler::instance()
{
    if(!mongoDB)
        mongoDB = new MongoDB_Handler();
    return mongoDB;
}
