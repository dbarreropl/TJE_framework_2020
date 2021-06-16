#include "includes.h"
#include <string>
#include <map>
#include "framework.h"
#include <cassert>
#include "bass.h"

class Audio
{
public:
	static std::map<std::string, Audio*> sLoadedAudios; //para nuestro manager

	HSAMPLE sample; //aqui guardamos el handler del sample que retorna BASS_SampleLoad
	float volume;
	bool loop;

	Audio(); //importante poner sample a cero aqui
	~Audio(); //aqui deberiamos liberar el sample con BASS_SampleFree

	HCHANNEL play(float volume); //lanza el audio y retorna el channel donde suena

	static void Stop(HCHANNEL channel); //para parar un audio necesitamos su channel
	static void Pause(HCHANNEL channel);
	static void Resume(HCHANNEL channel);
	static HCHANNEL GetChannel(const char* filename);
	static Audio* Get(const char* filename, bool loop); //manager de audios 
	static HCHANNEL Play(const char* filename, float volume, bool loop); //version estática para ir mas rapido
};