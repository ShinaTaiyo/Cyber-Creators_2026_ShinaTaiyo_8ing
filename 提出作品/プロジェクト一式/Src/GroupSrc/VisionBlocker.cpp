//============================================================================
// 
// ２０２５年６月１２日：視野妨害処理を作成 [VisionBlocker.cpp]
// Author : 椎名太陽
// 
//============================================================================

//****************************************************
// インクルードファイル
//****************************************************

// オブジェクト
#include "VIsionBlocker.h"
#include "StencilMask.h"

// 情報
#include "move_info.h"

// 入力用
#include "manager.h"
#include "renderer.h"
#include "input_keyboard.h"

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
// 静的メンバ宣言
//============================================================================

// 障害物のそれぞれのモデルタイプを宣言
const std::string CVisionBlocker::s_aVisionBlockerModelType[static_cast<int>(CVisionBlocker::TYPE::MAX)] =
{
	"Leaf_000",
	"Leaf_001",
	"Leaf_002",
};

//============================================================================
// コンストラクタ
//============================================================================
CVisionBlocker::CVisionBlocker() :
	CObject_X{ LAYER::DEFAULT },
	m_Type{TYPE::LEAF00},
    m_fLeafSwayAngle{0.0f},
	m_fLeafSwayAddAngle{0.0f},
	m_fLeafSwaySpeed{0.0f},
	m_AddRot{VEC3_INIT},
	m_nLife{1},
	m_BlowMove{VEC3_INIT},
	m_fBlowInertia{0.1f},
	m_pMove_Info{DBG_NEW CMove_Info()}
{
	SetType(TYPE::LEAF00); // タイプを「葉０」に設定
}

//============================================================================
// デストラクタ
//============================================================================
CVisionBlocker::~CVisionBlocker()
{
	// 何もなし
}

//============================================================================
// 初期設定
//============================================================================
HRESULT CVisionBlocker::Init()
{
	// Xオブジェクトクラスの初期設定
	if (FAILED(CObject_X::Init()))
	{
		assert(false && "プレイヤーの初期設定に失敗");
	}
	return S_OK;
}

//============================================================================
// 終了処理
//============================================================================
void CVisionBlocker::Uninit()
{
	// 移動情報終了
	if (m_pMove_Info != nullptr)
	{
		delete m_pMove_Info;
		m_pMove_Info = nullptr;
	}

	// Xオブジェクトクラスの終了処理
	CObject_X::Uninit();
}

//============================================================================
// 更新処理
//============================================================================
void CVisionBlocker::Update()
{
	D3DXVECTOR3 Pos = GetPos(); // 位置
	const D3DXVECTOR3& CurrentPos = GetPos(); // 参照位置

	// 移動情報の更新を行う
	if (m_pMove_Info)
	{
		D3DXVECTOR3 LeafMove = { cosf(m_fLeafSwayAngle) * m_fLeafSwaySpeed,0.0f,0.0f }; // 葉っぱの移動量に揺れる移動量を加算
		m_fLeafSwayAngle += m_fLeafSwayAddAngle; // 葉っぱを揺らす角度加算
		m_pMove_Info->Update(Pos, this); //	移動情報更新
		SetPos(Pos); // 位置設定
		SetPos(CurrentPos + LeafMove);   // 揺らす移動量で位置を加算
	}

	// 吹っ飛ばし処理
	Blow();

	// 向きを加算する
	SetRot(GetRot() + m_AddRot);

	// 体力がなくなったら破棄する
	if (m_nLife <= 0)
	{
		SetRelease();
	}

	// Xオブジェクトクラスの更新
	CObject_X::Update();

	// 体力を減らす
	m_nLife--;
}

//============================================================================
// 描画処理
//============================================================================
void CVisionBlocker::Draw()
{
	// ステンシルバッファの描画設定
	CStencilMask::SetOtherStencilState();

	// Xオブジェクトクラスの描画処理
	CObject_X::Draw();

	// ステンシルバッファの描画設定を元に戻す
	CStencilMask::SetOtherResetStencilState();
}

//============================================================================
// 種類設定
//============================================================================
void CVisionBlocker::SetType(TYPE Type)
{
	m_Type = Type;
}

//============================================================================
// 葉っぱを揺らす角度取得
//============================================================================
const float& CVisionBlocker::GetLeafSwayAngle() const
{
	return m_fLeafSwayAngle;
}

//============================================================================
// 葉っぱを揺らす角度設定
//============================================================================
void CVisionBlocker::SetLeafSwayAngle(float fAngle)
{
	m_fLeafSwayAngle = fAngle;
}

