//============================================================================
// 
// ２０２５年５月１１日：召喚マネージャーを作る [Summon_Manager_State_state.cpp]
// Author : 椎名太陽
// 
//============================================================================

//****************************************************
// インクルードファイル
//****************************************************
#include "Summon_Manager_state.h" // 元ヘッダ
#include "game.h"                 // ゲームシーン
#include "pch.h"                  // 様々なインクルード情報を使う
#include "renderer.h"             // デバッグなど
#include "input_keyboard.h"       // キー入力情報
#include "manager.h"              // マネージャー
#include "calculation.h"          // 計算
#include "SummonGroupSelector.h"  // 召喚グループ選択

// === オブジェクト ===
#include "object_X.h"     // オブジェクトX
#include "object_3D.h"    // オブジェクト3D
#include "obstacle.h"     // 障害物
#include "player.h"       // プレイヤー
#include "HoleObstacle.h" // 穴空き障害物
#include "zone.h"         // ゾーン
#include "VisionBlocker.h" // 視野妨害オブジェクト
#include "VisionBlockSpawner.h" // 視野妨害オブジェクトスポナー
#include "StencilMask.h"  // ステンシルマスク

// === シェーダー ===
#include "SmokeEffect.h" // 霧
#include "ToneAdjust.h"  // 色調変化

// === 状態 ===
#include "PlayerState_BeforeThrust.h" // プレイヤー状態：突きをする前


//****************************************************
// usingディレクティブ
//****************************************************
using namespace useful;
using namespace std;

//============================================================================
//
// publicメンバ
//
//============================================================================

//====================================================
// コンストラクタ
//====================================================
CSummon_Manager_State::CSummon_Manager_State() : 
	m_nGroup{0},
	m_bJustSummon{false}
{
	std::fstream ReadingFile; // ファイル読み込み用

	ReadingFile.open("Data\\JSON\\SummonPattern_List.Json", std::ios::in); // 召喚パターンリストVer2のジェイソンファイルを開く

	// ファイルが正常に開けていたら召喚パターンリストを格納
	if (ReadingFile.good())
	{
		ReadingFile >> m_JsonSummonPatternList; // ジェイソン格納

		// 配列じゃなかったら処理を終了
		if (!m_JsonSummonPatternList.is_array())
		{
			assert(false && "召喚パターンリストが配列型じゃありません");
		}
	}

	ReadingFile.close(); // ファイルを閉じる
}

//====================================================
// デストラクタ
//====================================================
CSummon_Manager_State::~CSummon_Manager_State()
{
	// 何もなし
}

//====================================================
// 更新処理
//====================================================
void CSummon_Manager_State::Update(CSummon_Manager* pManager)
{
	m_bJustSummon = false; // ちょうど召喚したフラグをマイフレームリセット
	DestroyOutOfRange(); // 指定範囲外に出たオブジェクトを破棄する
}

//====================================================
// ジェイソン召喚情報取得
//====================================================
const JSON& CSummon_Manager_State::GetJsonSummonGroup() const
{
	return m_JsonSummonGroup;
}

//====================================================
// ジェイソン召喚パターンリストを取得
//====================================================
const JSON& CSummon_Manager_State::GetJsonSummonPatternList() const
{
	return m_JsonSummonPatternList;
}

//====================================================
// 選んでいるグループ番号取得
//====================================================
const int& CSummon_Manager_State::GetChooseGroup() const
{
	return m_nGroup;
}

//====================================================
// 選んでいるグループ番号設定
//====================================================
void CSummon_Manager_State::SetChooseGroup(int nChoose)
{
	m_nGroup = nChoose;
}

