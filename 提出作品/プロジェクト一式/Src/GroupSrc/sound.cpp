//=============================================================================
//
// サウンド処理 [sound.cpp]
// Author : AKIRA TANAKA
//
//=============================================================================

//****************************************************
// インクルードファイル
//****************************************************
#include "sound.h"

//****************************************************
// 静的メンバ変数の初期化
//****************************************************
CSound* CSound::m_pSound = nullptr;	// サウンド

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CSound::Init(HWND hWnd)
{
	HRESULT hr;

	// COMライブラリの初期化
	CoInitializeEx(NULL, COINIT_MULTITHREADED);

	// XAudio2オブジェクトの作成
	hr = XAudio2Create(&m_pXAudio2, 0);
	if (FAILED(hr))
	{
		MessageBox(hWnd, "XAudio2オブジェクトの作成に失敗！", "警告！", MB_ICONWARNING);

		// COMライブラリの終了処理
		CoUninitialize();

		return E_FAIL;
	}

	// マスターボイスの生成
	hr = m_pXAudio2->CreateMasteringVoice(&m_pMasteringVoice);
	if (FAILED(hr))
	{
		MessageBox(hWnd, "マスターボイスの生成に失敗！", "警告！", MB_ICONWARNING);

		if (m_pXAudio2 != NULL)
		{
			// XAudio2オブジェクトの開放
			m_pXAudio2->Release();
			m_pXAudio2 = NULL;
		}

		// COMライブラリの終了処理
		CoUninitialize();

		return E_FAIL;
	}

	// サウンドデータの初期化
	for (int nCntSound = 0; nCntSound < static_cast<int>(LABEL::MAX); nCntSound++)
	{
		HANDLE hFile;
		DWORD dwChunkSize = 0;
		DWORD dwChunkPosition = 0;
		DWORD dwFiletype;
		WAVEFORMATEXTENSIBLE wfx;
		XAUDIO2_BUFFER buffer;

		// バッファのクリア
		memset(&wfx, 0, sizeof(WAVEFORMATEXTENSIBLE));
		memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));

		// サウンドデータファイルの生成
		const char* pFilename = m_aSoundInfo[nCntSound].pFilename;
		hFile = CreateFile(pFilename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
		if (hFile == INVALID_HANDLE_VALUE)
		{
			MessageBox(hWnd, "サウンドデータファイルの生成に失敗！(1)", "警告！", MB_ICONWARNING);
			return HRESULT_FROM_WIN32(GetLastError());
		}
		if (SetFilePointer(hFile, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
		{// ファイルポインタを先頭に移動
			MessageBox(hWnd, "サウンドデータファイルの生成に失敗！(2)", "警告！", MB_ICONWARNING);
			return HRESULT_FROM_WIN32(GetLastError());
		}

		// WAVEファイルのチェック
		hr = CheckChunk(hFile, 'FFIR', &dwChunkSize, &dwChunkPosition);
		if (FAILED(hr))
		{
			MessageBox(hWnd, "WAVEファイルのチェックに失敗！(1)", "警告！", MB_ICONWARNING);
			return S_FALSE;
		}
		hr = ReadChunkData(hFile, &dwFiletype, sizeof(DWORD), dwChunkPosition);
		if (FAILED(hr))
		{
			MessageBox(hWnd, "WAVEファイルのチェックに失敗！(2)", "警告！", MB_ICONWARNING);
			return S_FALSE;
		}
		if (dwFiletype != 'EVAW')
		{
			MessageBox(hWnd, "WAVEファイルのチェックに失敗！(3)", "警告！", MB_ICONWARNING);
			return S_FALSE;
		}

		// フォーマットチェック
		hr = CheckChunk(hFile, ' tmf', &dwChunkSize, &dwChunkPosition);
		if (FAILED(hr))
		{
			MessageBox(hWnd, "フォーマットチェックに失敗！(1)", "警告！", MB_ICONWARNING);
			return S_FALSE;
		}
		hr = ReadChunkData(hFile, &wfx, dwChunkSize, dwChunkPosition);
		if (FAILED(hr))
		{
			MessageBox(hWnd, "フォーマットチェックに失敗！(2)", "警告！", MB_ICONWARNING);
			return S_FALSE;
		}

		// オーディオデータ読み込み
		hr = CheckChunk(hFile, 'atad', &m_aSizeAudio[nCntSound], &dwChunkPosition);
		if (FAILED(hr))
		{
			MessageBox(hWnd, "オーディオデータ読み込みに失敗！(1)", "警告！", MB_ICONWARNING);
			return S_FALSE;
		}
		m_apDataAudio[nCntSound] = (BYTE*)malloc(m_aSizeAudio[nCntSound]);
		hr = ReadChunkData(hFile, m_apDataAudio[nCntSound], m_aSizeAudio[nCntSound], dwChunkPosition);
		if (FAILED(hr))
		{
			MessageBox(hWnd, "オーディオデータ読み込みに失敗！(2)", "警告！", MB_ICONWARNING);
			return S_FALSE;
		}

		// ソースボイスの生成
		hr = m_pXAudio2->CreateSourceVoice(&m_apSourceVoice[nCntSound], &(wfx.Format));
		if (FAILED(hr))
		{
			MessageBox(hWnd, "ソースボイスの生成に失敗！", "警告！", MB_ICONWARNING);
			return S_FALSE;
		}

		// バッファの値設定
		memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
		buffer.AudioBytes = m_aSizeAudio[nCntSound];
		buffer.pAudioData = m_apDataAudio[nCntSound];
		buffer.Flags = XAUDIO2_END_OF_STREAM;
		buffer.LoopCount = m_aSoundInfo[nCntSound].nCntLoop;

		// オーディオバッファの登録
		m_apSourceVoice[nCntSound]->SubmitSourceBuffer(&buffer);

		// ファイルをクローズ
		CloseHandle(hFile);
	}

	return S_OK;
}

//=============================================================================
// 解放
//=============================================================================
void CSound::Release()
{
	if (m_pSound != nullptr)
	{
		// サウンドの終了処理
		m_pSound->Uninit();
	
		// メモリを解放
		delete m_pSound;

		// ポインタを初期化
		m_pSound = nullptr;
	}
}

//=============================================================================
// セグメント再生(再生中なら停止)
//=============================================================================
HRESULT CSound::Play(LABEL label)
{
	// ラベル指定を要素数に変換
	const int nNumElement{ static_cast<int>(label) };

	XAUDIO2_VOICE_STATE xa2state;
	XAUDIO2_BUFFER buffer;

	// バッファの値設定
	memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
	buffer.AudioBytes = m_aSizeAudio[nNumElement];
	buffer.pAudioData = m_apDataAudio[nNumElement];
	buffer.Flags = XAUDIO2_END_OF_STREAM;
	buffer.LoopCount = m_aSoundInfo[nNumElement].nCntLoop;

	// 状態取得
	m_apSourceVoice[nNumElement]->GetState(&xa2state);

	// 音量取得
	m_apSourceVoice[nNumElement]->SetVolume(m_aSoundInfo[nNumElement].fVolume);

	if (xa2state.BuffersQueued != 0)
	{// 再生中

		// 一時停止
		m_apSourceVoice[nNumElement]->Stop(0);

		// オーディオバッファの削除
		m_apSourceVoice[nNumElement]->FlushSourceBuffers();
	}

	// オーディオバッファの登録
	m_apSourceVoice[nNumElement]->SubmitSourceBuffer(&buffer);

	// 再生
	m_apSourceVoice[nNumElement]->Start(0);

	return S_OK;
}

//=============================================================================
// 遷移
//=============================================================================
void CSound::Transition(LABEL labelPrev, LABEL labelNext)
{
	// 次の曲指定が、最後に再生された曲なら処理しない
	if (m_LastTransition == labelNext)
	{
		return;
	}

	// ラベル指定を要素数に変換
	WORD wPrev = static_cast<WORD>(labelPrev);

	// ボリュームが減少していく
	m_aSoundInfo[wPrev].fVolume += -0.05f;

	// フェードアウトが済んだら
	if (m_aSoundInfo[wPrev].fVolume <= 0.0f)
	{
		// 曲のボリュームは戻しておく
		m_aSoundInfo[wPrev].fVolume = 1.0f;

		// この曲を停止
		Stop(labelPrev);

		// 次の曲を再生
		Play(labelNext);

		// 最後に遷移した曲を保持
		m_LastTransition = labelNext;
	}

	// ボリュームを反映
	m_apSourceVoice[wPrev]->SetVolume(m_aSoundInfo[wPrev].fVolume);
}

//=============================================================================
// セグメント停止(ラベル指定)
//=============================================================================
void CSound::Stop(LABEL label)
{
	// ラベル指定を要素数に変換
	const int nNumElement{ static_cast<int>(label) };

	XAUDIO2_VOICE_STATE xa2state;

	// 状態取得
	m_apSourceVoice[nNumElement]->GetState(&xa2state);

	// 音量取得
	m_apSourceVoice[nNumElement]->SetVolume(m_aSoundInfo[nNumElement].fVolume);

	if (xa2state.BuffersQueued != 0)
	{// 再生中
		// 一時停止
		m_apSourceVoice[nNumElement]->Stop(0);

		// オーディオバッファの削除
		m_apSourceVoice[nNumElement]->FlushSourceBuffers();
	}
}

//=============================================================================
// セグメント停止(全て)
//=============================================================================
void CSound::Stop(void)
{
	// 一時停止
	for (int nCntSound = 0; nCntSound < static_cast<int>(LABEL::MAX); nCntSound++)
	{
		if (m_apSourceVoice[nCntSound] != NULL)
		{
			// 一時停止
			m_apSourceVoice[nCntSound]->Stop(0);
		}
	}
}

//=============================================================================
// 音量設定
//=============================================================================
void CSound::SetVol(LABEL label)
{
	// ラベル指定を要素数に変換
	const int nNumElement{ static_cast<int>(label) };

	// 指定のサウンドの音量をセット
	m_apSourceVoice[nNumElement]->SetVolume(m_aSoundInfo[nNumElement].fVolume);
}

//=============================================================================
// 音量取得
//=============================================================================
float CSound::GetVol(LABEL label)
{
	// ラベル指定を要素数に変換
	const int nNumElement{ static_cast<int>(label) };

	return m_aSoundInfo[nNumElement].fVolume;
}

//=============================================================================
// サウンドを取得
//=============================================================================
CSound* CSound::GetInstance()
{
	if (m_pSound == nullptr)
	{
		// 生成
		m_pSound->Create();
	}

	return m_pSound;
}

//=============================================================================
// コンストラクタ
//=============================================================================
CSound::CSound() :
	m_pXAudio2{ nullptr },
	m_pMasteringVoice{ nullptr }
{
	m_apSourceVoice[static_cast<int>(LABEL::MAX)] = {};	// ソースボイス
	m_apDataAudio[static_cast<int>(LABEL::MAX)] = {};	// オーディオデータ
	m_aSizeAudio[static_cast<int>(LABEL::MAX)] = {};	// オーディオデータサイズ

	// サウンド情報を取得
	JSON Json = hws::OpenJsonFile("Data\\JSON\\sound_list.json");

	// ポインタ保持用
	static std::vector<std::string> vFilePath;

	// サウンド情報を割り当て
	for (WORD wCntSound = 0; wCntSound < static_cast<WORD>(LABEL::MAX); ++wCntSound)
	{
		// ファイルパスをキャストしポインタを残す
		vFilePath.push_back(static_cast<std::string>(Json["FilePath"][wCntSound]));

		// ループカウントをキャスト
		int nCntLoop = static_cast<int>(Json["CountLoop"][wCntSound]);

		// 音量をキャスト
		float fVolume = static_cast<float>(Json["Volume"][wCntSound]);

		// データをセット
		m_aSoundInfo[wCntSound] = { vFilePath[wCntSound].c_str(), nCntLoop, fVolume};
	}
}

//=============================================================================
// デストラクタ
//=============================================================================
CSound::~CSound()
{

}

//=============================================================================
// 生成
//=============================================================================
void CSound::Create()
{
	if (m_pSound != nullptr)
	{ // 二重生成禁止
		assert(false);
	}

	// インスタンスを生成
	m_pSound = DBG_NEW CSound{};
}

//=============================================================================
// 終了処理
//=============================================================================
void CSound::Uninit(void)
{
	// 一時停止
	for (int nCntSound = 0; nCntSound < static_cast<int>(LABEL::MAX); nCntSound++)
	{
		if (m_apSourceVoice[nCntSound] != NULL)
		{
			// 一時停止
			m_apSourceVoice[nCntSound]->Stop(0);

			// ソースボイスの破棄
			m_apSourceVoice[nCntSound]->DestroyVoice();
			m_apSourceVoice[nCntSound] = NULL;

			// オーディオデータの開放
			free(m_apDataAudio[nCntSound]);
			m_apDataAudio[nCntSound] = NULL;
		}
	}

	// マスターボイスの破棄
	m_pMasteringVoice->DestroyVoice();
	m_pMasteringVoice = NULL;

	if (m_pXAudio2 != NULL)
	{
		// XAudio2オブジェクトの開放
		m_pXAudio2->Release();
		m_pXAudio2 = NULL;
	}

	// COMライブラリの終了処理
	CoUninitialize();
}

//=============================================================================
// チャンクのチェック
//=============================================================================
HRESULT CSound::CheckChunk(HANDLE hFile, DWORD format, DWORD* pChunkSize, DWORD* pChunkDataPosition)
{
	HRESULT hr = S_OK;
	DWORD dwRead;
	DWORD dwChunkType;
	DWORD dwChunkDataSize;
	DWORD dwRIFFDataSize = 0;
	DWORD dwFileType;
	DWORD dwBytesRead = 0;
	DWORD dwOffset = 0;

	if (SetFilePointer(hFile, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{// ファイルポインタを先頭に移動
		return HRESULT_FROM_WIN32(GetLastError());
	}

	while (hr == S_OK)
	{
		if (ReadFile(hFile, &dwChunkType, sizeof(DWORD), &dwRead, NULL) == 0)
		{// チャンクの読み込み
			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		if (ReadFile(hFile, &dwChunkDataSize, sizeof(DWORD), &dwRead, NULL) == 0)
		{// チャンクデータの読み込み
			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		switch (dwChunkType)
		{
		case 'FFIR':
			dwRIFFDataSize = dwChunkDataSize;
			dwChunkDataSize = 4;
			if (ReadFile(hFile, &dwFileType, sizeof(DWORD), &dwRead, NULL) == 0)
			{// ファイルタイプの読み込み
				hr = HRESULT_FROM_WIN32(GetLastError());
			}
			break;

		default:
			if (SetFilePointer(hFile, dwChunkDataSize, NULL, FILE_CURRENT) == INVALID_SET_FILE_POINTER)
			{// ファイルポインタをチャンクデータ分移動
				return HRESULT_FROM_WIN32(GetLastError());
			}
		}

		dwOffset += sizeof(DWORD) * 2;
		if (dwChunkType == format)
		{
			*pChunkSize = dwChunkDataSize;
			*pChunkDataPosition = dwOffset;

			return S_OK;
		}

		dwOffset += dwChunkDataSize;
		if (dwBytesRead >= dwRIFFDataSize)
		{
			return S_FALSE;
		}
	}

	return S_OK;
}

//=============================================================================
// チャンクデータの読み込み
//=============================================================================
HRESULT CSound::ReadChunkData(HANDLE hFile, void* pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset)
{
	DWORD dwRead;

	if (SetFilePointer(hFile, dwBufferoffset, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{// ファイルポインタを指定位置まで移動
		return HRESULT_FROM_WIN32(GetLastError());
	}

	if (ReadFile(hFile, pBuffer, dwBuffersize, &dwRead, NULL) == 0)
	{// データの読み込み
		return HRESULT_FROM_WIN32(GetLastError());
	}

	return S_OK;
}