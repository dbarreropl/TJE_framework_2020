#include "audio.h"
#include "utils.h"
#include <iostream> //to output
#include <cmath>

std::map<std::string, Audio*> Audio::sLoadedAudios;

Audio::Audio() {
	sample = 0;
	volume = 1;
	loop = false;
}

Audio::~Audio() {
	BASS_SampleFree(sample);
	/*
	//iterate samples
	for (auto it = sLoadedAudios.begin(); it != sLoadedAudios.end(); it++)
	{
		Audio* s = it->second;
		delete s;
	}
	sLoadedAudios.clear();*/
}

HCHANNEL Audio::play(float volume)
{
	//Creamos un canal para el sample
	HCHANNEL hSampleChannel = BASS_SampleGetChannel(this->sample, this->loop);

	//Lanzamos un sample
	BASS_ChannelPlay(hSampleChannel, true);
	return hSampleChannel;
}

HCHANNEL Audio::Play(const char* filename, float volume, bool loop)
{
	Audio* audio = Audio::Get(filename, loop);
	BASS_SetConfig(BASS_CONFIG_GVOL_SAMPLE,	volume); //volume 10000
	//Creamos un canal para el sample
	HCHANNEL hSampleChannel = BASS_SampleGetChannel(audio->sample, false);
	audio->channel = hSampleChannel;
	//Lanzamos un sample
	BASS_ChannelPlay(hSampleChannel, true);
	return hSampleChannel;
}

HCHANNEL Audio::GetChannel(const char* filename)
{
	Audio* audio = Audio::Get(filename, false);
	return audio->channel;
}

void Audio::Stop(HCHANNEL channel)
{
	BASS_ChannelStop(channel);
}

void Audio::Pause(HCHANNEL channel)
{
	BASS_ChannelPause(channel);
}

void Audio::Resume(HCHANNEL channel)
{
	BASS_ChannelPlay(channel,false);
}

Audio* Audio::Get(const char* filename, bool loop)
{
	assert(filename);

	//check if loaded
	std::map<std::string, Audio*>::iterator it = sLoadedAudios.find(filename);
	if (it != sLoadedAudios.end())
		return it->second;

	//load it
	std::cout << " + Audio loading: " << filename << " ... ";
	Audio* audio = new Audio();
	DWORD lp = loop;
	if (loop)
		lp = BASS_SAMPLE_LOOP;

	audio->sample = BASS_SampleLoad(false, filename, 0, 0, 3, lp);
	sLoadedAudios[filename] = audio;

	return audio;
}
