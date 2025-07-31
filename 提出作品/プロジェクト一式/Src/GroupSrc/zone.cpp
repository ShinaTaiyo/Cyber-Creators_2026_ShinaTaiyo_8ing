//============================================================================
// 
// ２０２５年５月９日：セーフゾーンクラスを作成 [Zone.cpp]
// Author : 椎名太陽
// 
//============================================================================

//****************************************************
// インクルードファイル
//****************************************************
#include "zone.h"
#include "summon_manager.h"
#include "bounding_box.h"
#include "SummonCommonData.h"
#include "main.h"
#include "summon_manager_state_start.h"
#include "StencilMask.h" // ステンシルマスク
#include "game.h"
#include "Vec3ExponertialInfo.h"
#include "calculation.h"
// オブジェクト生成用
#include "texture_manager.h"

// 入力用
#include "manager.h"

//****************************************************
// 静的メンバ宣言
//****************************************************
const std::string CZone::s_aSafeZoneTextureName[static_cast<int>(CZone::TYPE::MAX)] =
{
	"SafeZone_000"
};

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
CZone::CZone(bool bIsSummonManagerObj) :
	CObject_3D{ LAYER::DEFAULT },
	m_pCollisionInfo_Box{DBG_NEW CCollisionInfo_BoundingBox()},
	m_SummonCommonData(this),
	m_bIsSummonManagerObj{ bIsSummonManagerObj }
{
	// 召喚マネージャーが生成したオブジェクトなら
	if (m_bIsSummonManagerObj)
	{
		CSummon_Manager::AddNumObj(); // 召喚マネージャーが生成したオブジェクト数を増やす
	}

	SetType(CObject::TYPE::ZONE); // タイプを「セーフゾーン」に設定
}

//============================================================================
// デストラクタ
//============================================================================
CZone::~CZone()
{
	// 何もなし
}

//============================================================================
// 初期設定
//============================================================================
HRESULT CZone::Init()
{
	// 3Dオブジェクトクラスの初期設定
	if (FAILED(CObject_3D::Init()))
	{
		assert(false && "プレイヤーの初期設定に失敗");
	}

	return S_OK;
}

//============================================================================
// 終了処理
//============================================================================
void CZone::Uninit()
{
	// 召喚マネージャーが生成したオブジェクトなら
	if (m_bIsSummonManagerObj)
	{
		CSummon_Manager::SubNumObj(); // 召喚マネージャーが生成したオブジェクト数を減らす
	}

	// 箱判定情報終了
	if (m_pCollisionInfo_Box)
	{
		delete m_pCollisionInfo_Box;
		m_pCollisionInfo_Box = nullptr;
	}

	// 3Dオブジェクトクラスの終了処理
	CObject_3D::Uninit();
}

//============================================================================
// 更新処理
//============================================================================
void CZone::Update()
{
	// 召喚オブジェクト共通データ更新
	m_SummonCommonData.Update();
	
	// 召喚オブジェクト共通データから自身の情報を更新
	UpdateFromSummonCommonData();

	// 3Dオブジェクトクラスの更新
	CObject_3D::Update();

	const D3DXVECTOR3 Size = GetSize(); // サイズ

	// 箱判定情報を更新
	m_pCollisionInfo_Box->SetUpdateInfo(
		GetPos(),VEC3_INIT,VEC3_INIT,D3DXVECTOR3(Size.x, Size.y,5.0f),D3DXVECTOR3(Size.x,Size.y,5.0f),D3DXVECTOR3(-Size.x,-Size.y,-5.0f),
		false,false,false,false);
}

//============================================================================
// 描画処理
//============================================================================
void CZone::Draw()
{
	// 3Dオブジェクトクラスの描画処理
	CObject_3D::Draw();
}

//============================================================================
// ジェイソン初期化処理
//============================================================================
void CZone::InitFromJson(const JSON& Json)
{
	JSON CopyJson = Json;

	// 召喚オブジェクト共通データ初期化
	m_SummonCommonData.InitFromJson(CopyJson);

	// 召喚オブジェクト共通データから自身の情報を更新
	UpdateFromSummonCommonData();
}

//============================================================================
// 生成
//============================================================================
CZone* CZone::Create(TYPE Type, D3DXVECTOR3 InitPos, D3DXVECTOR3 Size, bool bIsSummmonManagerObj)
{
	// インスタンスを生成
	CZone* pZone = DBG_NEW CZone(bIsSummmonManagerObj);

	if (pZone == nullptr)
	{ // 生成失敗
		assert(false);
	}

	// タイプを設定
	pZone->SetType(CObject::TYPE::ZONE);

	// 初期設定
	pZone->Init();

	// 初期位置の設定
	pZone->SetPos(InitPos);

	// 初期サイズの設定
	pZone->SetInitSize(Size);
	pZone->SetSize(Size);

	// ゾーンの判定情報の設定
	pZone->m_pCollisionInfo_Box->SetUpdateInfo(InitPos, VEC3_INIT, VEC3_INIT,
		D3DXVECTOR3(Size.x, Size.y, 5.0f), D3DXVECTOR3(Size.x, Size.y, 5.0f), D3DXVECTOR3(-Size.x, -Size.y, -5.0f),
		false, false, false, false);

	// テクスチャを設定
	pZone->BindTex(CTexture_Manager::GetInstance()->RefRegistry().BindAtKey(s_aSafeZoneTextureName[static_cast<int>(Type)]));

	// アニメーション用パラメータを作成
	const int
		nChangeRate = 3;
	const float
		fWidth = (1.0f / 4.0f),
		fHeight = (1.0f / 4.0f);

	// このインスタンスにテクスチャアニメーターを合成
	Compose(pZone->RefTextureAnimator(),
		[nChangeRate, fWidth, fHeight]() -> up<CTextureAnimator>
		{
			up<CTextureAnimator> up = make_unique<CTextureAnimator>();

			up->SetChangeRate(nChangeRate);
			up->SetTexSplitWidth(fWidth);
			up->SetTexSplitHeight(fHeight);

			return up;
		});

	return pZone;
}

//============================================================================
// 判定情報取得
//============================================================================
CCollisionInfo* CZone::GetCollisionInfo() const
{
	return m_pCollisionInfo_Box;
}

//============================================================================
// 召喚マネージャーが生成したオブジェクトかどうか取得
//============================================================================
const bool& CZone::GetSummonManagerObj() const
{
	return m_bIsSummonManagerObj;
}

//============================================================================
// 召喚オブジェクト共通データ取得
//============================================================================
CSummonCommonData& CZone::GetSummonCommonData()
{
	return m_SummonCommonData;
}

//============================================================================
// 召喚オブジェクト共通データ設定
//============================================================================
void CZone::SetSummonCommonData(CSummonCommonData SummonCommonData)
{
	m_SummonCommonData = SummonCommonData;
}

//============================================================================
// 召喚オブジェクト共通データから自身のデータを更新する
//============================================================================
void CZone::UpdateFromSummonCommonData()
{
	SetPos(m_SummonCommonData.GetPos());
	SetRot(m_SummonCommonData.GetRot());
	SetScale(m_SummonCommonData.GetScale());
}