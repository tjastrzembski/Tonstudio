#pragma once

#include "portaudio.h"
#include <string>

typedef struct
{
	std::string name;
	int			frameIndex;  /* Index into sample array. */
	int			maxFrameIndex;
	PaStreamCallbackResult		activeStream;
}
recordData;

class SoundComponent
{
public:
	SoundComponent();
	SoundComponent(const PaStreamParameters* inputP, PaStreamParameters* outputP, PaStreamCallback* paCallback);
	~SoundComponent();

	recordData rData;
	PaStream* stream;

	static int recordCallback(const void *inputBuffer, void *outputBuffer,
		unsigned long framesPerBuffer,
		const PaStreamCallbackTimeInfo* timeInfo,
		PaStreamCallbackFlags statusFlags,
		void *userData);

	static int playCallback(const void *inputBuffer, void *outputBuffer,
		unsigned long framesPerBuffer,
		const PaStreamCallbackTimeInfo* timeInfo,
		PaStreamCallbackFlags statusFlags,
		void *userData);

	PaError openStream(const PaStreamParameters* inputP, PaStreamParameters* outputP, PaStreamCallback* paCallback);
	PaError startStream(void);
	PaError stopStream(void);
	PaError closeStream(void);
};
