//============================================================================
// 
// ２０２５年４月２８日：ブロッククラスを作成 [Button.cpp]
// Author : 椎名太陽
// 
//============================================================================

//****************************************************
// インクルードファイル
//****************************************************
#include "Button.h"
#include "bounding_box.h"

// 入力用
#include "manager.h"
#include "renderer.h"

// テクスチャ取得用
#include "texture_manager.h"

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
CButton::CButton() :
	m_nAnimCnt{0},
	m_nMaxAnim{0},
	CObject_2D{ LAYER::UI }
{
	SetType(CObject::TYPE::BUTTON); // タイプを「ボタン」に設定
}

//============================================================================
// デストラクタ
//============================================================================
CButton::~CButton()
{

}

//============================================================================
// 初期設定
//============================================================================
HRESULT CButton::Init()
{
	// オブジェクト2Dクラスの初期設定
	if (FAILED(CObject_2D::Init()))
	{
		assert(false && "数字の初期設定に失敗");
	}

	return S_OK;
}

//============================================================================
// 終了処理
//============================================================================
void CButton::Uninit()
{
	// オブジェクト2Dクラスの終了処理
	CObject_2D::Uninit();
}

//============================================================================
// 更新処理
//============================================================================
void CButton::Update()
{
	// ５フレームごとにアニメーションを進める
	if (m_nAnimCnt % 5 == 0)
	{
		int nNowPatternU = GetNowPatternU(); // テクスチャ座標Uを取得
		nNowPatternU++;
		// 最大に達したら基に戻す
		if (nNowPatternU == m_nMaxAnim)
		{
			nNowPatternU = 0;
		}

		// テクスチャ座標Uを設定
		SetNowPatternU(nNowPatternU);
	}

	// オブジェクト2Dクラスの更新
	CObject_2D::Update();

	m_nAnimCnt++;
}

//============================================================================
// 描画処理
//============================================================================
void CButton::Draw()
{
	LPDIRECT3DDEVICE9 pDev = CRenderer::GetDeviece(); // デバイス取得

	pDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	pDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);

	// オブジェクト2Dクラスの描画処理
	CObject_2D::Draw();

	pDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	pDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

}

//============================================================================
// 生成
//============================================================================
CButton* CButton::Create(D3DXVECTOR3 InitPos)
{
	// インスタンスを生成
	CButton* pButton = DBG_NEW CButton();

	// 生成失敗
	if (pButton == nullptr)
	{
		assert(false);
	}


	pButton->Init(); // 初期化処理
	pButton->SetPos(InitPos);// 初期位置設定
	
	// === テクスチャ情報設定 ===

	pButton->BindTex(CTexture_Manager::GetInstance()->RefRegistry().BindAtKey("UIButton_000"));
	pButton->m_nMaxAnim = 5;
	pButton->SetTexWidth(0.2f);  // 横幅５分の１
	pButton->SetTexHeight(1.0f); // 高さ１
	pButton->SetNowPatternU(0);  // ０の位置にする
	pButton->SetNowPatternV(0);  // ０の位置にする

	return pButton;
}