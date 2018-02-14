#ifndef PROJECTINFO_H
#define PROJECTINFO_H

#include <string>
#include <vector>

class ProjectInfo
{
public:
    ProjectInfo();

    // getter/setter?

private:
    std::string m_ProjectName;
    // other neccessary projectInformations
    std::string composer;
    std::vector<std::string> contributors;
    std::string year;
};

#endif // PROJECTINFO_H
