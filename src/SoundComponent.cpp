#include "SoundComponent.h"
#include "GeneralConfigs.h"
#include <cpp_redis\cpp_redis>

SoundComponent::SoundComponent(SoundDeviceSettings *soundDeviceSettings)
    : m_frameIndex(0), m_maxFrameIndex(-1), m_StreamState(paContinue),
      m_soundDeviceSettings(soundDeviceSettings)
{
}

SoundComponent::~SoundComponent()
{
    if (!Pa_IsStreamStopped)
        closeStream();
}

PaError SoundComponent::openStream(const PaStreamParameters *inputP,
                                   PaStreamParameters *outputP,
                                   PaStreamCallback *paCallback)
{
    auto err = Pa_OpenStream(
        &stream, inputP,       /* no input channels */
        outputP,               /* stereo output */
                               // paFloat32,  /* 32 bit floating point output */
        SAMPLE_RATE, 256,      /* frames per buffer, i.e. the number
                                           of sample frames that PortAudio will
                                           request from the callback. Many apps
                                           may want to use
                                           paFramesPerBufferUnspecified, which
                                           tells PortAudio to pick the best,
                                           possibly changing, buffer size.*/
        paClipOff, paCallback, /* this is your callback function */
        this);                 /*This is a pointer that will be passed to
                                         your callback*/

    return err;
}

PaError SoundComponent::startStream()
{
    auto err = Pa_StartStream(stream);
    return err;
}

PaError SoundComponent::stopStream()
{
    auto err = Pa_StopStream(stream);
    return err;
}

PaError SoundComponent::closeStream()
{
    auto err = Pa_CloseStream(stream);
    return err;
}

std::string &SoundComponent::getName() { return m_name; }

void SoundComponent::setName(std::string name) { m_name = name; }

int SoundComponent::getFrameIndex() const { return m_frameIndex; }

void SoundComponent::setFrameIndex(int frameIndex)
{
    m_frameIndex = frameIndex;
}

int SoundComponent::getLength() const { return m_maxFrameIndex; }

void SoundComponent::setLength(int length) { m_maxFrameIndex = length; }

PaStreamCallbackResult SoundComponent::getStreamState() const
{
    return m_StreamState;
}

void SoundComponent::setStreamState(PaStreamCallbackResult result)
{
    m_StreamState = result;
}
