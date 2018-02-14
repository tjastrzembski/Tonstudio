#include "ProjectData.h"
#include "GeneralConfigs.h"
#include "SoundComponentPersistent.h"
#include "SoundComponentVirtual.h"
#include <cpp_redis/cpp_redis>

ProjectData::ProjectData(SoundDeviceSettings *soundDeviceSettings,
                         QQuickItem *parent)
    : QQuickItem(parent), soundDeviceSettings(soundDeviceSettings),
      soundListModel(sounds)
{
    soundListModel.update();
}

QString ProjectData::testString() { return QString("testStringLOL"); }

void ProjectData::updateListModel()
{
    // soundListModel.update();
    m_model = QVariant::fromValue(sounds);

}

int ProjectData::getMaxSoundBarWidth()
{
    int maxWidth(0);
    int currentWidth;
    for (SoundComponentGraphic *scg : sounds) {
        currentWidth = scg->getSoundComponent()->getLength();
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
    std::string rawName = recName.toStdString() +"_raw"; // = checkforname
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

    SoundComponent *rec_virtual
        = new SoundComponentVirtual(soundDeviceSettings);
    rec_virtual->setName(recName.toStdString());
    // copy the entire hashmapentry on redis
    rec_virtual->openRecordStream();

    // needs to init SCG into 2 steps, otherwise Qt goes crazy
    SoundComponentGraphic *scg = new SoundComponentGraphic();
    scg->setSoundComponent(rec_virtual);
    sounds.append(scg);

    updateListModel();
    emit numberOfSoundsChanged();
    emit maxWidthChanged();
    emit modelChanged();
    return Operation::SUCCESS_;
}

int ProjectData::deleteSound(int id)
{
    auto it = sounds.begin() + id;

    delete *it;
    sounds.erase(it);


    updateListModel();
    emit numberOfSoundsChanged();
    emit maxWidthChanged();
    emit modelChanged();

    return Operation::SUCCESS_;
}

bool ProjectData::checkNameAvailability(const QString &recName)
{
    if(recName.isEmpty())
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
    redisAnswer = rClient.exists(std::vector<std::string>({recName.toStdString()}));
    rClient.sync_commit();
    rClient.disconnect();
    return redisAnswer.get().as_integer() == 0;
}

QVariant ProjectData::model()
{
    m_model = QVariant::fromValue(sounds);
    return m_model;
}
