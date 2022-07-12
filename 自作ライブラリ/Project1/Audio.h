#pragma once
#include <array>
#include <cstdint>
#include <wrl.h>
#include <xaudio2.h>
#include <string>
#include <map>

#include"XAudio2VoiceCallback.h"

#pragma comment(lib,"xaudio2.lib")

using namespace Microsoft::WRL;

//�`�����N�w�b�_
struct Chunk
{
	std::array<char, 4> id;;//�`�����N���Ƃ�ID
	int32_t size;//�`�����N�T�C�Y
};

//RIFF�w�b�_�`�����N
struct RiffHeader
{
	Chunk chunk;//"RIFF"
	std::array<char, 4> type;//"WAVE"
};

//FMT�`�����N
struct FormatChunk
{
	Chunk chunk;//"fmt"
	WAVEFORMAT fmt;//�g�`�t�H�[�}�b�g
};

struct File
{
	Chunk chunk;
	RiffHeader riffheader;
	FormatChunk fChunk;
	char* buff;
};

class Audio
{
public:
	Audio();
	~Audio();
	static void Initialize();
	static void LoadFile(const std::string& keyName, const std::string& fileName);
	static void End();	

	static void VolumeChangeBGM(const std::string& keyName, const float& soundVol);
	static void VolumeChangeSE(const std::string& keyName, const float& soundVol);

	//�Đ��E��~
	static void PlayBGM(const std::string& arg_name, const float arg_volume);
	static void StopBGM(const std::string& arg_name);
	//�ꎞ��~�E�ĊJ
	static void PauseBGM(const std::string& arg_name);
	static void ResumeBGM(const std::string& arg_name);

	static void PlaySE(const std::string& arg_name, const float arg_volume, bool loop = false, int loopCount = XAUDIO2_LOOP_INFINITE);
	static void StopSE(const std::string& arg_name);
	
	static void PauseSE(const std::string& arg_name);
	static void ResumeSE(const std::string& arg_name);

	static void AllPauseSE();
	static void AllResumeSE();

	static void AllStopSE();

	static float volume_bgm;
	static float volume_se;

private:
	static ComPtr<IXAudio2> xAudio2;
	static IXAudio2MasteringVoice* masterVoice;
	static std::map <std::string, File> soundFiles;
	//static std::map <std::string, IXAudio2SourceVoice*> soundVoices;
	static std::map <std::string, IXAudio2SourceVoice*> bgmSourceVoices;
	static std::map <std::string, IXAudio2SourceVoice*> seSourceVoices;
};