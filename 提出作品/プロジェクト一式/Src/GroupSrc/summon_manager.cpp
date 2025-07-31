//============================================================================
// 
// ２０２５年５月１１日：召喚マネージャーを作る [summon_manager.cpp]
// Author : 福田歩希
// 
//============================================================================

//****************************************************
// インクルードファイル
//****************************************************
#include "summon_manager.h" // 元ヘッダ
#include "renderer.h" // デバッグ用
#include "summon_manager_state_start.h" // 召喚開始状態
#include "summon_manager_state.h" // 召喚状態

//****************************************************
// usingディレクティブ
//****************************************************
using namespace useful;

//****************************************************
// 静的メンバ宣言
//****************************************************
int CSummon_Manager::s_nNumObj = 0; // 召喚したオブジェクト数

//============================================================================
//
// publicメンバ
//
//============================================================================

//====================================================
// コンストラクタ
//====================================================
CSummon_Manager::CSummon_Manager() :
	m_pSummonManagerState{ DBG_NEW CSummon_Manager_State_Start() },
	CObject(CObject::LAYER::DEFAULT)
{
	// タイプ設定
	CObject::SetType(CObject::TYPE::SUMMONMANAGER);
}

//====================================================
// デストラクタ
//====================================================
CSummon_Manager::~CSummon_Manager()
{
	if (m_pSummonManagerState != nullptr)
	{// 召喚マネージャーステートを終了
		delete m_pSummonManagerState;    // 動的メモリを解放
		m_pSummonManagerState = nullptr; // ポインタを初期化
	}
}

//====================================================
// 初期化処理
//====================================================
HRESULT CSummon_Manager::Init()
{
	// 何もなし
	return S_OK;
}

//====================================================
// 終了処理
//====================================================
void CSummon_Manager::Uninit()
{
	// 何もなし
}

//====================================================
// 更新処理
//====================================================
void CSummon_Manager::Update()
{
	// 召喚マネージャーステートへのポインタが存在していたら
	if (m_pSummonManagerState != nullptr)
	{
		m_pSummonManagerState->Update(this);
	}
}

//====================================================
// 描画処理
//====================================================
void CSummon_Manager::Draw()
{
	// 何もなし
}

//====================================================
// ステート変更処理
//====================================================
void CSummon_Manager::SetChengeState(CSummon_Manager_State* pState)
{
	if (m_pSummonManagerState != nullptr)
	{// 召喚マネージャーステートを終了
		delete m_pSummonManagerState;    // 動的メモリを解放
		m_pSummonManagerState = pState;  // 新しいステートを設定
	}
}

//====================================================
// ステート取得
//====================================================
CSummon_Manager_State* CSummon_Manager::GetState() const
{
	return m_pSummonManagerState;
}

//====================================================
// 召喚マネージャー生成処理
//====================================================
CSummon_Manager* CSummon_Manager::Create()
{
	CSummon_Manager* pSummonManager = DBG_NEW CSummon_Manager(); // インスタンス生成
	pSummonManager->Init(); // 初期化処理

	return pSummonManager;
}

//====================================================
// 生成したオブジェクト数を増やす
//====================================================
void CSummon_Manager::AddNumObj()
{
	s_nNumObj++;
}

//====================================================
// 生成したオブジェクト数を減らす
//====================================================
void CSummon_Manager::SubNumObj()
{
	s_nNumObj--;
}

//====================================================
// 召喚マネージャー生成オブジェクト数取得
//====================================================
int CSummon_Manager::GetNumObj()
{
	return s_nNumObj;
}

//====================================================
// 召喚オブジェクトの動きを止める
//====================================================
void CSummon_Manager::StopSummonObj()
{
	// デフォルトレイヤーのリストの先頭ポインタを取得
	CObject* pObj = CObject::GetTopObject(CObject::LAYER::DEFAULT);

	// 召喚オブジェクトの更新を止める
	while (pObj)
	{
		// リストの次のオブジェクトへのポインタを取得
		CObject* pNext = pObj->GetNext();

		if (pObj->GetType() == CObject::TYPE::OBSTACLE ||
			pObj->GetType() == CObject::TYPE::HOLEOBSTACLE ||
			pObj->GetType() == CObject::TYPE::ZONE)
		{
			pObj->SetPause(true);
		}

		// リストを次に進める
		pObj = pNext;
	}
}
