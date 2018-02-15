#ifndef MONGODB_HANDLER_H
#define MONGODB_HANDLER_H

#include "projectinfo.h"

#include <string>
#include <list>
#include <bsoncxx/document/value.hpp>

using namespace std;

class MongoDB_Handler
        {
        public:
            ~MongoDB_Handler();
            static MongoDB_Handler *instance();

            bool checkForUser(std::string& user);
            bool login(std::string& user, std::string& pw);
            bool registerUser(std::string& user, std::string& pw);

            bool saveProjectInfo(ProjectInfo& info);
            bool loadProjectInfo(std::string& pName, ProjectInfo& info);
        private:
            MongoDB_Handler(){authenticated = false;}
            MongoDB_Handler(const MongoDB_Handler &) {}
            MongoDB_Handler &operator=(const MongoDB_Handler &)
            {
                return *mongoDB;
            }

            static bool authenticated;
            static std::string userID;
            static MongoDB_Handler *mongoDB;

            static const string DATABASE;
            static const string INFO_COLLECTION;
            static const string USER_COLLECTION;

            static bsoncxx::document::value CreateValue(string& name);
            static bsoncxx::document::view CreateView(string& name);
            static bsoncxx::document::value CreateValue(ProjectInfo& info);
            static bsoncxx::document::view CreateView(ProjectInfo& info);

            static bool ExistsInDatabase(string& name);
            static void SaveInDatabase(ProjectInfo& info);
            static void OverrideInDatabase(ProjectInfo& info);
            static bsoncxx::document::value LoadFromDatabase(string& name);
        };
#endif // MONGODB_HANDLER_H
