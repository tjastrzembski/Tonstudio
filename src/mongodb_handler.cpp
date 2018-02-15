#include "mongodb_handler.h"

#include <iostream>
#include <sstream>
#include <vector>

#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/json.hpp>
#include <mongocxx/client.hpp>

using mongocxx::client;
using mongocxx::uri;

const std::string MongoDB_Handler::DATABASE = "recording_studio";
const std::string MongoDB_Handler::INFO_COLLECTION = "project";
const std::string MongoDB_Handler::USER_COLLECTION = "user";

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


bool MongoDB_Handler::checkForUser(std::string& user)
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

bool MongoDB_Handler::saveProjectInfo(ProjectInfo& info)
{
    try {
        if (!MongoDB_Handler::ExistsInDatabase(info.m_ProjectName)) {
            this->SaveInDatabase(info);
        } else {
            this->OverrideInDatabase(info);
        }

        return true;
    } catch (const std::exception& e) {
        return false;
    }
}

bool MongoDB_Handler::loadProjectInfo(std::string &name, ProjectInfo &info)
{
    try {
        auto project_value = MongoDB_Handler::LoadFromDatabase(name);
        std::cout << bsoncxx::to_json(project_value) << std::endl;
        auto project_view = project_value.view();

        info.m_ProjectName = name;
        info.composer = project_view["composer"].get_utf8().value.to_string();
        auto contributors_array = bsoncxx::array::view{
                project_view["contributors"].get_array().value};
        for (auto contributor : contributors_array) {
            info.contributors.push_front(contributor.get_utf8().value.to_string());
        }

        return true;
    } catch (const std::exception& e) {
        return false;
    }
}

void MongoDB_Handler::SaveInDatabase(ProjectInfo &info) {
    auto project_value = CreateValue(info);

    client client{uri{}};
    auto projects_collection = client[DATABASE][INFO_COLLECTION];

    std::cout << "Save project in database " << bsoncxx::to_json(project_value) << std::endl;
    projects_collection.insert_one(project_value.view());
}

void MongoDB_Handler::OverrideInDatabase(ProjectInfo &info) {
    auto search_pattern = MongoDB_Handler::CreateValue(info.m_ProjectName);
    auto new_value = MongoDB_Handler::CreateValue(info);

    client client{uri{}};
    auto projects_collection = client[DATABASE][INFO_COLLECTION];

    std::cout << "Search pattern: " << bsoncxx::to_json(search_pattern) << std::endl;
    std::cout << "New value: " << bsoncxx::to_json(new_value) << std::endl;

    projects_collection.find_one_and_replace(search_pattern.view(), new_value.view());
}

bsoncxx::document::value MongoDB_Handler::CreateValue(string &name) {
    auto stream = std::stringstream{};
    stream << "{";
    stream << "\"name\":\"" << name << "\"";
    stream << "}";
    auto value = bsoncxx::from_json(stream.str());
    return value;
}

bsoncxx::document::view MongoDB_Handler::CreateView(string &name) {
    return MongoDB_Handler::CreateValue(name).view();
}

bsoncxx::document::value MongoDB_Handler::CreateValue(ProjectInfo &info) {
    auto stream = std::stringstream{};
    stream << "{";
    stream << "\"name\":\"" << info.m_ProjectName << "\",";
    stream << "\"composer\":\"" + info.composer + "\",";

    stream << "\"contributors\":[";
    std::list<std::string>::iterator it;
    for (it = info.contributors.begin(); it != info.contributors.end(); ++it) {
        if (it != info.contributors.begin()) {
            stream << ",";
        }
        stream << "\"" << *it << "\"";
    }
    stream << "]";

    stream << "}";
    auto value = bsoncxx::from_json(stream.str());
    return value;
}

bsoncxx::document::view MongoDB_Handler::CreateView(ProjectInfo &info) {
    return MongoDB_Handler::CreateValue(info).view();
}

bsoncxx::document::value MongoDB_Handler::LoadFromDatabase(std::string &name) {
    client client{uri{}};

    auto projects_collection = client[DATABASE][INFO_COLLECTION];

    auto search_pattern = CreateValue(name);
    std::cout << "Search for a project like " << bsoncxx::to_json(search_pattern) << std::endl;
    auto optional_project_value = projects_collection.find_one(search_pattern.view());

    if (!optional_project_value) {
        throw std::range_error("There is no project with the name \"" + name + "\" on the database.");
    }

    return optional_project_value.value();
}

bool MongoDB_Handler::ExistsInDatabase(std::string &name) {
    try {
        auto project_value = MongoDB_Handler::LoadFromDatabase(name);
        return true;
    } catch (const std::exception& e) {
        return false;
    }
}
