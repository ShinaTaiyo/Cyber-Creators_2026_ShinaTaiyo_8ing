//============================================================================
// 
// ２０２５年４月２８日：ブロッククラスを作成 [Target.cpp]
// Author : 椎名太陽
// 
//============================================================================

//****************************************************
// インクルードファイル
//****************************************************
#include "target.h"           // 元ヘッダ
#include "transparent_info.h" // 透明度情報

// 情報
#include "calculation.h" // 計算

// 入力用
#include "manager.h" // マネージャー

// カメラ取得用
#include "camera_manager.h"

// デバッグ表示用
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
CTarget::CTarget() :
	CObject_3D{ LAYER::UI },
	m_bControl{ true },
	m_Move{ useful::VEC3_INIT },
	m_pTransParent_Info{ DBG_NEW CTransparent_Info(60,60,false,1) },
	m_PosTarget{GetPos()},
	m_PosTargetOld{GetPos()},
	m_bUseMove{true},
	m_bUseAdjustCameraPos{true}
{
	SetType(CObject::TYPE::TARGET); // タイプを「ターゲット」に設定
}

//============================================================================
// デストラクタ
//============================================================================
CTarget::~CTarget()
{
	// 何もなし
}

//============================================================================
// 初期設定
//============================================================================
HRESULT CTarget::Init()
{
	// オブジェクト3Dの初期設定
	if (FAILED(CObject_3D::Init()))
	{
		assert(false && "プレイヤーの初期設定に失敗");
	}
     
	return S_OK;
}

//============================================================================
// 終了処理
//============================================================================
void CTarget::Uninit()
{
	// 透明度情報終了
	if (m_pTransParent_Info != nullptr)
	{
		delete m_pTransParent_Info;    // 破棄する
		m_pTransParent_Info = nullptr; // ポインタ初期化
	}

	// オブジェクト3Dの終了処理
	CObject_3D::Uninit();
}

//============================================================================
// 更新処理
//============================================================================
void CTarget::Update()
{
	D3DXCOLOR Col = GetCol(); // 色合い

	// 動かす
	Control(); 

	// オブジェクト3Dの更新
	CObject_3D::Update();

	// 透明度情報が存在するなら透明度を操作
	if (m_pTransParent_Info != nullptr)
	{
		m_pTransParent_Info->Update(Col);
	}

	AdjustCameraPos(); // カメラの位置を調整する

	SetCol(Col); // 色合い設定
}

//============================================================================
// 描画処理
//============================================================================
void CTarget::Draw()
{
	// デバイスを取得
	LPDIRECT3DDEVICE9 pDev = CRenderer::GetDeviece();

	//Zバッファに書き込まない
	pDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	// 深度を無視して描画順にします
	pDev->SetRenderState(D3DRS_ZENABLE, FALSE);

	// オブジェクト3Dの描画処理
	CObject_3D::Draw();

	//Zバッファに書き込む
	pDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	// 深度を無視して描画順にします
	pDev->SetRenderState(D3DRS_ZENABLE, TRUE);

}

//============================================================================
// 操作するかどうか取得
//============================================================================
const bool& CTarget::GetUseControl() const
{
	return m_bControl;
}

//============================================================================
// 操作するかどうか設定
//============================================================================
void CTarget::SetUseControl(bool bControl)
{
	m_bControl = bControl;
}

//============================================================================
// 生成
//============================================================================
CTarget* CTarget::Create(D3DXVECTOR3 InitPos)
{
	// インスタンスを生成
	CTarget* pTarget = DBG_NEW CTarget;

	// 生成失敗
	if (pTarget == nullptr)
	{
		assert(false);
	}

	// タイプを設定
	pTarget->SetType(TYPE::TARGET);

	// テクスチャ設定
	pTarget->BindTex(CTexture_Manager::GetInstance()->RefRegistry().BindAtKey("Target_000"));

	// 初期設定
	pTarget->Init();

	// 初期位置の設定
	pTarget->SetPos(InitPos);

	return pTarget;
}

