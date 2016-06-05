#include "SoundManager.h"
#include "AssetManager.h"
#include <stb_vorbis.c>

void SLAPIENTRY play_callback(SLPlayItf player, void *context, SLuint32 event) {
	if (event & SL_PLAYEVENT_HEADATEND) {
		for (int i = 0; i < SOUND_COUNT; ++i) {
			if (player == g_SoundManager.GetClip(i).Player) {
				g_SoundManager.GetClip(i).Isdone = true;
			}
		}
	}
}

SoundManager::SoundManager() {

}

SoundManager::~SoundManager() {

}

SoundManager& SoundManager::GetInstance() {
	static SoundManager m_Instance;
	return m_Instance;
}

void SoundManager::Init() {
	SLObjectItf engine_obj;
	SLresult res;
	res = slCreateEngine(&m_EngineObj, 0, nullptr, 0, nullptr, nullptr);
	(*m_EngineObj)->Realize(m_EngineObj, SL_BOOLEAN_FALSE);
	(*m_EngineObj)->GetInterface(m_EngineObj, SL_IID_ENGINE, &m_Engine);
	if (res != SL_RESULT_SUCCESS)
		int i = 0;
	const SLInterfaceID ids[] = { SL_IID_VOLUME };
	const SLboolean req[] = { SL_BOOLEAN_FALSE };

	SLVolumeItf output_mix_vol;

	res = (*m_Engine)->CreateOutputMix(m_Engine, &m_OutputMix, 1, ids, req);
	if (res != SL_RESULT_SUCCESS)
		int i = 0;
	res = (*m_OutputMix)->Realize(m_OutputMix, SL_BOOLEAN_FALSE);
	if (res != SL_RESULT_SUCCESS)
		int i = 0;
	if ((*m_OutputMix)->GetInterface(m_OutputMix, SL_IID_VOLUME, &output_mix_vol) != SL_RESULT_SUCCESS)
		output_mix_vol = nullptr;

	LoadSound("sounds/Dog.ogg", BGM);
	PlaySound(BGM, false);
}

void SoundManager::Update() {
}

void SoundManager::PlaySound(SOUND_ID id, bool loop) {
	(*m_Clips[id].PlayerBuffer)->Enqueue(m_Clips[id].PlayerBuffer, m_Clips[id].ClipBuffer, m_Clips[id].Size);
	(*m_Clips[id].Player)->SetPlayState(m_Clips[id].Player, SL_PLAYSTATE_PLAYING);
	m_Clips[id].IsPlaying = true;
	m_Clips[id].IsLooping = loop;
	m_Clips[id].Isdone = false;

}

AudioClip& SoundManager::GetClip(int id) {
	return m_Clips[id];
}

void SoundManager::LoadSound(const std::string& filename, SOUND_ID id) {
	AssetFile* file;
	if (g_AssetManager.LoadAssetFile(filename, &file)) {
		m_Clips[id].ClipBuffer = (short*)malloc(file->Size);
		stb_vorbis_decode_memory((uint8*)file->Data, file->Size, &m_Clips[id].Channels, &m_Clips[id].SampleRate, &m_Clips[id].ClipBuffer);
	} else {
		return;
	}
	m_Clips[id].Size = file->Size;
	g_AssetManager.FreeAssetFile(file);

	SLDataLocator_Address in_loc;
	in_loc.locatorType = SL_DATALOCATOR_ADDRESS;
	in_loc.pAddress = m_Clips[id].ClipBuffer;
	in_loc.length = m_Clips[id].Size;

	SLDataFormat_PCM format;
	format.formatType = SL_DATAFORMAT_PCM;
	format.numChannels = 2;
	format.samplesPerSec = SL_SAMPLINGRATE_44_1;
	format.bitsPerSample = SL_PCMSAMPLEFORMAT_FIXED_16;
	format.containerSize = SL_PCMSAMPLEFORMAT_FIXED_16;
	format.channelMask = SL_SPEAKER_FRONT_CENTER;
	format.endianness = SL_BYTEORDER_LITTLEENDIAN;

	SLDataSource src;
	src.pLocator = &in_loc;
	src.pFormat = &format;

	SLDataLocator_OutputMix out_loc;
	out_loc.locatorType = SL_DATALOCATOR_OUTPUTMIX;
	out_loc.outputMix = m_OutputMix;

	SLDataSink dst;
	dst.pLocator = &out_loc;
	dst.pFormat = nullptr;

	const SLuint32 soundPlayerIIDCount = 2;
	const SLInterfaceID ids[] = { SL_IID_VOLUME, SL_IID_BUFFERQUEUE };
	const SLboolean req[] = { SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE };

	SLresult result = (*m_Engine)->CreateAudioPlayer(m_Engine, &m_Clips[id].PlayerObj, &src, &dst,
		soundPlayerIIDCount, ids, req);

	(*m_Clips[id].PlayerObj)->Realize(m_Clips[id].PlayerObj, SL_BOOLEAN_FALSE);

	(*m_Clips[id].PlayerObj)->GetInterface(m_Clips[id].PlayerObj,
		SL_IID_PLAY, &m_Clips[id].Player);

	(*m_Clips[id].PlayerObj)->GetInterface(m_Clips[id].PlayerObj,
		SL_IID_VOLUME, &m_Clips[id].Volume);

	(*m_Clips[id].PlayerObj)->GetInterface(m_Clips[id].PlayerObj,
		SL_IID_ANDROIDSIMPLEBUFFERQUEUE, &m_Clips[id].PlayerBuffer);

	(*m_Clips[id].Player)->RegisterCallback(m_Clips[id].Player, play_callback, nullptr);
	(*m_Clips[id].Player)->SetCallbackEventsMask(m_Clips[id].Player, SL_PLAYEVENT_HEADATEND);

}
