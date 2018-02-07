#include "SoundComponentPersistent.h"
#include "GeneralConfigs.h"
#include <cpp_redis\cpp_redis>

int SoundComponentPersistent::recordCallback(
    const void *inputBuffer, void *outputBuffer, unsigned long framesPerBuffer,
    const PaStreamCallbackTimeInfo *timeInfo, PaStreamCallbackFlags statusFlags,
    void *userData)
{
    SoundComponentPersistent *data = (SoundComponentPersistent *)userData;
    const float *rptr = (const float *)inputBuffer;
    long i;
    int frameIndex = data->getFrameIndex();
    int pos = frameIndex * NUM_CHANNELS;
    data->setFrameIndex(frameIndex + framesPerBuffer);
    int bufferSize = frameIndex * NUM_CHANNELS;

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

int SoundComponentPersistent::playCallback(
    const void *inputBuffer, void *outputBuffer, unsigned long framesPerBuffer,
    const PaStreamCallbackTimeInfo *timeInfo, PaStreamCallbackFlags statusFlags,
    void *userData)
{
    (void)inputBuffer; /* Prevent unused variable warnings. */
    (void)timeInfo;
    (void)statusFlags;
    (void)userData;

    SoundComponentPersistent *data = (SoundComponentPersistent *)userData;
    float *wptr = (float *)outputBuffer;
    unsigned int i;

    cpp_redis::client rClient;
    std::future<cpp_redis::reply> redisAnswer;
    rClient.connect(REDIS_HOST, REDIS_PORT);

    if (data->getLength() == -1) {
        redisAnswer = rClient.hlen(data->getName());
        rClient.sync_commit();
        data->setLength(redisAnswer.get().as_integer());
    }

    int frameIndex = data->getFrameIndex();
    int pos = frameIndex * NUM_CHANNELS;
    data->setFrameIndex(frameIndex + framesPerBuffer);
    int bufferSize = frameIndex * NUM_CHANNELS;

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

void SoundComponentPersistent::openRecordStream()
{
    // Define InputStream

    Pa_OpenStream(&stream, m_soundDeviceSettings->getInputDevice(), nullptr,

                  SAMPLE_RATE, 256, paClipOff, recordCallback, this);
}

void SoundComponentPersistent::openPlayBackStream()
{
    // Define Outputstream

    Pa_OpenStream(&stream, nullptr, m_soundDeviceSettings->getOutputDevice(),

                  SAMPLE_RATE, 256, paClipOff, playCallback, this);
}

SoundComponentPersistent::SoundComponentPersistent(
    SoundDeviceSettings *soundDeviceSettings)
    : SoundComponent(soundDeviceSettings)
{
}

SoundComponentPersistent::~SoundComponentPersistent() {}
