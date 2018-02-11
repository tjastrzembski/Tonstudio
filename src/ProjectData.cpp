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
    emit
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
    // Open new window for recording
    cpp_redis::client rClient;
    try {
        rClient.connect(REDIS_HOST, REDIS_PORT);
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

    std::string name = "";
    // fill with data

    // mentality: keep RAW untouched, copy sound into other file
    std::string rawName = +"_raw"; // = checkforname
    rec->setName(rawName);
    rec->openRecordStream();

    while (rec->getStreamState() == paContinue) {
        // wait For ActionEvent
        1 + 1;
    }

    // wait For ActionEvent
    rec->stopStream();
    rec->closeStream();

    SoundComponent *rec_virtual
        = new SoundComponentVirtual(soundDeviceSettings);
    rec_virtual->setName(name);
    // copy the entire hashmapentry on redis

    rClient.disconnect();

    // needs to init SCG into 2 steps, otherwise Qt goes crazy
    SoundComponentGraphic *scg = new SoundComponentGraphic();
    scg->setSoundComponent(rec_virtual);
    sounds.append(scg);

    emit numberOfSoundsChanged();
    emit maxWidthChanged();
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

    emit numberOfSoundsChanged();
    emit maxWidthChanged();

    return Operation::SUCCESS_;
}

bool ProjectData::checkNameAvailability(const QString &recName)
{

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
    redisAnswer = rClient.exists(std::vector<std::string>({"Test"}));
    rClient.sync_commit();
    rClient.disconnect();
    return redisAnswer.get().as_integer() == 0;
}

QVariant ProjectData::model()
{
    m_model = QVariant::fromValue(sounds);
    return m_model;
}