//====================================================
// グループ召喚処理
//====================================================
bool CSummon_Manager_State::SetSummonGroup(int nSummonGroup)
{
	// === 初期設定 ===
	m_bJustSummon = true; // ちょうど召喚した
	ChangeJsonSummonGroup(nSummonGroup); // 召喚グループを指定グループに変更
	const JSON& JsonSummonGroup = GetJsonSummonGroup(); // 召喚グループ
	SetAnticipationFrame(JsonSummonGroup); // 見切りフレーム数を設定する
	ReleaseAllVisionBlockSpawner(); // 全ての視野妨害オブジェクトスポナーを破棄する
	ResetEvent(); // イベント設定リセット

	// オブジェクト以外なら処理を終了します
	if (!JsonSummonGroup.is_object())
	{
		assert(false && "召喚グループがオブジェクト型じゃありません");
	}

	if (!JsonSummonGroup.contains("Objects"))
	{
		assert(false && "現在の召喚パターンにObjectのキーがありません");
		return false;
	}

	SetEvent(JsonSummonGroup); // イベント設定処理

	JSON JsonObj = JsonSummonGroup["Objects"]; // ジェイソンオブジェクト(召喚オブジェクト情報)

	// 指定グループ範囲ベースfor文
	for (auto& Obj : JsonObj)
	{
		string Name;  // オブジェクト名

		// 名前を読み込み生成するオブジェクトを判別
		if (Obj.contains("Name"))
		{
			Name = Obj["Name"];
		}

		// ※なぜvector型で読み込む？
		//→Jsonは配列を自動でvector型に変換する機能があるからです。D3DXVECTOR3には自動変換する機能がないです。

		// 召喚グループのオブジェクト単位のモデルパスで整合変換
		OldModelPathConverterForSummonPettern(Obj);

		// モデルパスを取得
		const std::string& ModelPath = static_cast<std::string>(Obj["ModelName"]);

		// 障害物
		if (Name == "Obstacle")
		{
			CObstacle* pObstacle = CObstacle::Create(ModelPath, CObstacle::OBSTACLETYPE::TYPE00_BLOCK, VEC3_INIT, true); // 障害物生成
			pObstacle->InitFromJson(Obj); // ジェイソン初期化処理
		}
		// セーフゾーン
		else if (Name == "SafeZone")
		{
			CZone* pZone = CZone::Create(CZone::TYPE::SAFEZONE, VEC3_INIT, D3DXVECTOR3(10.0f, 10.0f, 0.0f), true);
			pZone->InitFromJson(Obj); // ジェイソン初期化処理
		}
		// 穴あき障害物
		else if (Name == "HoleObstacle")
		{
			CHoleObstacle* pHoleObstacle = CHoleObstacle::Create(ModelPath, CObstacle::OBSTACLETYPE::TYPE00_BLOCK, VEC3_INIT, true);
			pHoleObstacle->InitFromJson(Obj); // ジェイソン初期化処理
		}
	};

	return true;
}

//====================================================
// ステート変更処理
//====================================================
void CSummon_Manager_State::ChengeState(CSummon_Manager_State* pState, CSummon_Manager* pManager)
{
	pManager->SetChengeState(pState);
}

//====================================================
// 召喚グループを変更する
//====================================================
void CSummon_Manager_State::ChangeJsonSummonGroup(int nChooseGroup)
{
	m_JsonSummonGroup.clear(); // 召喚グループジェイソンを初期化
	std::fstream ReadingFile;  // ファイル読み込み用

	m_nGroup = nChooseGroup; // 選択グループ番号変更

	// 召喚パターンリストから召喚パターンのジェイソンファイルのパスを指定し召喚
	int nSize = m_JsonSummonPatternList.size(); // モデルパスの数

	// 配列外アクセスしてたら処理を中断
	if (!Calculation::Value::IsAccessSafeForArray(m_nGroup, nSize))
	{
		assert(false && "召喚グループ変更処理で配列外アクセスです！");
		return;
	}

	// 召喚パターンリストにFilePassのキーがないので処理を中段
	if (!m_JsonSummonPatternList[m_nGroup].contains("FilePass"))
	{
		assert(false && "召喚パターンリストにファイルパスのキーがありません");
	}

	const std::string FileName = m_JsonSummonPatternList[m_nGroup]["FilePass"]; // 指定グループのパスの名前

	ReadingFile.open(FileName, std::ios::in); // 指定グループのジェイソンファイルを開く

	// ファイルが開けてなければ処理を終了する
	if (!ReadingFile.good())
	{
		assert(false && "ファイルが正常に開けていません");
		return;
	}

	ReadingFile >> m_JsonSummonGroup; // ジェイソン召喚グループ格納

	// レベル名のキーがなければ処理を終了
	if (!m_JsonSummonPatternList[m_nGroup].contains("LevelName"))
	{
		assert(false && "レベル名のキーがありません");
	}

	string DifficultyName = m_JsonSummonPatternList[m_nGroup]["LevelName"]; // レベル名

	ReadingFile.close(); // ファイルを閉じる
}

