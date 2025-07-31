//============================================================================
// 
// ２０２５年６月２日：ゲーム用のプレイヤーを実装 [TutorialTutorialPlayer.cpp]
// Author : 椎名太陽
// 
//============================================================================

//****************************************************
// インクルードファイル
//****************************************************

// === その他 ===

#include "renderer.h" // デバッグ用
#include "Tutorial.h"     // ゲーム
#include "manager.h"

// カメラ取得用
#include "camera_manager.h"

// === オブジェクト ===

#include "TutorialPlayer.h"   // プレイヤー（元ヘッダ）
#include "block.h"        // ブロック
#include "bgmodel.h"      // 背景モデル
#include "object_X.h"     // オブジェクトX
#include "obstacle.h"     // 障害物
#include "HoleObstacle.h" // 穴あき障害物
#include "hole.h"         // 穴
#include "target.h"       // ターゲット
#include "particle_2D.h"  // パーティクル2D（手刀など）
#include "particle.h"     // パーティクル（セーフゾーン通過時など）
#include "zone.h"         // ゾーン（移動先表示、セーフゾーンなど）
#include "VisionBlocker.h"// 視野妨害オブジェクト
#include "logger.h"       // ログ
#include "PlayerState.h"  // プレイヤー状態
#include "PlayerState_BeforeThrust.h" // プレイヤー状態_突きをする前
#include "PlayerState_Returning.h" // プレイヤー状態＿戻る
// === 情報 ===

#include "calculation.h"       // 計算
#include "OBB.h"               // OBB情報
#include "RotatingCylinder.h"  // 回転円柱情報
#include "collision.h"         // 当たり判定ストラテジークラスのコンポーネントがある
#include "horming_info.h"      // ホーミング情報（パーティクルなど）
#include "bounding_box.h"      // Box判定情報
#include "transparent_info.h"  // 透明度情報（ターゲット透明度など）
#include "SummonGroupSelector.h" // 召喚グループ選択
#include "bounding_cylinder.h" // Cylinder判定情報
#include "summon_manager_state_start.h" // 召喚マネージャーの召喚開始状態
#include "camera_state_targetzoom.h"    // 目標位置にズームするカメラ状態

//****************************************************
// usingディレクティブ
//****************************************************
using namespace useful;

//============================================================================
// 
// publicメンバ
// 
//============================================================================

//============================================================================
// コンストラクタ
//============================================================================
CTutorialPlayer::CTutorialPlayer() :
	CPlayer()
{
	// 何もなし
}

//============================================================================
// デストラクタ
//============================================================================
CTutorialPlayer::~CTutorialPlayer()
{
	// 何もなし
}

//============================================================================
// 初期設定
//============================================================================
HRESULT CTutorialPlayer::Init()
{
	// プレイヤークラスの初期設定
	if (FAILED(CPlayer::Init()))
	{
		assert(false && "プレイヤーの初期設定に失敗");
	}

	// プレイヤータイプを「ゲーム」に設定
	CPlayer::SetType(CPlayer::TYPE::TUTORIAL);

	return S_OK;
}

//============================================================================
// 終了処理
//============================================================================
void CTutorialPlayer::Uninit()
{
	// プレイヤークラスの終了処理
	CPlayer::Uninit();
}

//============================================================================
// 更新処理
//============================================================================
void CTutorialPlayer::Update()
{
	// プレイヤー更新
	CPlayer::Update();
}

//============================================================================
// 描画処理
//============================================================================
void CTutorialPlayer::Draw()
{
	// プレイヤークラスの描画処理
	CPlayer::Draw();
}

//============================================================================
// 生成
//============================================================================
CTutorialPlayer* CTutorialPlayer::Create(D3DXVECTOR3 InitPos)
{
	// インスタンスを生成
	CTutorialPlayer* pTutorialPlayer = DBG_NEW CTutorialPlayer;

	if (pTutorialPlayer == nullptr)
	{ // 生成失敗
		assert(false);
	}

	// タイプを設定
	pTutorialPlayer->SetType(TYPE::TUTORIAL);

	// 初期設定
	pTutorialPlayer->Init();

	// 位置の設定
	pTutorialPlayer->SetPos(InitPos);

	// モデルを設定
	pTutorialPlayer->BindModel(CX_Manager::GetInstance()->RefRegistry().BindAtKey("Player"));

	// サイズを設定
	pTutorialPlayer->SetSize(pTutorialPlayer->GetModel()->Size);

	// 最大頂点設定
	pTutorialPlayer->SetBaseVtxMax(pTutorialPlayer->GetModel()->VtxMax);

	// 最小頂点設定
	pTutorialPlayer->SetBaseVtxMin(pTutorialPlayer->GetModel()->VtxMin);

	return pTutorialPlayer;
}