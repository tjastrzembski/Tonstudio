#ifndef PROJECTDATA_H
#define PROJECTDATA_H

#include "SoundComponent.h"
#include "SoundComponentGraphic.h"
#include "SoundListModel.h"
#include <QQuickItem>
#include <vector>

// Forward declaration
class SoundDeviceSettings;
class SoundListModel;

class ProjectData : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(QString testString READ testString)
    Q_PROPERTY(int maxSBWidth READ getMaxSoundBarWidth)
    Q_PROPERTY(int numSounds READ getNumSounds)
public:
    ProjectData(SoundDeviceSettings *soundDeviceSettings,
                QQuickItem *parent = nullptr);
    ~ProjectData() = default;

    QString testString();

    void updateListModel();
    int getMaxSoundBarWidth();
    int getNumSounds();

    int recordSound();
    int deleteSound(int id);

    int cutSound(int id, long from, long to);
    int insertSound(int id, long at);

    bool checkNameAvailability();

private:
    QList<SoundComponentGraphic *> sounds;
    SoundListModel soundListModel;
    std::vector<std::string> redisTmpKeyStore;
    SoundDeviceSettings *soundDeviceSettings;
};

#endif // PROJECTDATA_H
