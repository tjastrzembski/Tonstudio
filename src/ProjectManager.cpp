#include "ProjectManager.h"
#include "GeneralConfigs.h"

ProjectManager::ProjectManager(QQuickItem *parent)
    : QQuickItem(parent), soundDeviceSettings(new SoundDeviceSettings())
{
    project = new ProjectData(soundDeviceSettings);
    // sth about name for project etc
    // id for project
}

ProjectManager::~ProjectManager()
{
    delete project;
    delete soundDeviceSettings;
}

ProjectData *ProjectManager::getCurrentProject() const { return project; }

int ProjectManager::loadProject()
{
    delete project;

    project = new ProjectData(soundDeviceSettings);
    return Operation::ERROR_;
}

int ProjectManager::saveProject() {

    return Operation::ERROR_; }

int ProjectManager::deleteProject() {

    delete project;

    return Operation::ERROR_; }
