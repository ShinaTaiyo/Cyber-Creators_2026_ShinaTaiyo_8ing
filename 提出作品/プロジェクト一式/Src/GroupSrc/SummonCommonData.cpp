//============================================================================
// 
// ２０２５年６月２０日：召喚オブジェクトに共通のデータを持たせる [SummonCommonData.cpp]
// Author : 椎名太陽
// 
//============================================================================

//****************************************************
// インクルードファイル
//****************************************************
#include "SummonCommonData.h" 
#include "game.h"
#include "calculation.h"

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
// コンストラクタ（デフォルト)
//====================================================
CSummonCommonData::CSummonCommonData() : 
	m_pHostObj{nullptr}
{
	// 何もなし
}

//====================================================
// コンストラクタ
//====================================================
CSummonCommonData::CSummonCommonData(CObject* pHostObj) :
	m_Pos{ VEC3_INIT },
	m_Rot{ VEC3_INIT },
	m_Scale{ VEC3_INIT },
	m_LerpPosInfo{},
	m_ExponertialRotInfo{},
	m_LerpScaleInfo{},
	m_pHostObj{pHostObj}
{
	// 何もなし
}

//====================================================
// デストラクタ
//====================================================
CSummonCommonData::~CSummonCommonData()
{
	// 何もなし
}

//====================================================
// 更新処理
//====================================================
void CSummonCommonData::Update()
{
	Moving(); // 移動処理
	Rotating(); // 回転処理
	Scaling(); // 拡大処理
}

//====================================================
// 位置取得
//====================================================
const D3DXVECTOR3& CSummonCommonData::GetPos() const
{
	return m_Pos;
}

//====================================================
// 位置設定
//====================================================
void CSummonCommonData::SetPos(D3DXVECTOR3 Pos)
{
	m_Pos = Pos;
}

//====================================================
// 向き取得
//====================================================
const D3DXVECTOR3& CSummonCommonData::GetRot() const
{
	return m_Rot;
}

//====================================================
// 向き設定
//====================================================
void CSummonCommonData::SetRot(D3DXVECTOR3 Rot)
{
	m_Rot = Rot;
}

//====================================================
// 拡大率取得
//====================================================
const D3DXVECTOR3& CSummonCommonData::GetScale() const
{
	return m_Scale;
}

//====================================================
// 拡大率設定
//====================================================
void CSummonCommonData::SetScale(D3DXVECTOR3 Scale)
{
	m_Scale = Scale;
}

//====================================================
// 移動情報取得
//====================================================
CMove_Info& CSummonCommonData::GetMoveInfo()
{
	return m_MoveInfo;
}

//====================================================
// 位置線形補間情報
//====================================================
CVec3LerpInfo& CSummonCommonData::GetLerpPosInfo()
{
	return m_LerpPosInfo;
}

//====================================================
// 向き指数減衰情報
//====================================================
CVec3ExponertialInfo& CSummonCommonData::GetExponertialRotInfo()
{
	return m_ExponertialRotInfo;
}

//====================================================
// ジェイソン初期化処理
//====================================================
void CSummonCommonData::InitFromJson(JSON& Json)
{
	vector<float> vPos;    // 位置
	vector<float> vMove;   // 移動量
	vector<float> vRot;    // 向き
	vector<float> vScale;  // 拡大率
	vector<float> vRotation; // 回転量

	// 位置設定
	if (Json.contains("Pos"))
	{
		vPos = Json["Pos"].get<vector<float>>();
		// ハリセンボンの位置から召喚予定位置まで線形補間する
		m_Pos = D3DXVECTOR3(0.0f,0.0f,500.0f);
		m_LerpPosInfo.SetInitialize(30, m_Pos, D3DXVECTOR3(vPos[0], vPos[1], vPos[2]));
	}
	// 向き設定
	if (Json.contains("Rotation"))
	{
		vRot = Json["Rotation"].get<vector<float>>();
		// 最初の向きをランダムにし、徐々に目的の向きに指数減衰する
		m_Rot = Calculation::Value::RandVector3(-D3DXVECTOR3(s_fRandumRotRate, s_fRandumRotRate, s_fRandumRotRate), 
			D3DXVECTOR3(s_fRandumRotRate, s_fRandumRotRate, s_fRandumRotRate));
		m_ExponertialRotInfo.SetInitialize(0.15f,D3DXVECTOR3(vRot[0], vRot[1], vRot[2]));
	}
	// 拡大率設定
	if (Json.contains("Scale"))
	{
		vScale = Json["Scale"].get<vector<float>>();
		// 最初はかなり小さい拡大率に設定し、目的の拡大率に線形補間する
		SetScale(D3DXVECTOR3(0.01f, 0.01f, 0.01f));
		m_LerpScaleInfo.SetInitialize(45, GetScale(), D3DXVECTOR3(vScale[0], vScale[1], vScale[2]));
	}

	m_MoveInfo.SetMove(D3DXVECTOR3(0.0f, 0.0f, s_fNormalMoveZ)); // 移動量は固定します
}

//====================================================
// 名前取得
//====================================================
const std::string& CSummonCommonData::GetName() const
{
	return m_Name;
}

//====================================================
// 名前設定
//====================================================
void CSummonCommonData::SetName(std::string Name)
{
	m_Name = Name;
}

//============================================================================
// 
// privateメンバ
//
//============================================================================

//====================================================
// 移動処理
//====================================================
void CSummonCommonData::Moving()
{
	// 位置の線形補間を行う
	if (m_LerpPosInfo.GetUse())
	{
		// 指数減衰動目標位置更新
		m_LerpPosInfo.Update(m_Pos,m_pHostObj);
	}
	// 位置の線形補間をしないなら普通に移動する
	else
	{
		// 移動処理
		m_MoveInfo.Update(m_Pos, m_pHostObj);
	}
}

//====================================================
// 回転処理
//====================================================
void CSummonCommonData::Rotating()
{
	const bool& bUseExponertialRot = m_ExponertialRotInfo.GetUse(); // 向きの指数減衰をするかどうか

	// 向きの線形補間をするなら行う
	if (bUseExponertialRot)
	{
		m_ExponertialRotInfo.Update(m_Rot,m_pHostObj); // 目標の向きに指数減衰
	}
}

//====================================================
// 拡大処理
//====================================================
void CSummonCommonData::Scaling()
{
	m_LerpScaleInfo.Update(m_Scale,m_pHostObj); // 目標の拡大率に線形補間
}
