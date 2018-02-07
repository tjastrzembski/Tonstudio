#include "sounddevicesettings.h"

SoundDeviceSettings::SoundDeviceSettings()
    : inputParameters(new PaStreamParameters()),
      outputParameters(new PaStreamParameters())
{
    // Get the devices, which are set by system
    memset(inputParameters, '_', sizeof(PaStreamParameters));
    inputParameters->channelCount = 2;
    inputParameters->device = Pa_GetDefaultInputDevice();
    inputParameters->hostApiSpecificStreamInfo = nullptr;
    inputParameters->sampleFormat = paFloat32;
    inputParameters->suggestedLatency
        = Pa_GetDeviceInfo(inputParameters->device)->defaultLowInputLatency;
    inputParameters->hostApiSpecificStreamInfo = nullptr;

    memset(outputParameters, '_', sizeof(PaStreamParameters));
    outputParameters->channelCount = 2;
    outputParameters->device = Pa_GetDefaultOutputDevice();
    outputParameters->hostApiSpecificStreamInfo = nullptr;
    outputParameters->sampleFormat = paFloat32;
    outputParameters->suggestedLatency
        = Pa_GetDeviceInfo(outputParameters->device)->defaultLowOutputLatency;
    outputParameters->hostApiSpecificStreamInfo = nullptr;
}

SoundDeviceSettings::~SoundDeviceSettings()
{
    delete inputParameters;
    delete outputParameters;
}

PaStreamParameters *SoundDeviceSettings::getInputDevice()
{
    return inputParameters;
}

void SoundDeviceSettings::setInputDevice(PaDeviceIndex index)
{
    inputParameters->device = index;
    inputParameters->suggestedLatency
        = Pa_GetDeviceInfo(index)->defaultLowInputLatency;
}

PaStreamParameters *SoundDeviceSettings::getOutputDevice()
{
    return outputParameters;
}

void SoundDeviceSettings::setOutputDevice(PaDeviceIndex index)
{
    outputParameters->device = index;
    outputParameters->suggestedLatency
        = Pa_GetDeviceInfo(index)->defaultLowOutputLatency;
}
