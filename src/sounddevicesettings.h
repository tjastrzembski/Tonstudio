#ifndef SOUNDDEVICESETTINGS_H
#define SOUNDDEVICESETTINGS_H

#include "portaudio.h"
#include <QQuickItem>

class SoundDeviceSettings : public QQuickItem
{
    Q_OBJECT
public:
    SoundDeviceSettings();
    ~SoundDeviceSettings();

    PaStreamParameters *getInputDevice();
    void setInputDevice(PaDeviceIndex index);
    PaStreamParameters *getOutputDevice();
    void setOutputDevice(PaDeviceIndex index);

signals:

public slots:

private:
    // Define InputStream
    PaStreamParameters *inputParameters;
    PaStreamParameters *outputParameters;
};

#endif // SOUNDDEVICESETTINGS_H
