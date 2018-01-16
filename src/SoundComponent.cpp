#include "SoundComponent.h"
#include <cpp_redis\cpp_redis>
#include "GeneralConfigs.h"

int SoundComponent::recordCallback(const void *inputBuffer, void *outputBuffer,
	unsigned long framesPerBuffer,
	const PaStreamCallbackTimeInfo* timeInfo,
	PaStreamCallbackFlags statusFlags,
	void *userData)
{
	recordData *data = (recordData*)userData;
	const float *rptr = (const float*)inputBuffer;
	long i;
	int pos = data->frameIndex * NUM_CHANNELS;
	data->frameIndex += framesPerBuffer;
	int bufferSize = data->frameIndex * NUM_CHANNELS;

	//setup cpp_redis
	cpp_redis::client rClient;
	std::future<cpp_redis::reply> redisAnswer;
	rClient.connect(REDIS_HOST, REDIS_PORT);
	std::vector<std::pair<std::string, std::string>> keyVal;

	if (inputBuffer == nullptr)
	{
		for (i = pos; i < bufferSize; )
		{
			keyVal.push_back(std::pair<std::string, std::string>(std::to_string(i++), "0"));
			if (NUM_CHANNELS == 2)
				keyVal.push_back(std::pair<std::string, std::string>(std::to_string(i++), "0"));
		}
	}
	else
	{
		for (i = pos; i < bufferSize; )
		{
			keyVal.push_back(std::pair<std::string, std::string>(std::to_string(i++), std::to_string(*rptr++)));
			if (NUM_CHANNELS == 2)
				keyVal.push_back(std::pair<std::string, std::string>(std::to_string(i++), std::to_string(*rptr++)));
		}
	}
	rClient.hmset(data->name, keyVal);
	rClient.sync_commit();
	keyVal.clear();
	rClient.disconnect();
	return data->activeStream;
}

int SoundComponent::playCallback(const void *inputBuffer, void *outputBuffer,
	unsigned long framesPerBuffer,
	const PaStreamCallbackTimeInfo* timeInfo,
	PaStreamCallbackFlags statusFlags,
	void *userData)
{
	(void)inputBuffer; /* Prevent unused variable warnings. */
	(void)timeInfo;
	(void)statusFlags;
	(void)userData;

	recordData *data = (recordData*)userData;
	float *wptr = (float*)outputBuffer;
	unsigned int i;

	cpp_redis::client rClient;
	std::future<cpp_redis::reply> redisAnswer;
	rClient.connect(REDIS_HOST, REDIS_PORT);

	if (data->maxFrameIndex == -1)
	{
		redisAnswer = rClient.hlen(data->name);
		rClient.sync_commit();
		data->maxFrameIndex = redisAnswer.get().as_integer();
	}

	int pos = data->frameIndex * NUM_CHANNELS;
	data->frameIndex += framesPerBuffer;
	int bufferSize = data->frameIndex * NUM_CHANNELS;

	std::vector<std::string> fields;

	for (i = pos; i < bufferSize; )
	{
		fields.push_back(std::to_string(i++));
	}
	redisAnswer = rClient.hmget(data->name, fields);
	rClient.sync_commit();
	std::vector<cpp_redis::reply> tmp = redisAnswer.get().as_array();

	for (auto iter = tmp.cbegin(); iter != tmp.cend();)
	{
		*wptr++ = std::stof((*iter++).as_string());		/* left */
		if (NUM_CHANNELS == 2)
			*wptr++ = std::stof((*iter++).as_string());  /* right */
	}
	rClient.disconnect();
	return  data->frameIndex* NUM_CHANNELS < data->maxFrameIndex ? paContinue : paComplete;
}

SoundComponent::SoundComponent()
{
	rData.maxFrameIndex = -1;
	rData.frameIndex = 0;

	rData.activeStream = paContinue;
}

SoundComponent::SoundComponent(const PaStreamParameters* inputP, PaStreamParameters* outputP, PaStreamCallback* paCallback)
{
	rData.maxFrameIndex = -1;
	rData.frameIndex = 0;

	rData.activeStream = paContinue;
	openStream(inputP, outputP, paCallback);
}

SoundComponent::~SoundComponent()
{
	if (!Pa_IsStreamStopped)
		closeStream();
}

PaError SoundComponent::openStream(const PaStreamParameters* inputP, PaStreamParameters* outputP, PaStreamCallback* paCallback)
{
	auto err = Pa_OpenStream(&stream,
		inputP,          /* no input channels */
		outputP,          /* stereo output */
						  //paFloat32,  /* 32 bit floating point output */
		SAMPLE_RATE,
		256,        /* frames per buffer, i.e. the number
					of sample frames that PortAudio will
					request from the callback. Many apps
					may want to use
					paFramesPerBufferUnspecified, which
					tells PortAudio to pick the best,
					possibly changing, buffer size.*/
		paClipOff,
		paCallback, /* this is your callback function */
		&rData); /*This is a pointer that will be passed to
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