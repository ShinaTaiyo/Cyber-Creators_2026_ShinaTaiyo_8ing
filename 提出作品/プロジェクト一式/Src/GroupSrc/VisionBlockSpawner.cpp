//============================================================================
// 
// ２０２５年６月１２日：視野妨害オブジェクト召喚クラスを作成 [VisionBlockSpawner.cpp]
// Author : 椎名太陽
// 
//============================================================================

//****************************************************************************
// インクルードファイル
//****************************************************************************
#include "game.h"          // ゲームシーン
#include "manager.h"       // マネージャー
#include "renderer.h"      // デバッグ用
#include "logger.h"        // ログ

// === オブジェクト ===

#include "VisionBlocker.h" // 視野妨害オブジェクト
#include "VisionBlockSpawner.h"   // 元ヘッダ

// === 情報 ===

#include "calculation.h"   // 計算情報
#include "move_info.h"     // 移動情報 

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
CVisionBlockSpawner::CVisionBlockSpawner() :
	CObject(LAYER::DEFAULT),
	m_nSpawnRate{ 0 },
	m_nCntFrame{ 0 },
	m_SpawnPoint{ VEC3_INIT },
	m_MaxSpawnRange{ VEC3_INIT },
	m_MinSpawnRange{ VEC3_INIT },
	m_BaseMove{ VEC3_INIT },
	m_fMaxLeafSpeed{1.0f},
	m_fMinLeafSpeed{-1.0f},
	m_fLeafAddRotRange{0.05f},
	m_fMaxLeafSwayAddAngle{0.1f},
	m_fMinLeafSwayAddAngle{-0.1f},
	m_nLeafLife{1}
{
	// 何もなし
}

//============================================================================
// デストラクタ
//============================================================================
CVisionBlockSpawner::~CVisionBlockSpawner()
{
	// 何もなし
}

//============================================================================
// 初期化処理
//============================================================================
HRESULT CVisionBlockSpawner::Init()
{
	SetType(CObject::TYPE::VISIONBLOCKER); // タイプ設定
	return S_OK;
}

//============================================================================
// 終了処理
//============================================================================
void CVisionBlockSpawner::Uninit()
{
	// 何もなし
}

//============================================================================
// 更新処理
//============================================================================
void CVisionBlockSpawner::Update()
{
	CLogger* pLogger = CLogger::GetInstance(); // ログへのポインタ

	m_nCntFrame++; // フレーム数カウント

	if (m_nCntFrame % m_nSpawnRate == 0)
	{
		// === 処理に使用する情報を宣言、初期化 ===

		D3DXVECTOR3
			SpawnPos = DecideSpawnPos(),     // 召喚位置決定
			LeafAddRot = DecideLeafAddRot(); // 葉っぱ向き加算量決定
		float
			fSpeed = DecideLeafSpeed(), // 葉っぱの速度決定
			fAddAngle = DecideLeafSwayAddAngle(); // 葉っぱを揺らす角度加算量決定

		// デバッグ表示
		pLogger->AddDuration(u8"視野妨害オブジェクトが召喚されました！", 1);

		// === 視野妨害オブジェクトを生成 ===

		CVisionBlocker* pVisionBlocker = CVisionBlocker::Create(m_LeafType, SpawnPos);
		pVisionBlocker->SetLeafSwaySpeed(fSpeed); // 揺らす速度設定
		pVisionBlocker->SetLeafSwayAddAngle(fAddAngle); // 揺らす角度加算量設定
		pVisionBlocker->GetMoveInfo()->SetMove(m_BaseMove); // 移動量を設定
		pVisionBlocker->SetAddRot(LeafAddRot); // 向き加算量設定
		pVisionBlocker->SetLife(m_nLeafLife); // 体力設定
	}
}

//============================================================================
// 描画処理
//============================================================================
void CVisionBlockSpawner::Draw()
{
	// 何もなし
}

//============================================================================
// 召喚レート取得
//============================================================================
const int& CVisionBlockSpawner::GetSpawnRate() const
{
	return m_nSpawnRate;
}

//============================================================================
// 召喚レート設定
//============================================================================
void CVisionBlockSpawner::SetSpawnRate(int nRate)
{
	m_nSpawnRate = nRate;
}

//============================================================================
// フレーム数
//============================================================================
const int& CVisionBlockSpawner::GetCntFrame() const
{
	return m_nCntFrame;
}

//============================================================================
// 召喚する基点取得
//============================================================================
const D3DXVECTOR3& CVisionBlockSpawner::GetSpawnPoint() const
{
	return m_SpawnPoint;
}

//============================================================================
// 召喚する基点設定
//============================================================================
void CVisionBlockSpawner::SetSpawnPoint(D3DXVECTOR3 SpawnPoint)
{
	m_SpawnPoint = SpawnPoint;
}

//============================================================================
// 召喚する範囲最大値取得
//============================================================================
const D3DXVECTOR3& CVisionBlockSpawner::GetMaxSpawnRange() const
{
	return m_MaxSpawnRange;
}