//============================================================================
// 葉っぱを揺らす角度加算量取得
//============================================================================
const float& CVisionBlocker::GetLeafSwayAddAngle() const
{
	return m_fLeafSwayAddAngle;
}

//============================================================================
// 葉っぱを揺らす角度加算量設定
//============================================================================
void CVisionBlocker::SetLeafSwayAddAngle(float fAddAngle)
{
	m_fLeafSwayAddAngle = fAddAngle;
}

//============================================================================
// 葉っぱを揺らす速度取得
//============================================================================
const float& CVisionBlocker::GetLeafSwaySpeed() const
{
	return m_fLeafSwaySpeed;
}

//============================================================================
// 葉っぱを揺らす速度設定
//============================================================================
void CVisionBlocker::SetLeafSwaySpeed(float fSwaySpeed)
{
	m_fLeafSwaySpeed = fSwaySpeed;
}

//============================================================================
// 角度加算量取得
//============================================================================
const D3DXVECTOR3& CVisionBlocker::GetAddRot() const
{
	return m_AddRot;
}

//============================================================================
// 角度加算量設定
//============================================================================
void CVisionBlocker::SetAddRot(D3DXVECTOR3 AddRot)
{
	m_AddRot = AddRot;
}

//============================================================================
// 吹っ飛ばし移動量取得
//============================================================================
const D3DXVECTOR3& CVisionBlocker::GetBlowMove() const
{
	return m_BlowMove;
}

//============================================================================
// 吹っ飛ばし移動量設定
//============================================================================
void CVisionBlocker::SetBlowMove(D3DXVECTOR3 Move)
{
	m_BlowMove = Move;
}

//============================================================================
// 吹っ飛ばし慣性取得
//============================================================================
const float& CVisionBlocker::GetBlowInertia() const
{
	return m_fBlowInertia;
}

//============================================================================
// 吹っ飛ばし慣性設定
//============================================================================
void CVisionBlocker::SetBlowInertia(float fInertia)
{
	m_fBlowInertia = fInertia;
}

//============================================================================
// 種類取得
//============================================================================
const CVisionBlocker::TYPE& CVisionBlocker::GetType() const
{
	return m_Type;
}

//============================================================================
// 移動情報取得
//============================================================================
CMove_Info* CVisionBlocker::GetMoveInfo() const
{
	return m_pMove_Info;
}

//============================================================================
// 体力取得
//============================================================================
const int& CVisionBlocker::GetLife() const
{
	return m_nLife;
}

//============================================================================
// 体力設定
//============================================================================
void CVisionBlocker::SetLife(int nLife)
{
	m_nLife = nLife;
}

//============================================================================
// 生成
//============================================================================
CVisionBlocker* CVisionBlocker::Create(TYPE Type, D3DXVECTOR3 InitPos)
{
	// インスタンスを生成
	CVisionBlocker* pVisionBlocker = DBG_NEW CVisionBlocker();

	// 生成失敗
	if (pVisionBlocker == nullptr)
	{
		assert(false);
	}

	// タイプを設定
	pVisionBlocker->CObject::SetType(CObject::TYPE::VISIONBLOCKER);

	// 障害物タイプ設定
	pVisionBlocker->SetType(Type);

	// 初期設定
	pVisionBlocker->Init();

	// 初期位置の設定
	pVisionBlocker->SetPos(InitPos);

	// モデルを設定
	pVisionBlocker->BindModel(CX_Manager::GetInstance()->RefRegistry().BindAtKey(s_aVisionBlockerModelType[static_cast<int>(Type)]));

	// サイズを設定
	pVisionBlocker->SetSize(pVisionBlocker->GetModel()->Size);

	// 最大頂点設定
	pVisionBlocker->SetBaseVtxMax(pVisionBlocker->GetModel()->VtxMax);

	// 最小頂点設定
	pVisionBlocker->SetBaseVtxMin(pVisionBlocker->GetModel()->VtxMin);

	return pVisionBlocker;
}

//============================================================================
// 吹っ飛ばし処理
//============================================================================
void CVisionBlocker::Blow()
{
	const D3DXVECTOR3& CurrentPos = GetPos(); // 参照位置

	// 吹っ飛ばし移動量を設定
	SetPos(CurrentPos + m_BlowMove);

	// 吹っ飛ばし移動量減衰
	m_BlowMove += (VEC3_INIT - m_BlowMove) * m_fBlowInertia;
}
