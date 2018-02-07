#ifndef SOUNDCOMPONENTVIRTUAL_H
#define SOUNDCOMPONENTVIRTUAL_H

#include "SoundComponent.h"
#include <string>

class SoundComponentVirtual : public SoundComponent
{

public:
    SoundComponentVirtual(SoundDeviceSettings *soundDeviceSettings);
    ~SoundComponentVirtual();

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

#endif // SOUNDCOMPONENTVIRTUAL_H