//============================================================================
// 召喚する範囲最大値設定
//===========================================================================
void CVisionBlockSpawner::SetMaxSpawnRage(D3DXVECTOR3 MaxSpawnRange)
{
	m_MaxSpawnRange = MaxSpawnRange;
}

//============================================================================
// 召喚する最低値取得
//===========================================================================
const D3DXVECTOR3& CVisionBlockSpawner::GetMinSpawnRange() const
{
	return m_MinSpawnRange;
}

//============================================================================
// 召喚する最低値設定
//===========================================================================
void CVisionBlockSpawner::SetMinSpawnRage(D3DXVECTOR3 MinSpawnRange)
{
	m_MinSpawnRange = MinSpawnRange;
}

//============================================================================
// ベースとなる移動量取得
//===========================================================================
const D3DXVECTOR3& CVisionBlockSpawner::GetBaseMove() const
{
	return m_BaseMove;
}

//============================================================================
// ベースとなる移動量設定
//===========================================================================
void CVisionBlockSpawner::SetBaseMove(D3DXVECTOR3 BaseMove)
{
	m_BaseMove = BaseMove;
}

//============================================================================
// 葉っぱ最大速度取得
//===========================================================================
const float& CVisionBlockSpawner::GetMaxLeafSpeed() const
{
	return m_fMaxLeafSpeed;
}

//============================================================================
// 葉っぱ最大速度設定
//===========================================================================
void CVisionBlockSpawner::SetMaxLeafSpeed(float fMaxSpeed)
{
	m_fMaxLeafSpeed = fMaxSpeed;
}

//============================================================================
// 葉っぱ最低速度取得
//===========================================================================
const float& CVisionBlockSpawner::GetMinLeafSpeed() const
{
	return m_fMinLeafSpeed;
}

//============================================================================
// 葉っぱ最低速度設定
//===========================================================================
void CVisionBlockSpawner::SetMinLeafSpeed(float fMinSpeed)
{
	m_fMinLeafSpeed = fMinSpeed;
}

//============================================================================
// 葉っぱを揺らす角度加算量最大値取得
//===========================================================================
const float& CVisionBlockSpawner::GetMaxLeafSwayAddAngle() const
{
	return m_fMaxLeafSwayAddAngle;
}

//============================================================================
// 葉っぱを揺らす角度加算量最大値設定
//===========================================================================
void CVisionBlockSpawner::SetMaxLeafSwayAddAngle(float fMaxAddAngle)
{
	m_fMaxLeafSwayAddAngle = fMaxAddAngle;
}

//============================================================================
// 葉っぱを揺らす角度加算量最低値取得
//===========================================================================
const float& CVisionBlockSpawner::GetMinLeafSwayAddAngle() const
{
	return m_fMinLeafSwayAddAngle;
}

//============================================================================
// 葉っぱを揺らす角度加算量最低値設定
//===========================================================================
void CVisionBlockSpawner::SetMinLeafSwayAddAngle(float fMinAddAngle)
{
	m_fMinLeafSwayAddAngle = fMinAddAngle;
}

//============================================================================
// 葉っぱ加算角度範囲取得
//===========================================================================
const float& CVisionBlockSpawner::GetLeafAddRotRange() const
{
	return m_fLeafAddRotRange;
}

//============================================================================
// 葉っぱ加算角度範囲設定
//===========================================================================
void CVisionBlockSpawner::SetLeafAddRotRange(float fRange)
{
	m_fLeafAddRotRange = fRange;
}

//============================================================================
// 葉っぱ体力取得
//===========================================================================
const int& CVisionBlockSpawner::GetLeafLife() const
{
	return m_nLeafLife;
}

//============================================================================
// 葉っぱ体力設定
//===========================================================================
void CVisionBlockSpawner::SetLeafLife(int nLife)
{
	m_nLeafLife = nLife;
}

//============================================================================
// 葉っぱ種類取得
//===========================================================================
const CVisionBlocker::TYPE& CVisionBlockSpawner::GetLeafType() const
{
	return m_LeafType;
}

//============================================================================
// 葉っぱ種類設定
//===========================================================================
void CVisionBlockSpawner::SetLeafType(CVisionBlocker::TYPE Type)
{
	m_LeafType = Type;
}

