#ifndef PROJECTINFO_H
#define PROJECTINFO_H

#include <string>
#include <list>

class ProjectInfo
{
public:
    ProjectInfo();

    std::string m_ProjectName;
    // other neccessary projectInformations
    std::string composer;
    std::list<std::string> contributors;
    std::string year;
};

#endif // PROJECTINFO_H
