//============================================================================
// 
// ２０２５年４月２８日：ブロッククラスを作成 [Obstacle.cpp]
// Author : 椎名太陽
// 
//============================================================================

//****************************************************
// インクルードファイル
//****************************************************
#include "Obstacle.h"
#include "bounding_box.h"
#include "summon_manager.h"
#include "move_info.h"
#include "StencilMask.h" // ステンシルマスク
#include "OBB.h"
#include "collision.h"
#include "SummonCommonData.h"
#include "summon_manager_state_start.h"
#include "main.h"
#include "game.h"
#include "calculation.h"
#include "Vec3LerpInfo.h"
#include "Vec3ExponertialInfo.h"

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
// コンストラクタ
//============================================================================
CObstacle::CObstacle(bool bIsSummonManagerObj) :
	CObject_X{ LAYER::DEFAULT },
	m_pCollisionInfo_Box{ DBG_NEW CCollisionInfo_BoundingBox() },
	m_Type{OBSTACLETYPE::TYPE00_BLOCK},
	m_bCanbeDestroy{true},
	m_bIsSummonManagerObj{bIsSummonManagerObj},
	m_pOBB{DBG_NEW COBB()},
	m_AddRot{VEC3_INIT},
	m_bUseAddRot{true},
	m_SummonCommonData(this)
{
	// 召喚マネージャーが生成したオブジェクトなら
	if (m_bIsSummonManagerObj)
	{
		CSummon_Manager::AddNumObj(); // 召喚マネージャーが生成したオブジェクト数を増やす
	}

	// 箱判定情報初期化
	m_pCollisionInfo_Box->SetUpdateInfo(GetPos(), VEC3_INIT, VEC3_INIT, GetSize(), GetVtxMax(), GetVtxMin(), false, false, false, false);
}

//============================================================================
// デストラクタ
//============================================================================
CObstacle::~CObstacle()
{
	// 何もなし
}

//============================================================================
// 初期設定
//============================================================================
HRESULT CObstacle::Init()
{
	// Xオブジェクトクラスの初期設定
	if (FAILED(CObject_X::Init()))
	{
		assert(false && "プレイヤーの初期設定に失敗");
	}

	SetType(CObject::TYPE::OBSTACLE); // タイプを「障害物」に設定

	return S_OK;
}

//============================================================================
// 終了処理
//============================================================================
void CObstacle::Uninit()
{
	// 箱判定情報終了
	if (m_pCollisionInfo_Box != nullptr)
	{
		delete m_pCollisionInfo_Box;   
		m_pCollisionInfo_Box = nullptr;
	}

	// OBB判定終了
	if (m_pOBB)
	{
		delete m_pOBB;
		m_pOBB = nullptr;
	}

	// 召喚マネージャーが生成したオブジェクトなら召喚マネージャーオブジェクト数をデクリメント
	if (m_bIsSummonManagerObj)
	{
		CSummon_Manager::SubNumObj();
	}

	// Xオブジェクトクラスの終了処理
	CObject_X::Uninit();
}

//============================================================================
// 更新処理
//============================================================================
void CObstacle::Update()
{
	// 拡散処理
	if (m_upSpreader)
	{
		CObstacle& r = *this;
		m_upSpreader->Spread(r);
	}

	// 召喚オブジェクト共通データ更新
	m_SummonCommonData.Update();

	// 召喚オブジェクト共通データから自身の情報を更新する
	UpdateFromSummonCommonData();

	Rotating(); // 回転処理

	// Xオブジェクトクラスの更新
	CObject_X::Update();

	// 判定情報を更新する
	UpdateCollisionInfo();
}

//============================================================================
// 描画処理
//============================================================================
void CObstacle::Draw()
{
	// ステンシルバッファの描画設定
	CStencilMask::SetOtherStencilState();

	// Xオブジェクトクラスの描画処理
	CObject_X::Draw();

	// ステンシルバッファの描画設定を元に戻す
	CStencilMask::SetOtherResetStencilState();
}

//============================================================================
// OBB情報取得
//============================================================================
COBB* CObstacle::GetOBB() const
{
	return m_pOBB;
}

//============================================================================
// ジェイソン初期化処理
//============================================================================
void CObstacle::InitFromJson(const JSON& Json)
{
	vector<float> vRotation; // 回転量

	JSON CopyJson = Json;

	// 召喚オブジェクト共通データ初期化
	m_SummonCommonData.InitFromJson(CopyJson);

	// 回転量を設定
	if (Json.contains("Rot"))
	{
		vRotation = Json["Rot"].get<vector<float>>();
		SetAddRot(D3DXVECTOR3(vRotation[0], vRotation[1], vRotation[2]));
	}

	// 召喚オブジェクト共通データから自身の情報を更新
	UpdateFromSummonCommonData();
}