//============================================================================
// 生成処理
//===========================================================================
CVisionBlockSpawner* CVisionBlockSpawner::Create(
	D3DXVECTOR3 SpawnPoint, // 召喚基点
	D3DXVECTOR3 BaseMove,   // ベース移動量
	int nSpawnRate, // 召喚レート
	D3DXVECTOR3 MaxSpawnRange, // 召喚範囲最大
	D3DXVECTOR3 MinSpawnRange, // 召喚範囲最小
	float fMaxLeafSpeed, // 葉っぱ速度最大
	float fMinLeafSpeed, // 葉っぱ速度最低
	float fMaxLeafSwayAddAngle, // 葉っぱを揺らす角度加算量最大
	float fMinLeafSwayAddAngle, // 葉っぱを揺らす角度加算量最小
	float fLeafAddRotRange, // 葉っぱの向き加算範囲
	int nLeafLife, // 葉っぱの体力
	CVisionBlocker::TYPE LeafType // 葉っぱ種類
)
{
	// インスタンス生成
	CVisionBlockSpawner* pVisionBlockSpawner = DBG_NEW CVisionBlockSpawner;

	pVisionBlockSpawner->SetType(CObject::TYPE::VISIONBLOCKSPAWNER); // タイプ設定
	pVisionBlockSpawner->SetSpawnPoint(SpawnPoint); // 召喚位置設定
	pVisionBlockSpawner->SetBaseMove(BaseMove);     // ベースとなる移動量設定
	pVisionBlockSpawner->SetSpawnRate(nSpawnRate);  // 召喚レート設定
	pVisionBlockSpawner->SetMaxSpawnRage(MaxSpawnRange); // 召喚範囲最大値
	pVisionBlockSpawner->SetMinSpawnRage(MinSpawnRange); // 召喚範囲最低値
	pVisionBlockSpawner->SetMaxLeafSpeed(fMaxLeafSpeed); // 葉っぱ最大速度
	pVisionBlockSpawner->SetMinLeafSpeed(fMinLeafSpeed); // 葉っぱ最低速度
	pVisionBlockSpawner->SetMaxLeafSwayAddAngle(fMaxLeafSwayAddAngle); // 葉っぱを揺らす角度加算量最大
	pVisionBlockSpawner->SetMinLeafSwayAddAngle(fMinLeafSwayAddAngle); // 葉っぱを揺らす角度加算量最小
	pVisionBlockSpawner->SetLeafAddRotRange(fLeafAddRotRange); // 葉っぱ向き加算範囲
	pVisionBlockSpawner->SetLeafLife(nLeafLife); // 葉っぱの体力設定
	pVisionBlockSpawner->SetLeafType(LeafType); // 葉っぱ種類設定

	return pVisionBlockSpawner;
}

//============================================================================
//
// privateメンバ
//
//============================================================================

//============================================================================
// 召喚位置決定
//===========================================================================
D3DXVECTOR3 CVisionBlockSpawner::DecideSpawnPos()
{
	// === 視野妨害オブジェクト生成位置を決定 ===

	int nSpawnRangeX = static_cast<int>(m_MaxSpawnRange.x - m_MinSpawnRange.x);
	int nSpawnRangeY = static_cast<int>(m_MaxSpawnRange.y - m_MinSpawnRange.y);
	int nSpawnRangeZ = static_cast<int>(m_MaxSpawnRange.z - m_MinSpawnRange.z);
	
	// 召喚位置を乱数値で決める
	D3DXVECTOR3 SpawnPos =
	{
		static_cast<float>(rand() % nSpawnRangeX),
		static_cast<float>(rand() % nSpawnRangeY),
		static_cast<float>(rand() % nSpawnRangeZ)
	};

	// 召喚範囲に最低値と召喚基点を足して範囲内に収める
	SpawnPos += m_MinSpawnRange + m_SpawnPoint;

	return SpawnPos;
}

//============================================================================
// 葉っぱの速度決定
//===========================================================================
float CVisionBlockSpawner::DecideLeafSpeed()
{
	float fRand = static_cast<float>(rand()) / RAND_MAX; // 0.0〜1.0のfloat乱数
	float fDiff = m_fMaxLeafSpeed - m_fMinLeafSpeed;     // 最大最低速度の差分
	
	// 速度決定
	float fSpeed = fDiff * fRand + m_fMinLeafSpeed;

	return fSpeed;
}

//============================================================================
// 葉っぱを揺らす角度加算量決定
//===========================================================================
float CVisionBlockSpawner::DecideLeafSwayAddAngle()
{
	float fRand = static_cast<float>(rand()) / RAND_MAX; // 0.0〜1.0のfloat乱数
	float fDiff = m_fMaxLeafSwayAddAngle - m_fMinLeafSwayAddAngle; // 最大最低揺らす角度加算量の差分

	// 葉っぱを揺らす角度加算量を求める
	float fLeafSwayAddAngle = fDiff * fRand + m_fMinLeafSwayAddAngle;

	return fLeafSwayAddAngle;
}

//============================================================================
// 葉っぱ加算角度決定
//===========================================================================
D3DXVECTOR3 CVisionBlockSpawner::DecideLeafAddRot()
{
	float fLeafAddRot[3] = {}; // それぞれの軸の葉っぱ加算角度

	for (int nCnt = 0; nCnt < 3; nCnt++)
	{
		float fRand = static_cast<float>(rand()) / RAND_MAX; // 0.0〜1.0のfloat乱数
		float fMinus = rand() % 2 == 0 ? 1.0f : -1.0f; // マイナスにするかどうか

		// 軸の葉っぱ加算角度決定
		fLeafAddRot[nCnt] = fRand * m_fLeafAddRotRange * fMinus;
	}

	// 加算向き決定
	D3DXVECTOR3 LeafAddRot = fLeafAddRot;

	return LeafAddRot;
}
