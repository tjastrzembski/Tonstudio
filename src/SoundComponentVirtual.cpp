#include "SoundComponentVirtual.h"
#include "GeneralConfigs.h"
#include <cpp_redis/cpp_redis>

int SoundComponentVirtual::recordCallback(
    const void *inputBuffer, void *outputBuffer, unsigned long framesPerBuffer,
    const PaStreamCallbackTimeInfo *timeInfo, PaStreamCallbackFlags statusFlags,
    void *userData)
{
    SoundComponentVirtual *data = (SoundComponentVirtual *)userData;
    const float *rptr = (const float *)inputBuffer;
    long i;
    int frameIndex = data->getFrameIndex();
    int pos = frameIndex * NUM_CHANNELS;
    data->setFrameIndex(frameIndex + framesPerBuffer);
    int bufferSize = data->getFrameIndex() * NUM_CHANNELS;

    // setup cpp_redis
    cpp_redis::client rClient;
    std::future<cpp_redis::reply> redisAnswer;
    rClient.connect(REDIS_HOST, REDIS_PORT);
    std::vector<std::pair<std::string, std::string>> keyVal;

    if (inputBuffer == nullptr) {
        for (i = pos; i < bufferSize;) {
            keyVal.push_back(
                std::pair<std::string, std::string>(std::to_string(i++), "0"));
            if (NUM_CHANNELS == 2)
                keyVal.push_back(std::pair<std::string, std::string>(
                    std::to_string(i++), "0"));
        }
    } else {
        for (i = pos; i < bufferSize;) {
            keyVal.push_back(std::pair<std::string, std::string>(
                std::to_string(i++), std::to_string(*rptr++)));
            if (NUM_CHANNELS == 2)
                keyVal.push_back(std::pair<std::string, std::string>(
                    std::to_string(i++), std::to_string(*rptr++)));
        }
    }
    rClient.hmset(data->getName(), keyVal);
    rClient.sync_commit();
    keyVal.clear();
    rClient.disconnect();
    return data->getStreamState();
}

int SoundComponentVirtual::playCallback(
    const void *inputBuffer, void *outputBuffer, unsigned long framesPerBuffer,
    const PaStreamCallbackTimeInfo *timeInfo, PaStreamCallbackFlags statusFlags,
    void *userData)
{
    (void)inputBuffer; /* Prevent unused variable warnings. */
    (void)timeInfo;
    (void)statusFlags;
    (void)userData;

    SoundComponentVirtual *data = (SoundComponentVirtual *)userData;
    float *wptr = (float *)outputBuffer;
    unsigned int i;

    cpp_redis::client rClient;
    std::future<cpp_redis::reply> redisAnswer;
    rClient.connect(REDIS_HOST, REDIS_PORT);

    int frameIndex = data->getFrameIndex();
    int pos = frameIndex * NUM_CHANNELS;
    data->setFrameIndex(frameIndex + framesPerBuffer);
    int bufferSize = data->getFrameIndex() * NUM_CHANNELS;

    std::vector<std::string> fields;

    for (i = pos; i < bufferSize;) {
        fields.push_back(std::to_string(i++));
    }
    redisAnswer = rClient.hmget(data->getName(), fields);
    rClient.sync_commit();
    std::vector<cpp_redis::reply> tmp = redisAnswer.get().as_array();

    for (auto iter = tmp.cbegin(); iter != tmp.cend();) {
        *wptr++ = std::stof((*iter++).as_string()); /* left */
        if (NUM_CHANNELS == 2)
            *wptr++ = std::stof((*iter++).as_string()); /* right */
    }
    rClient.disconnect();
    return data->getFrameIndex() * NUM_CHANNELS < data->getLength()
               ? paContinue
               : paComplete;
}

void SoundComponentVirtual::openRecordStream()
{
    // does not really have a record function, it uses given information to
    // recreate the modified sound

    cpp_redis::client rClient;
    std::future<cpp_redis::reply> redisAnswer;
    rClient.connect(REDIS_HOST, REDIS_PORT);
    std::vector<cpp_redis::reply> tmp;
    std::vector<std::pair<std::string, std::string>> keyVal;
    std::vector<std::string> fields;

    redisAnswer = rClient.hlen(m_name + "_raw");
    rClient.sync_commit();
    m_maxFrameIndex = redisAnswer.get().as_integer();

    // Todo: if multiple sounds are used, this must be modified

    for (int j = 0; j < m_maxFrameIndex; j++) {
        fields.push_back(std::to_string(j));
    }

    redisAnswer = rClient.hmget(m_name + "_raw", fields);
    rClient.sync_commit();
    tmp = redisAnswer.get().as_array();

    int i(0);
    for (auto iter = tmp.cbegin(); iter != tmp.cend();) {
        keyVal.push_back(std::pair<std::string, std::string>(
            std::to_string(i++), (*iter++).as_string()));
    }
    rClient.hmset(m_name, keyVal);
    rClient.sync_commit();
    keyVal.clear();
    fields.clear();
}

void SoundComponentVirtual::openPlayBackStream()
{
    // Define Outputstream

    Pa_OpenStream(&stream, m_soundDeviceSettings->getInputDevice(),
                  m_soundDeviceSettings->getOutputDevice(),

                  SAMPLE_RATE, 1024, paClipOff, playCallback, this);
}

SoundComponentVirtual::SoundComponentVirtual(
    SoundDeviceSettings *soundDeviceSettings)
    : SoundComponent(soundDeviceSettings)
{
}

SoundComponentVirtual::~SoundComponentVirtual() {}
