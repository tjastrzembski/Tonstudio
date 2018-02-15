#ifndef MONGODB_HANDLER_H
#define MONGODB_HANDLER_H

#include <string>

class MongoDB_Handler
        {
        public:
            ~MongoDB_Handler();
            static MongoDB_Handler *instance();

            bool login(std::string& user, std::string& pw);

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
