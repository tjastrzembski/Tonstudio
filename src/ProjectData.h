#ifndef PROJECTDATA_H
#define PROJECTDATA_H

#include "SoundComponent.h"
#include "SoundComponentGraphic.h"
#include "SoundListModel.h"
#include "projectinfo.h"
#include <QQuickItem>
#include <QVariant>
#include <vector>

// Forward declaration
class SoundDeviceSettings;
class SoundListModel;

class ProjectData : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(int maxSBWidth READ getMaxSoundBarWidth NOTIFY maxWidthChanged)
    Q_PROPERTY(int numSounds READ getNumSounds NOTIFY numberOfSoundsChanged)
public:
    ProjectData(SoundDeviceSettings *soundDeviceSettings,
                QQuickItem *parent = nullptr);
    ProjectData(std::string& projectName, SoundDeviceSettings *soundDeviceSettings,
                QQuickItem *parent = nullptr);
    ~ProjectData();

    int saveProjectData();

    int getMaxSoundBarWidth();
    int getNumSounds();

    Q_INVOKABLE bool checkNameAvailability(const QString &recName);

    Q_INVOKABLE int recordSound(const QString &recName);
    Q_INVOKABLE int finishRecording(const QString &recName);
    Q_INVOKABLE int abortRecording();
    Q_INVOKABLE int deleteSound(int id);

    Q_INVOKABLE int playSound();
    Q_INVOKABLE int pauseSound();
    Q_INVOKABLE int stopSound();

    // Q_INVOKABLE int cutSound(int id, long from, long to);
    // Q_INVOKABLE int insertSound(int id, long at);

    // Redis involved

signals:
    void numberOfSoundsChanged();
    void maxWidthChanged();

private:
    // project related data
    QList<SoundComponent *> sounds;
    QList<SoundComponent *> rawSounds;
    ProjectInfo projectInfo;
    SoundComponent *recSound;

    // Variables needed by application itself
    SoundListModel *soundListModel;
    SoundDeviceSettings *soundDeviceSettings;
};

#endif // PROJECTDATA_H
