#ifndef PROJECTMANAGER_H
#define PROJECTMANAGER_H

#include <QQuickItem>

#include "ProjectData.h"
#include "sounddevicesettings.h"

class ProjectManager : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(ProjectData *currentProject READ getCurrentProject CONSTANT)
public:
    ProjectManager(QQuickItem *parent = nullptr);
    ~ProjectManager();

    int loadProject();
    int saveProject();
    int deleteProject();

    ProjectData *getCurrentProject() const;

private:
    ProjectData *project;
    SoundDeviceSettings *soundDeviceSettings;
};

#endif // PROJECTMANAGER_H
