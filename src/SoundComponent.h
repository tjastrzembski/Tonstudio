#ifndef SOUNDCOMPONENT_H
#define SOUNDCOMPONENT_H

#include "portaudio.h"
#include "sounddevicesettings.h"
#include <QQuickItem>
#include <string>

class SoundComponent : public QQuickItem
// Todo: the signals are strong tonight
{
public:
    SoundComponent(SoundDeviceSettings *soundDeviceSettings);
    virtual ~SoundComponent();

    PaStream *stream;

    virtual PaError openStream(const PaStreamParameters *inputP,
                               PaStreamParameters *outputP,
                               PaStreamCallback *paCallback);
    virtual PaError startStream(void);
    virtual PaError stopStream(void);
    virtual PaError closeStream(void);

    virtual void openPlayBackStream() = 0;
    virtual void openRecordStream() = 0;

    std::string &getName();
    void setName(std::string name);
    int getFrameIndex() const;
    void setFrameIndex(int frameIndex);
    int getLength() const;
    void setLength(int length);
    PaStreamCallbackResult getStreamState() const;
    void setStreamState(PaStreamCallbackResult result);
    bool isMuted() const;
    void mute(bool state);

protected:
    std::string m_name;
    int m_frameIndex;
    int m_maxFrameIndex;
    PaStreamCallbackResult m_StreamState;

    // Application Related Stuff
    bool m_Mute;
    SoundDeviceSettings *m_soundDeviceSettings;
};

#endif // SOUNDCOMPONENT_H
