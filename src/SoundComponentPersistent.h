#ifndef SOUNDCOMPONENTPERSISTENT_H
#define SOUNDCOMPONENTPERSISTENT_H

#include "SoundComponent.h"
#include <string>

class SoundComponentPersistent : public SoundComponent
{

public:
    SoundComponentPersistent(SoundDeviceSettings *soundDeviceSettings);
    ~SoundComponentPersistent();

    virtual void openPlayBackStream();
    virtual void openRecordStream();

private:
    static int recordCallback(const void *inputBuffer, void *outputBuffer,
                              unsigned long framesPerBuffer,
                              const PaStreamCallbackTimeInfo *timeInfo,
                              PaStreamCallbackFlags statusFlags,
                              void *userData);

    static int playCallback(const void *inputBuffer, void *outputBuffer,
                            unsigned long framesPerBuffer,
                            const PaStreamCallbackTimeInfo *timeInfo,
                            PaStreamCallbackFlags statusFlags, void *userData);
};

#endif // SOUNDCOMPONENTPERSISTENT_H
