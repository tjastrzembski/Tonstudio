#ifndef MONGODB_HANDLER_H
#define MONGODB_HANDLER_H

#include <string>
#include "projectinfo.h"

class MongoDB_Handler
        {
        public:
            ~MongoDB_Handler();
            static MongoDB_Handler *instance();

            bool checkForUser(std::string& user);
            bool login(std::string& user, std::string& pw);
            bool registerUser(std::string& user, std::string& pw);

            bool saveProjectInfo(ProjectInfo& pInfo);
            bool loadProjectInfo(std::string& pName, ProjectInfo& pInfo);
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

        };
#endif // MONGODB_HANDLER_H
