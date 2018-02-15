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


bool  MongoDB_Handler::checkForUser(std::string& user)
{
    bool userExists(true);
    //check, if username exists
    return userExists;
}

bool MongoDB_Handler::login(std::string &user, std::string &pw)
{
    std::hash<std::string> h;
    auto hPW = h(pw);

    int aPW = 0;//get value for 'userID'
    authenticated = hPW == aPW;

    return authenticated;

}

bool MongoDB_Handler::registerUser(std::string &user, std::string &pw)
{
    bool userRegistered(false);
    //write new User into mongo

    return userRegistered;

}

bool MongoDB_Handler::saveProjectInfo(ProjectInfo& pInfo)
{
    //put all information of pInfo into mongo
    return false;
}

bool MongoDB_Handler::loadProjectInfo(std::string &pName, ProjectInfo &pInfo)
{
    //save everything in pInfo
    return false;
}
