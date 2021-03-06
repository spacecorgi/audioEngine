#pragma once
#ifndef SOUNDMANAGER_H
#define SOUNDMANAGER_H






#include <map>
#include <Windows.h>
#include <stdio.h>
#include <xaudio2.h>
#include "snd_err.h"
#include "SoundList.h"
#include "VoiceManager.h"
#include "VoiceFactory.h"
#include "Channel.h"
#include "Timeline.h"

class AudioCommand;
class SoundNode;
class Timeline;
class Sound;

struct SoundManagerInfo
{
	XAUDIO2_VOICE_DETAILS voiceDetails;
	DWORD channelMask;
};

class SoundManager
{

private:

	SoundManager();

	SoundManager(const SoundManager&) = delete;
	const SoundManager& operator=(const SoundManager&) = delete;
	~SoundManager();

	static SoundManager* instance;
	static SoundManager& Instance()
	{
		if (!instance)
		{
			instance = new SoundManager();
		}
		return *instance;
	}

	// timeline stuff
	Timeline* timeline;

	friend class Sound;
	friend class VoiceFactory;
	// sounds can get XAudio2 Instance
	snd_err GetXAudio2(IXAudio2*&  out);
	snd_err privRemove(Sound* snd);
	static snd_err Remove(Sound* snd)
	{
		return Instance().privRemove(snd);
	};

	snd_err privCreateVoice(Track*& v, WAVEFORMATEXTENSIBLE& wfx);

	// sound pool
	//std::map<std::string, Sound*> sounds;
	SoundList* list;

	// load a sound
	snd_err privLoadSound(std::string key, char* path);

	// load a sound
	snd_err privLoadSound(unsigned int key, char* path);

	// load a sound
	snd_err privLoadSound(unsigned int key, const char* path);

	// unload
	snd_err privUnloadSound(Sound*& snd);

	snd_err privCreateChannel(Channel*& out);

	// unload
	snd_err privUnload();
	// get a sound via a string
	snd_err privGetSound(Sound*& out, std::string in);

	// get a sound via a string
	snd_err privGetSound(Sound*& out, unsigned int in);

	snd_err privGetInfo(SoundManagerInfo & out);

	snd_err privInsertTimelineCommand(AudioCommand* cmd);

	snd_err privInsertTimelineCommand(AudioCommand * cmd, int trigger_time);

	snd_err privUpdate();

	int privGetTime();

	// mastering voice
	IXAudio2MasteringVoice* masteringVoice;

	// XAudio2 instance
	IXAudio2* xAudio2;

	SoundManagerInfo info;

	snd_err privEndAudioThread();
	bool privAudioThreadEnded();

	bool endAudioThread;

	snd_err privPassCallback(CallbackMailbox* cb)
	{
		this->callbacks.push(cb);
		return snd_err::OK;
	}
	std::stack<CallbackMailbox*> callbacks;


	const char* assetPath = "Sounds\\";

public:


	static snd_err PassCallback(CallbackMailbox* cb)
	{
		return Instance().privPassCallback(cb);
	}

	static snd_err EndAudioThread()
	{
		return Instance().privEndAudioThread();
	}

	static bool AudioThreadEnded()
	{
		return Instance().privAudioThreadEnded();
	}

	static snd_err CancelPlaylist(unsigned int i)
	{
		return Instance().timeline->RemovePlaylist(i);
	}


	static snd_err Update()
	{
		return Instance().privUpdate();
	}

	static snd_err GetInfo(SoundManagerInfo & out)
	{
		return Instance().privGetInfo(out);
	}

	static snd_err InsertTimelineCommand(AudioCommand* cmd)
	{
		return Instance().privInsertTimelineCommand(cmd);
	}

	static snd_err InsertTimelineCommand(AudioCommand* cmd, int t)
	{
		return Instance().privInsertTimelineCommand(cmd, t);
	}

	// load a sound
	static snd_err LoadSound(std::string key, char* in)
	{
		return Instance().privLoadSound(key, in);
	};

	// load a sound
	static snd_err LoadSound(unsigned int key, char* in)
	{
		return Instance().privLoadSound(key, in);
	};

	// load a sound
	static snd_err LoadSound(unsigned int key, const char* in)
	{
		return Instance().privLoadSound(key, in);
	};

	static snd_err UnloadSound(Sound*& snd)
	{
		return Instance().privUnloadSound(snd);
	}

	//create a track
	static snd_err CreateChannel(Channel*& out)
	{
		return Instance().privCreateChannel(out);
	}

	// unload
	static snd_err Unload()
	{
		return Instance().privUnload();
	}

	// get a sound via a string
	static snd_err GetSound(Sound*& out, std::string in)
	{
		return Instance().privGetSound(out, in);
	}

	static snd_err GetSound(Sound*& out, unsigned int in)
	{
		return Instance().privGetSound(out, in);
	}

	static void Print()
	{
		Instance().list->Print();
	}

	static snd_err CreateVoice(Track*& v, WAVEFORMATEXTENSIBLE& wfx)
	{
		return Instance().privCreateVoice(v, wfx);
	};

	static int GetSoundTime()
	{
		return Instance().privGetTime();
	}

};




#endif