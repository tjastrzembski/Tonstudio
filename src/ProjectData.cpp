#include "ProjectData.h"
#include "GeneralConfigs.h"
#include "SoundComponentPersistent.h"
#include "SoundComponentVirtual.h"
#include <cpp_redis/cpp_redis>

ProjectData::ProjectData(SoundDeviceSettings *soundDeviceSettings,
                         QQuickItem *parent)
    : QQuickItem(parent), soundDeviceSettings(soundDeviceSettings), sounds(), rawSounds()
{
    soundListModel = new SoundListModel(sounds);
    soundListModel->update();
}

ProjectData::ProjectData(std::string& projectName, SoundDeviceSettings *soundDeviceSettings,
                         QQuickItem *parent)
    : QQuickItem(parent), soundDeviceSettings(soundDeviceSettings), sounds(),rawSounds()
{
    soundListModel = new SoundListModel(sounds);

    //get all stuff from cassandra and MongoDB; projectName shall be the identifier

    //mongoDB stuff
    projectInfo.m_ProjectName = projectName;
    projectInfo.year;
    projectInfo.contributors;
    projectInfo.composer;

    //Cassandra
    rawSounds.append(nullptr);
    soundListModel->addSound(nullptr);



    soundListModel->update();
}

ProjectData::~ProjectData() { delete soundListModel; }


int ProjectData::saveProjectData()
{


   return Operation::SUCCESS_;
}


int ProjectData::getMaxSoundBarWidth()
{
    int maxWidth(0);
    int currentWidth;
    for (SoundComponent *sc : sounds) {
        currentWidth = sc->getLength();
        if (maxWidth < currentWidth)
            maxWidth = currentWidth;
    }
    return 1300000;
    // return maxWidth;
}

int ProjectData::getNumSounds() { return sounds.size(); }

int ProjectData::recordSound(const QString &recName)
{
    recSound = new SoundComponentPersistent(soundDeviceSettings);

    // mentality: keep RAW untouched, copy sound into other file
    std::string rawName = recName.toStdString() + "_raw"; // = checkforname
    recSound->setName(rawName);
    recSound->openRecordStream();
    recSound->startStream();

    return Operation::SUCCESS_;
}

int ProjectData::abortRecording()
{
    recSound->stopStream();
    recSound->closeStream();

    delete recSound;

    return Operation::SUCCESS_;
}

int ProjectData::finishRecording(const QString &recName)
{
    recSound->stopStream();
    recSound->closeStream();

    // "save" the raw sound for a possible commit into Cassandra
    rawSounds.append(recSound);

    // make an editable copy of the raw file,
    SoundComponent *rec_virtual
        = new SoundComponentVirtual(soundDeviceSettings);
    rec_virtual->setName(recName.toStdString());
    rec_virtual->openRecordStream();

    soundListModel->addSound(rec_virtual);
    soundListModel->update();

    emit numberOfSoundsChanged();
    emit maxWidthChanged();
    return Operation::SUCCESS_;
}

int ProjectData::deleteSound(int id)
{
    auto it = sounds.begin() + id;

    delete *it;
    sounds.erase(it);

    soundListModel->update();
    emit numberOfSoundsChanged();
    emit maxWidthChanged();

    return Operation::SUCCESS_;
}

int ProjectData::playSound()
{
    for (SoundComponent *sc : sounds) {

        sc->openPlayBackStream();
        sc->startStream();
    }
    return Operation::SUCCESS_;
}

int ProjectData::pauseSound()
{
    for (SoundComponent *sc : sounds) {

        sc->stopStream();
        sc->closeStream();
    }
    return Operation::SUCCESS_;
}

int ProjectData::stopSound()
{
    for (SoundComponent *sc : sounds) {
        sc->stopStream();
        sc->closeStream();
        sc->setFrameIndex(0);
    }
    return Operation::SUCCESS_;
}

bool ProjectData::checkNameAvailability(const QString &recName)
{
    if (recName.isEmpty())
        return false;

    cpp_redis::client rClient;
    std::future<cpp_redis::reply> redisAnswer;
    try {
        rClient.connect(REDIS_HOST, REDIS_PORT);
    } catch (const std::exception &ex) {
        std::cerr << "Can't connect to Redis: " << ex.what() << std::endl;
        std::cerr << "Make sure, that Redis is Running at " << REDIS_HOST << ":"
                  << REDIS_PORT << std::endl;
        return false;
    }
    redisAnswer
        = rClient.exists(std::vector<std::string>({recName.toStdString()}));
    rClient.sync_commit();
    rClient.disconnect();
    return redisAnswer.get().as_integer() == 0;
}
