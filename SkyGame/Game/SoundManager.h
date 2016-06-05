#pragma once
#include <string>
#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>
#define g_SoundManager SoundManager::GetInstance()

struct AudioClip {
	SLPlayItf Player;
	SLObjectItf PlayerObj;
	SLAndroidSimpleBufferQueueItf PlayerBuffer;
	SLVolumeItf Volume;
	short* ClipBuffer;
	int Channels;
	int SampleRate;
	bool IsPlaying = false;
	bool Isdone = false;
	bool IsLooping = false;
	int Size;
};

enum SOUND_ID {
	BGM,
	JUMP,
	LAND,
	SOUND_COUNT
};

class SoundManager {
public:
	~SoundManager();
	static SoundManager& GetInstance();
	void Init();
	void Update();
	void PlaySound(SOUND_ID id, bool loop);
	AudioClip& GetClip(int id);
private:
	void LoadSound(const std::string& filename, SOUND_ID id);
	SoundManager();
	SLObjectItf m_OutputMix;
	SLEngineItf m_Engine;
	SLObjectItf m_EngineObj;
	AudioClip m_Clips[SOUND_COUNT];
};