//============================================================================
// 種類取得
//============================================================================
const CObstacle::OBSTACLETYPE& CObstacle::GetObstacleType() const
{
	return m_Type;
}

//============================================================================
// 種類設定
//============================================================================
void CObstacle::SetObstacleType(OBSTACLETYPE Type)
{
	m_Type = Type;
}

//============================================================================
// 向き加算量設定
//============================================================================
void CObstacle::SetAddRot(D3DXVECTOR3 AddRot)
{
	m_AddRot = AddRot;
}

//============================================================================
// 向き加算量取得
//============================================================================
const D3DXVECTOR3& CObstacle::GetAddRot() const
{
	return m_AddRot;
}

//============================================================================
// 向きを加算し続けるかどうか取得
//============================================================================
const bool& CObstacle::GetUseAddRot() const
{
	return m_bUseAddRot;
}

//============================================================================
// 向きを加算し続けるかどうか設定
//============================================================================
void CObstacle::SetUseAddRot(bool bUse)
{
	m_bUseAddRot = bUse;
}

//============================================================================
// 召喚オブジェクト共通データ取得
//============================================================================
CSummonCommonData& CObstacle::GetSummonCommonData()
{
	return m_SummonCommonData;
}

//============================================================================
// 召喚オブジェクト共通データ設定
//============================================================================
void CObstacle::SetSummonCommonData(CSummonCommonData SummonCommonData)
{
	m_SummonCommonData = SummonCommonData;
}

//============================================================================
// 破壊可能かどうか取得
//============================================================================
const bool& CObstacle::GetCanbeDestroy() const
{
	return m_bCanbeDestroy;
}

//============================================================================
// 破壊可能かどうか設定
//============================================================================
void CObstacle::SetCanbeDestroy(bool bDestroy)
{
	m_bCanbeDestroy = bDestroy;
}

//============================================================================
// 召喚マネージャーが生成したオブジェクトかどうか取得
//============================================================================
const bool& CObstacle::GetSummonManagerObj() const
{
	return m_bIsSummonManagerObj;
}

//============================================================================
// 生成
//============================================================================
CObstacle* CObstacle::Create(const std::string& modelpath, OBSTACLETYPE Type, D3DXVECTOR3 InitPos, bool bIsSummonManagerObj)
{
	// インスタンスを生成
	CObstacle* pObstacle = DBG_NEW CObstacle(bIsSummonManagerObj);

	// 生成失敗
	if (pObstacle == nullptr)
	{
		assert(false);
	}

	// タイプを設定
	pObstacle->SetType(TYPE::OBSTACLE);

	// 障害物タイプ設定
	pObstacle->SetObstacleType(Type);

	// 障害物タイプ設定
	pObstacle->SetObstacleType(CObstacle::OBSTACLETYPE::TYPE00_BLOCK);

	// 初期設定
	pObstacle->Init();

	// 初期位置の設定
	pObstacle->SetPos(InitPos);

	// モデルを設定
	pObstacle->BindModel(CX_Manager::GetInstance()->RefRegistry().BindAtPath(modelpath));

	// 色合いを設定
	//pObstacle->SetUseCol(true);                           // 色合いを使用
	//pObstacle->SetCol(D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f)); // 色合いを設定

	// サイズを設定
	pObstacle->SetSize(pObstacle->GetModel()->Size);

	// 最大頂点設定
	pObstacle->SetBaseVtxMax(pObstacle->GetModel()->VtxMax);

	// 最小頂点設定
	pObstacle->SetBaseVtxMin(pObstacle->GetModel()->VtxMin);

	// 箱判定情報初期化
	pObstacle->m_pCollisionInfo_Box->SetUpdateInfo(pObstacle->GetPos(), VEC3_INIT, VEC3_INIT, pObstacle->GetSize(), pObstacle->GetVtxMax(), pObstacle->GetVtxMin(), false, false, false, false);

	// OBB判定情報初期化
	COBB* pOBB = pObstacle->m_pOBB;
	pOBB->SetPos(pObstacle->GetPos());       // 位置設定
	pOBB->SetPosOld(pObstacle->GetPos());    // 1f前の位置設定
	pOBB->SetVtxMax(pObstacle->GetVtxMax()); // 最大頂点設定
	pOBB->SetVtxMin(pObstacle->GetVtxMin()); // 最小頂点設定

	return pObstacle;
}