//====================================================
// イベント設定
//====================================================
void CSummon_Manager_State::SetEvent(const JSON& SummonPattern)
{
	// 「タグ」のキーが存在するならイベントを発生させる
	if (SummonPattern.contains("Tag"))
	{
		string EventTag = SummonPattern["Tag"];

		// 視野妨害オブジェクト（葉っぱ）を召喚する
		if (EventTag == "Leaf")
		{
			VisionBlockerEvent();
		}
		else if (EventTag == "Smork")
		{
			SmorkEvent();
		}
	}
}

//====================================================
// 視野妨害イベントを行う
//====================================================
void CSummon_Manager_State::VisionBlockerEvent()
{
	fstream ReadingFile; // ファイル読み込み用
	ReadingFile.open("Data\\JSON\\VisionBlockerPattern_List.json", ios::in);
	CSummon_Manager::DIFFICULTY DifficultyRank = CSummon_Manager::DIFFICULTY::NORMAL; // 難易度ランク（固定値はNORMAL）

	// ファイルが正常に読み込めていなかったら例外処理
	if (!ReadingFile.good())
	{
		assert(false && "視野妨害パターンリストが正常に読み込めていません");
	}

	JSON PatternList; // 視野妨害パターンリスト
	ReadingFile >> PatternList; // 視野妨害パターンを読み込む

	ReadingFile.close(); // ファイルを閉じる

	// 配列型で読み込むのでちがうなら例外処理
	if (!PatternList.is_array())
	{
		assert(false && "視野妨害パターンリストが配列型じゃありません");
	}

	string PatternFilePass = PatternList[static_cast<int>(DifficultyRank)]; // 今回のパターン

	ReadingFile.open(PatternFilePass, ios::in); // 視野妨害パターンリストの指定したパスのファイルを読み込む

	if (!ReadingFile.good())
	{
		assert(false && "視野妨害パターンが正常に読み込めていません");
	}

	JSON JsonPattern;
	ReadingFile >> JsonPattern; // 視野妨害パターンを読み込む

	ReadingFile.close(); // ファイルを閉じる

	// 読み込んだパターンの全ての要素を検索
	for (auto& Obj : JsonPattern)
	{
		vector<float> vSpawnPos = Obj["SpawnPoint"]; // 召喚位置
		vector<float> vBaseMove = Obj["BaseMove"];   // ベース移動量
		int nSpawnRate = Obj["SpawnRate"]; // 召喚レート
		vector<float> vMaxSpawnRange = Obj["SpawnRange"]["Max"]; // 最大召喚範囲
		vector<float> vMinSpawnRange = Obj["SpawnRange"]["Min"]; // 最小召喚範囲
		float fMaxLeafSpeed = Obj["LeafSpeed"]["Max"]; // 最大葉っぱ速度
		float fMinLeafSpeed = Obj["LeafSpeed"]["Min"]; // 最低葉っぱ速度
		float fMaxLeafSwayAddAngle = Obj["LeafSwayAddAngle"]["Max"]; // 最大葉っぱが揺れる向き加算量
		float fMinLeafSwayAddAngle = Obj["LeafSwayAddAngle"]["Min"]; // 最低葉っぱが揺れる向き加算量
		float fLeafAddRotRange = Obj["LeafAddRotRange"]; // 葉っぱを回転させる向き加算量範囲
		int nLife = Obj["Life"]; // 体力
		string LeafName = Obj["LeafType"]; // 葉っぱの種類名前
		CVisionBlocker::TYPE LeafType = CVisionBlocker::TYPE::LEAF00; // 葉っぱ種類

		// 葉っぱの名前によって種類を決定する
		if (LeafName == "Green")
		{
			LeafType = CVisionBlocker::TYPE::LEAF00;
		}
		else if (LeafName == "Yellow")
		{
			LeafType = CVisionBlocker::TYPE::LEAF01;
		}
		else if (LeafName == "Red")
		{
			LeafType = CVisionBlocker::TYPE::LEAF02;
		}

		// 視野妨害オブジェクトスポナーを生成
		CVisionBlockSpawner::Create(
			D3DXVECTOR3(vSpawnPos[0], vSpawnPos[1], vSpawnPos[2]),
			D3DXVECTOR3(vBaseMove[0], vBaseMove[1], vBaseMove[2]),
			nSpawnRate,
			D3DXVECTOR3(vMaxSpawnRange[0], vMaxSpawnRange[1], vMaxSpawnRange[2]),
			D3DXVECTOR3(vMinSpawnRange[0], vMinSpawnRange[1], vMinSpawnRange[2]),
			fMaxLeafSpeed,
			fMinLeafSpeed,
			fMaxLeafSwayAddAngle,
			fMinLeafSwayAddAngle,
			fLeafAddRotRange,
			nLife,
			LeafType);
	}
}