//============================================================================
// 透明度情報を取得
//============================================================================
CTransparent_Info* CTarget::GetTransparent_Info() const
{
	return m_pTransParent_Info;
}

//============================================================================
// 目的地を設定
//============================================================================
void CTarget::SetPosTarget(D3DXVECTOR3 Pos)
{
	m_PosTarget = Pos;
}

//============================================================================
// 移動をするかどうか取得
//============================================================================
const bool& CTarget::GetUseMove() const
{
	return m_bUseMove;
}

//============================================================================
// 移動をするかどうか設定
//============================================================================
void CTarget::SetUseMove(bool bUse)
{
	m_bUseMove = bUse;
}

//============================================================================
// カメラの位置の調整を使用するかどうか取得
//============================================================================
const bool& CTarget::GetUseAdjustCameraPos() const
{
	return m_bUseAdjustCameraPos;
}

//============================================================================
// カメラの位置の調整を使用するかどうか設定
//============================================================================
void CTarget::SetUseAdjustCameraPos(bool bUse)
{
	m_bUseAdjustCameraPos = bUse;
}

//============================================================================
// 
// privateメンバ
// 
//============================================================================

//============================================================================
// 操作
//============================================================================
void CTarget::Control()
{
	// インプットを取得
	auto* pKeyboard = CManager::GetKeyboard();
	auto* pPad = CManager::GetPad();

	// === 処理に使用する変数宣言 ===

	float fRot = 0.0f;  // 向き
	float fX = 0.0f;    // X
	float fY = 0.0f;    // Y
	const D3DXVECTOR3& Pos = GetPos(); // 位置

	// === 移動処理開始 ===

	// 操作可能なら動かす
	if (m_bControl)
	{
		// 右
		if (pKeyboard->GetPress(DIK_D) ||
			pPad->GetPress(CInputPad::JOYKEY::RIGHT) ||
			pPad->GetJoyStickL().X > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
		{
			fX = 1.0f;
		}
		// 左
		if (pKeyboard->GetPress(DIK_A) ||
			pPad->GetPress(CInputPad::JOYKEY::LEFT) ||
			pPad->GetJoyStickL().X < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
		{
			fX = -1.0f;
		}
		// 上
		if (pKeyboard->GetPress(DIK_W) ||
			pPad->GetPress(CInputPad::JOYKEY::UP) ||
			pPad->GetJoyStickL().Y > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
		{
			fY = 1.0f;
		}
		// 下
		if (pKeyboard->GetPress(DIK_S) ||
			pPad->GetPress(CInputPad::JOYKEY::DOWN) ||
			pPad->GetJoyStickL().Y < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
		{
			fY = -1.0f;
		}
	}

	// 1f前の目的地の更新
	m_PosTargetOld = m_PosTarget;

	// 移動ボタンを押していた目的地を更新
	if (fX != 0.0f || fY != 0.0f)
	{
		fRot = atan2f(fX, fY);                                          
		m_Move = { sinf(fRot) * MOVESPEED,cosf(fRot) * MOVESPEED,0.0f };
		m_PosTarget += m_Move; // 目的地を更新
	}

	D3DXVECTOR3 Dir = m_PosTarget - Pos; // 目的地と現在位置のベクトル
	m_PosOld = Pos; // 1f前の位置を更新

	// 移動をする時だけ移動
	if (m_bUseMove)
	{
		SetPos(Pos + Dir * s_fMoveAdjust); // 目的地に向かって徐々に位置を更新
	}
	// 位置をデバッグ表示
	CRenderer::SetDebugString("ターゲット位置：X = " + to_string(Pos.x) + "Y = " + to_string(Pos.y) + "Z = " + to_string(Pos.z));

	LimitRange(); // 移動範囲制限
}

//============================================================================
// 画面外に出ないようにする
//============================================================================
void CTarget::LimitRange()
{
	// === 処理に使用する情報宣言 ===

	CCamera* pCamera = CCamera_Manager::GetInstance()->RefRegistry().BindAtKey("Game"); // カメラへのポインタ
	D3DXVECTOR3 PosTarget = m_PosTarget,   // 位置
                Size = GetSize(), // サイズ
	            PosVtxMax = PosTarget + Size, // 最大頂点の位置
	            PosVtxMin = PosTarget - Size, // 最小頂点の位置

	            // 最大頂点のスクリーン座標
                ScreenPos_VtxMax = Calculation::Pos::WorldToScreen(PosVtxMax, *pCamera->GetMtxView(), *pCamera->GetMtxProj(), FSCREEN_WIDTH, FSCREEN_HEIGHT),
	            
	            // 最小頂点のスクリーン座標
	            ScreenPos_VtxMin = Calculation::Pos::WorldToScreen(PosVtxMin, *pCamera->GetMtxView(), *pCamera->GetMtxProj(), FSCREEN_WIDTH, FSCREEN_HEIGHT),
	            
                TurnVec = m_PosTargetOld - PosTarget, // 戻らせるベクトル
	            Move = VEC3_INIT;         // 移動量
	bool bTurn = false; // 戻すかどうか

	// === 移動範囲制限処理開始 ===

	//// 最大最小頂点Xが画面外に出ていたら戻す
	//if (ScreenPos_VtxMax.x < 0.0f || ScreenPos_VtxMax.x > FSCREEN_WIDTH ||
	//	ScreenPos_VtxMin.x < 0.0f || ScreenPos_VtxMin.x > FSCREEN_WIDTH)
	//{
	//	Move.x = TurnVec.x * 1.0f;
	//	bTurn = true;
	//}
	//// 最大最小頂点Yが画面外に出ていたら戻す
	//if (ScreenPos_VtxMax.y < 0.0f || ScreenPos_VtxMax.y > FSCREEN_HEIGHT ||
	//	ScreenPos_VtxMin.y < 0.0f || ScreenPos_VtxMin.y > FSCREEN_HEIGHT)
	//{
	//	Move.y = TurnVec.y * 1.0f;
	//	bTurn = true;
	//}
	//// 最大最小頂点Zが画面外に出ていたら戻す
	//if (ScreenPos_VtxMax.z < 0.0f || ScreenPos_VtxMax.z > FSCREEN_WIDTH ||
	//	ScreenPos_VtxMin.z < 0.0f || ScreenPos_VtxMin.z > FSCREEN_WIDTH)
	//{
	//	Move.z = TurnVec.z * 1.0f;
	//	bTurn = true;
	//}

	if (PosTarget.x > s_fMoveLimitMaxX || PosTarget.x < s_fMoveLimitMinX)
	{
		Move.x = TurnVec.x * 1.0f;
		bTurn = true;
	}
	if (PosTarget.y > s_fMoveLimitMaxY || PosTarget.y < s_fMoveLimitMinY)
	{
		Move.y = TurnVec.y * 1.0f;
		bTurn = true;
	}

	// 1f前の位置へのベクトル分位置を戻す
	if (bTurn)
	{
		m_PosTarget += Move;
	}
}

//============================================================================
// カメラの位置を調整する
//============================================================================
void CTarget::AdjustCameraPos()
{
	if (!m_bUseAdjustCameraPos)
	{
		return;
	}

	const D3DXVECTOR3& SelfPos = GetPos(); // 自身の位置

	// カメラへのポインタ
	CCamera* pCamera = CCamera_Manager::GetInstance()->RefRegistry().BindAtKey("Game");

	// 現在の位置の半分の位置にカメラを設定
	pCamera->SetPos(D3DXVECTOR3(SelfPos.x, SelfPos.y, 0.0f) / 2);
}