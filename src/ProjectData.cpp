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

void ProjectData::updateListModel() { soundListModel.update(); }

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

int ProjectData::recordSound()
{
    // Open new window for recording

    cpp_redis::client rClient;
    try {
        rClient.connect("127.0.0.1", 7000);
    } catch (const std::exception &ex) {
        std::cerr << "Can't connect to Redis: " << ex.what() << std::endl;
        std::cerr << "Make sure, that Redis is Running at " << REDIS_HOST << ":"
                  << REDIS_PORT << std::endl;
        return Operation::ERROR_;
    }

    // open Window for recording (Textfield(Name), recStart,  recFinish
    // recCancel, "x"/Cancel )

    // startRec
    // check, if name is available

    // cancelRec

    SoundComponent *rec = new SoundComponentPersistent(soundDeviceSettings);

    // fill with data
    rec->openRecordStream();

    while (rec->getStreamState() == paContinue) {
        1 + 1;
    }

    rClient.disconnect();

    // needs to init SCG into 2 steps, otherwise Qt goes crazy
    SoundComponentGraphic *scg = new SoundComponentGraphic();
    scg->setSoundComponent(rec);
    sounds.append(scg);
    return Operation::SUCCESS_;
}

int ProjectData::deleteSound(int id)
{
    cpp_redis::client rClient;
    std::future<cpp_redis::reply> redisAnswer;
    try {
        rClient.connect("127.0.0.1", 7000);
    } catch (const std::exception &ex) {
        std::cerr << "Can't connect to Redis: " << ex.what() << std::endl;
        std::cerr << "Make sure, that Redis is Running at " << REDIS_HOST << ":"
                  << REDIS_PORT << std::endl;
        return false;
    }
    redisTmpKeyStore.clear();

    auto it = sounds.begin() + id;
    redisTmpKeyStore.push_back((*it)->getSoundComponent()->getName());

    // Delete Soundstream
    rClient.del(redisTmpKeyStore);

    delete *it;
    sounds.erase(it);

    return Operation::SUCCESS_;
}

bool ProjectData::checkNameAvailability()
{

    cpp_redis::client rClient;
    std::future<cpp_redis::reply> redisAnswer;
    try {
        rClient.connect("127.0.0.1", 7000);
    } catch (const std::exception &ex) {
        std::cerr << "Can't connect to Redis: " << ex.what() << std::endl;
        std::cerr << "Make sure, that Redis is Running at " << REDIS_HOST << ":"
                  << REDIS_PORT << std::endl;
        return false;
    }
    redisAnswer = rClient.exists(std::vector<std::string>({"Test"}));
    rClient.sync_commit();
    rClient.disconnect();
    return redisAnswer.get().as_integer() == 0;
}