//====================================================
// 全ての視野妨害オブジェクトスポナーを消す
//====================================================
void CSummon_Manager_State::ReleaseAllVisionBlockSpawner()
{
	// デフォルトレイヤーのリストの先頭オブジェクトへのポインタを取得
	CObject* pObj = CObject::GetTopObject(CObject::LAYER::DEFAULT);

	// リストから視野妨害オブジェクトスポナーを探し、破棄する
	while (pObj)
	{
		CObject* pNext = pObj->GetNext(); // リストの次のオブジェクトへのポインタを取得
		
		// 視野妨害オブジェクトスポナー以外なら処理を中断しリストのポインタを次に進める
		if (pObj->GetType() != CObject::TYPE::VISIONBLOCKSPAWNER)
		{
			pObj = pNext;
			continue;
		}

		// 破棄する
		pObj->SetRelease();

		pObj = pNext; // リストのポインタを次に進める
	}
}

//====================================================
// 霧イベントを行う
//====================================================
void CSummon_Manager_State::SmorkEvent()
{
	CStencilMask::SetUseStencil(true); // ステンシルマスクを使用し、シルエットを表現
	CSmokeEffect::SetUseDraw(true); // 霧を描画する
	//CToneAdjust::SetUseDraw(true);  // 色調変化を描画する
}

//====================================================
// イベント設定リセット
//====================================================
void CSummon_Manager_State::ResetEvent()
{
	CStencilMask::SetUseStencil(false);  // ステンシルマスクのイベントをリセット
	CSmokeEffect::SetUseDraw(false); // 霧を描画しない
	//CToneAdjust::SetUseDraw(false);  // 色調変化を描画しない
}

//====================================================
// 一定範囲外に出た召喚オブジェクトを破棄する
//====================================================
void CSummon_Manager_State::DestroyOutOfRange()
{
	CObject* pObj = CObject::GetTopObject(CObject::LAYER::DEFAULT); // デフォルトレイヤーの先頭リストを取得

	// 破棄するオブジェクトをリストから検索
	while (pObj != nullptr)
	{
		CObject* pNext = pObj->GetNext(); // 次のオブジェクトを取得
		const CObject::TYPE Type = pObj->GetType(); // タイプ

		// タイプが障害物とゾーンと穴あき障害物以外なら処理をスキップ
		if (Type == CObject::TYPE::OBSTACLE ||
			Type == CObject::TYPE::ZONE ||
			Type == CObject::TYPE::HOLEOBSTACLE)
		{
			D3DXVECTOR3 OtherPos = VEC3_INIT; // 相手の位置

			// タイプが「ゾーン」ならオブジェクト3Dにダウンキャストして位置を取得
			if (Type == CObject::TYPE::ZONE)
			{
				CObject_3D* pObj3D = dynamic_cast<CObject_3D*>(pObj);
				OtherPos = pObj3D->GetPos();
			}
			// タイプが「障害物」または「穴あき障害物」ならオブジェクトXにダウンキャストして位置を取得
			else if (Type == CObject::TYPE::OBSTACLE ||
				Type == CObject::TYPE::HOLEOBSTACLE)
			{
				CObject_X* pObjX = dynamic_cast<CObject_X*>(pObj);
				OtherPos = pObjX->GetPos();
			}

			// 後ろ側より奥にいたら破棄する
			if (OtherPos.z < s_fNormal_ReleaseRangeZ)
			{
				pObj->SetRelease();
			}
		}

		pObj = pNext; // リストを次に進める
	}

}

//====================================================
// 見切りフレーム数を設定する
//====================================================
void CSummon_Manager_State::SetAnticipationFrame(const JSON& SummonPattern)
{
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(CObject::FindSpecificObject(CObject::TYPE::PLAYER)); // プレイヤーへのポインタ

    // 「見切りフレーム数」のキーが存在するなら見切りフレーム数を設定する
	if (SummonPattern.contains("AnticipationFrame"))
	{
		int nAnticipationFrame = SummonPattern["AnticipationFrame"];
		pPlayer->SetState(DBG_NEW CPlayerState_BeforeThrust(pPlayer, nAnticipationFrame));
	}
	// 「見切りフレーム数」のキーが存在しないので固定値を設定
	else
	{
		pPlayer->SetState(DBG_NEW CPlayerState_BeforeThrust(pPlayer, 120));
	}
}
