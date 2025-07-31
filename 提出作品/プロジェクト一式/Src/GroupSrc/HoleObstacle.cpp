//============================================================================
// 
// ２０２５年６月１１日：穴あき障害物クラスを作成 [HoleObstacle.cpp]
// Author : 椎名太陽
// 
//============================================================================

//****************************************************
// インクルードファイル
//****************************************************
#include "HoleObstacle.h"
#include "hole.h"
#include "bounding_box.h"
#include "summon_manager.h"
#include "move_info.h"
#include "OBB.h"
#include "collision.h"
#include "RotatingCylinder.h"

// 入力用
#include "manager.h"
#include "renderer.h"
#include "input_keyboard.h"
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

//============================================================================
// 静的メンバ宣言
//============================================================================

//============================================================================
// コンストラクタ
//============================================================================
CHoleObstacle::CHoleObstacle(bool bIsSummonManagerObj) :
	CObstacle(bIsSummonManagerObj),
	m_pHole{CHole::Create(VEC3_INIT)}
{
	// 何もなし
}

//============================================================================
// デストラクタ
//============================================================================
CHoleObstacle::~CHoleObstacle()
{
	// 何もなし
}

//============================================================================
// 初期設定
//============================================================================
HRESULT CHoleObstacle::Init()
{
	// 障害物の初期設定
	if (FAILED(CObstacle::Init()))
	{
		assert(false && "プレイヤーの初期設定に失敗");
	}

	SetType(CObject::TYPE::HOLEOBSTACLE); // タイプを「穴あき障害物」に設定

	return S_OK;
}

//============================================================================
// 終了処理
//============================================================================
void CHoleObstacle::Uninit()
{
	// 穴の終了処理
	if (m_pHole)
	{
		m_pHole->SetRelease(); // 死亡フラグを設定
		m_pHole = nullptr;
	}

	// 障害物の終了処理
	CObstacle::Uninit();
}

//============================================================================
// 更新処理
//============================================================================
void CHoleObstacle::Update()
{
	CObstacle::Update();    // 障害物更新

	m_pHole->SetParentPos(GetPos()); // 穴に親の位置を設定する
	m_pHole->SetParentRot(GetRot()); // 穴に親の向きを設定する
	m_pHole->ExtraUpdate(); // 穴更新（オブジェクトからは呼ばれないので手動で更新）
}

//============================================================================
// 描画処理
//============================================================================
void CHoleObstacle::Draw()
{
	// 障害物の描画処理
	CObstacle::Draw();
}

//============================================================================
// ジェイソン初期化処理
//============================================================================
void CHoleObstacle::InitFromJson(const JSON& Json)
{
	// === 処理に使用する情報を宣言、初期化 ===

	vector<float> vHolePos;
	vector<float> vHoleRot;
	vector<float> vHoleScale;
	CRotatingCylinder* pHoleRotCyl = m_pHole->GetRotatingCylinder(); // 穴の回転円柱情報

	// === ジェイソン初期化処理開始 ===

	// 穴の位置設定
	if (Json.contains("HoleOffset"))
	{
		vHolePos = Json["HoleOffset"].get<vector<float>>();
		m_pHole->SetPos(D3DXVECTOR3(vHolePos[0], vHolePos[1], vHolePos[2]));
		pHoleRotCyl->SetPos(GetPos()); // 穴の回転円柱位置設定
	}

	// 穴の向き設定
	if (Json.contains("HoleRot"))
	{
		vHoleRot = Json["HoleRot"].get<vector<float>>();
		m_pHole->SetRot(D3DXVECTOR3(vHoleRot[0], vHoleRot[1], vHoleRot[2]));
		pHoleRotCyl->SetPos(GetRot()); // 穴の回転円柱向き設定
	}

	// 穴の拡大率設定
	if (Json.contains("HoleScale"))
	{
		vHoleScale = Json["HoleScale"].get<vector<float>>();
		m_pHole->SetScale(D3DXVECTOR3(vHoleScale[0], vHoleScale[1], vHoleScale[2]));
	}

	CObstacle::InitFromJson(Json); // 障害物ジェイソン初期化処理
}

//============================================================================
// 穴を取得
//============================================================================
CHole* CHoleObstacle::GetHole() const
{
	return m_pHole;
}

//============================================================================
// 生成
//============================================================================
CHoleObstacle* CHoleObstacle::Create(const std::string& modelpath, CObstacle::OBSTACLETYPE Type, D3DXVECTOR3 InitPos, bool bIsSummonManagerObj)
{
	// インスタンスを生成
	CHoleObstacle* pHoleObstacle = DBG_NEW CHoleObstacle(bIsSummonManagerObj);

	// 生成失敗
	if (pHoleObstacle == nullptr)
	{
		assert(false);
	}

	// タイプを設定
	pHoleObstacle->SetType(TYPE::HOLEOBSTACLE);

	// 初期設定
	pHoleObstacle->Init();

	// 初期位置の設定
	pHoleObstacle->SetPos(InitPos);

	// モデルを設定
	pHoleObstacle->BindModel(CX_Manager::GetInstance()->RefRegistry().BindAtPath(modelpath));

	// サイズを設定
	pHoleObstacle->SetSize(pHoleObstacle->GetModel()->Size);

	// 最大頂点設定
	pHoleObstacle->SetBaseVtxMax(pHoleObstacle->GetModel()->VtxMax);

	// 最小頂点設定
	pHoleObstacle->SetBaseVtxMin(pHoleObstacle->GetModel()->VtxMin);

	// OBB判定情報初期化
	COBB* pOBB = pHoleObstacle->GetOBB();        // OBB情報へのポインタ
	pOBB->SetPos(pHoleObstacle->GetPos());       // 位置設定
	pOBB->SetPosOld(pHoleObstacle->GetPos());    // 1f前の位置設定
	pOBB->SetVtxMax(pHoleObstacle->GetVtxMax()); // 最大頂点設定
	pOBB->SetVtxMin(pHoleObstacle->GetVtxMin()); // 最小頂点設定

	return pHoleObstacle;
}