//============================================================================
// 
// privateメンバ
// 
//============================================================================

//============================================================================
// 位置編集
//============================================================================
void CObstacle::ControlInfo_Pos()
{
	float fX = 0.0f; // X移動量
	float fZ = 0.0f; // Y移動量
	CInputKeyboard* pInputKeyboard = CManager::GetManager()->GetKeyboard(); // キー入力情報
	const D3DXVECTOR3 Pos = GetPos(); // 位置
	CRenderer::SetDebugString("位置編集（十字キー）：X = " + to_string(Pos.x) + "、Y = " + to_string(Pos.y) + "、Z = " + to_string(Pos.z));

	// === 移動方向決定 ===

	if (pInputKeyboard->GetPress(DIK_UP))
	{
		fZ = 1.0f;
	}
	if (pInputKeyboard->GetPress(DIK_DOWN))
	{
		fZ = -1.0f;
	}
	if (pInputKeyboard->GetPress(DIK_RIGHT))
	{
		fX = 1.0f;
	}
	if (pInputKeyboard->GetPress(DIK_LEFT))
	{
		fX = -1.0f;
	}

	// Lシフトを押していなければ上下左右
	if (!pInputKeyboard->GetPress(DIK_LSHIFT))
	{
		if (fX != 0.0f || fZ != 0.0f)
		{// XY
			float fRotXZ = atan2f(fX, fZ); // 移動角度XZ
			SetPos(GetPos() + D3DXVECTOR3(sinf(fRotXZ) * 1.0f,cosf(fRotXZ) * 1.0f,0.0f));
		}
	}
	// Lシフトを押していれば前後移動
	else
	{
		if (fZ != 0.0f)
		{// Z
			SetPos(GetPos() + D3DXVECTOR3(0.0f,0.0f, 1.0f * fZ));
		}
	}
}

//============================================================================
// 判定情報を更新する
//============================================================================
void CObstacle::UpdateCollisionInfo()
{
	D3DXVECTOR3 Pos = GetPos(); // 位置（引数に渡した先で変わる）
	const D3DXVECTOR3& CurrentPos = GetPos(); // 参照用位置（位置を設定した後もその位置を使用できる）
	const D3DXVECTOR3& Rot = GetRot();   // 向き
	const D3DXVECTOR3& Size = GetSize(); // サイズ
	const D3DXVECTOR3& VtxMax = GetVtxMax(); // 最大頂点
	const D3DXVECTOR3& VtxMin = GetVtxMin(); // 最小頂点

	// 箱判定情報更新
	m_pCollisionInfo_Box->SetUpdateInfo(CurrentPos, VEC3_INIT, VEC3_INIT, Size, VtxMax, VtxMin, false, false, false, false);

	// OBB判定情報更新
	m_pOBB->SetPos(CurrentPos); // 位置を更新
	m_pOBB->SetVtxMax(VtxMax);  // 最大頂点を更新
	m_pOBB->SetVtxMin(VtxMin);  // 最小頂点を更新
	m_pOBB->SetRot(Rot);        // 向きを更新
}

//============================================================================
// 召喚オブジェクト共通データから自身の情報を更新する
//============================================================================
void CObstacle::UpdateFromSummonCommonData()
{
	SetPos(m_SummonCommonData.GetPos());
	SetRot(m_SummonCommonData.GetRot());
	SetScale(m_SummonCommonData.GetScale());
}

//============================================================================
// 回転処理
//============================================================================
void CObstacle::Rotating()
{
	// 向きの加算を開始
	if (m_bUseAddRot && !m_SummonCommonData.GetExponertialRotInfo().GetUse())
	{
		const D3DXVECTOR3& SelfRot = GetRot();
		SetRot(SelfRot + m_AddRot);

		// 召喚オブジェクト共通データの向きも更新
		m_SummonCommonData.SetRot(SelfRot);
	}
}

//============================================================================
// 拡散
//============================================================================
void CObstacle::CSpreader::Spread(CObstacle& r)
{
	Vec3 TmpRot = r.GetRot();
	TmpRot += (m_Dir * m_fRate) * 0.015f;
	r.SetRot(TmpRot);

	// 召喚オブジェクト共通データの向きを更新
	r.GetSummonCommonData().SetRot(r.GetRot());

	Vec3 TmpPos = r.GetPos();
	TmpPos += m_Dir * m_fRate;
	r.SetPos(TmpPos);

	// 召喚オブジェクト共通データの位置を更新
	r.GetSummonCommonData().SetPos(r.GetPos());
}