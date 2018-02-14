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
    Q_PROPERTY(QString testString READ testString CONSTANT)
    Q_PROPERTY(int maxSBWidth READ getMaxSoundBarWidth NOTIFY maxWidthChanged)
    Q_PROPERTY(int numSounds READ getNumSounds NOTIFY numberOfSoundsChanged)
    Q_PROPERTY(QVariant model READ model NOTIFY modelChanged)
public:
    ProjectData(SoundDeviceSettings *soundDeviceSettings,
                QQuickItem *parent = nullptr);
    ~ProjectData() = default;

    QString testString();

    QVariant model();
    void updateListModel();

    int getMaxSoundBarWidth();
    int getNumSounds();

    Q_INVOKABLE bool checkNameAvailability(const QString &recName);

    Q_INVOKABLE int recordSound(const QString &recName);
    Q_INVOKABLE int finishRecording(const QString &recName);
    Q_INVOKABLE int abortRecording();
    int deleteSound(int id);

    int cutSound(int id, long from, long to);
    int insertSound(int id, long at);

    // Redis involved

signals:
    void numberOfSoundsChanged();
    void maxWidthChanged();
    void modelChanged();

private:
    // project related data
    QList<SoundComponentGraphic *> sounds;
    ProjectInfo projectInfo;
    QVariant m_model;
    SoundComponent* recSound;

    // Variables needed by application itself
    SoundListModel soundListModel;
    SoundDeviceSettings *soundDeviceSettings;
};

#endif // PROJECTDATA_H
