#pragma once
#include <xaudio2.h>
#include <fstream>
#include <wrl.h>
#include <string>
#include <array>

/// <summary>
/// 音楽関係のクラス
/// </summary>
class Audio final{
public://静的メンバ変数
	static inline const int kSoundNum = 256;
public://構造体等
	//チャンクヘッダ
	typedef struct ChunkHeader {
		char id[4];//チャンク毎のID
		int32_t size;//チャンクサイズ
	}ChunkHeader;

	//RIFFヘッダチャンク
	typedef struct RiffHeader {
		ChunkHeader chunk;//"RIFF"
		char type[4];//"WAVE"
	}RiffHeader;

	//FMTチャンク
	typedef struct FormatChunk {
		ChunkHeader chunk;//"fmt"
		WAVEFORMATEX fmt;//波形フォーマット
	}FormatChunk;

	//音声データ
	typedef struct SoundData {
		WAVEFORMATEX wfex;//波形フォーマット
		BYTE* pBuffer;//バッファの先頭アドレス
		unsigned int bufferSize;//バッファサイズ
	}SoundData;

public://メンバ関数

	/// <summary>
	/// インスタンスのゲッター
	/// </summary>
	/// <returns></returns>
	static Audio* GetInstance();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="directoryPath">ディレクトリ名</param>
	void Initialize(const std::string& directoryPath = "engine/resources/sounds");

	/// <summary>
	/// サウンドデータのセッター
	/// </summary>
	/// <param name="soundNumber">サウンドの番号</param>
	/// <param name="filename">ファイル名</param>
	void SetSoundData(int soundNumber, const std::string& filename);

	/// <summary>
	/// サウンドデータの再生
	/// </summary>
	/// <param name="soundNumber">サウンドの番号</param>
	/// <param name="isLoop">ループさせるかのフラグ</param>
	void SoundPlayWave(int soundNumber, bool isLoop);

	/// <summary>
	/// 最後にやるやつ
	/// </summary>
	void Finalize();

	/// <summary>
	/// 音量のセッター
	/// </summary>
	/// <param name="soundNumber">サウンド番号</param>
	/// <param name="volume">音量</param>
	void SetVolume(int soundNumber, float volume = 1.0f);

	/// <summary>
	/// 再生
	/// </summary>
	/// <param name="soundNumber">サウンド番号</param>
	void PlayAudio(int soundNumber);

	/// <summary>
	/// 一時停止
	/// </summary>
	/// <param name="soundNumber"></param>
	void PauseAudio(int soundNumber);

	/// <summary>
	/// 完全停止
	/// </summary>
	/// <param name="soundNumber"></param>
	void StopAudio(int soundNumber);

	//コピーコンストラクタ禁止
	Audio(const Audio&) = delete;
	//代入演算子禁止
	Audio& operator=(const Audio&) = delete;

private://メンバ関数
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Audio() = default;

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Audio() = default;

	/// <summary>
	/// サウンドデータのロード
	/// </summary>
	/// <param name="filename">ファイル名</param>
	/// <returns></returns>
	SoundData SoundLoadWave(const std::string& filename);

	/// <summary>
	/// 音声データの解放
	/// </summary>
	/// <param name="soundData">サウンドデータ</param>
	void SoundUnLoad(SoundData* soundData);

	/// <summary>
	/// サウンドデータの再生
	/// </summary>
	/// <param name="soundData">サウンドデータ</param>
	/// <param name="isLoop">ループするかのフラグ</param>
	/// <param name="soundNamber">サウンド番号</param>
	void SoundPlayWave(const SoundData& soundData, bool isLoop, int soundNamber);
private://メンバ変数
	//XAudio2
	Microsoft::WRL::ComPtr<IXAudio2>xAudio2_;
	//マスターボイス
	IXAudio2MasteringVoice* masterVoice_;
	//ディレクトリ名
	std::string directoryPath_ = "";
	//サウンドデータ
	std::array<SoundData, kSoundNum> soundDatas_ = {};
	//波形フォーマットを元にSourceVoiceの生成
	std::array < IXAudio2SourceVoice*, kSoundNum> pSorceVoice_ = { nullptr };
};

