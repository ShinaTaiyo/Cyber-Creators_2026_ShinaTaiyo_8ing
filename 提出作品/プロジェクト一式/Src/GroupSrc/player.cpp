//============================================================================
// 
// ２０２５年５月３０日：シーンごとのプレイヤーを作成 [player.cpp]
// Author : 福田歩希
// 
//============================================================================

//****************************************************
// インクルードファイル
//****************************************************

// === オブジェクト ===
#include "player.h" // プレイヤー（元ヘッダ）
#include "target.h" // ターゲット
#include "summon_manager.h" // 召喚マネージャー
#include "VisionBlocker.h"  // 視野妨害オブジェクト
#include "particle.h" // パーティクル
#include "StencilMask.h"

// === 入力用 ===
#include "manager.h"
#include "renderer.h"  // デバイスを使用

// === 情報 ===
#include "move_info.h" // 移動
#include "transparent_info.h" // 透明度
#include "calculation.h" // 計算

// === 状態 ===
#include "PlayerState_Returning.h"    // プレイヤー戻る
#include "PlayerState_BeforeThrust.h" // プレイヤー突き前
#include "PlayerState_Thrusting.h"    // プレイヤー突き
#include "summon_manager_state_start.h" // 召喚マネージャー召喚開始状態

// === その他 ===
#include "SummonGroupSelector.h" // 召喚グループ選択

#define SHADER_ACTIVE (1)  // シェーダーのonoff  0か1で変更できる

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
CPlayer::CPlayer() :
	CObject_X{ LAYER::DEFAULT },
	m_Velocity{ VEC3_INIT },
	m_RotTarget{ VEC3_INIT },
	m_pMesh{nullptr},
	m_pToonEffect{nullptr},
	m_pToonMap{nullptr},
	m_RimPowor{0.2f},
	m_RimIntensity{0.7f},
	m_RimThickness{0.45f},
	m_nAirPower{s_nINITAIRPOWER},
	m_nDamageFrame{0},
	m_pMoveInfo{DBG_NEW CMove_Info()},
	m_pTarget{CTarget::Create(VEC3_INIT)},
	m_pState{nullptr},
	m_Type{ CPlayer::TYPE::TUTORIAL },
	m_upSpace(nullptr)
{
	// ターゲット初期サイズ設定
	m_pTarget->SetInitSize(D3DXVECTOR3(30.0f, 30.0f, 0.0f));

	// タイプ設定
	CObject::SetType(CObject::TYPE::PLAYER);
}

//============================================================================
// デストラクタ
//============================================================================
CPlayer::~CPlayer()
{
	// 何もなし
}

//============================================================================
// 初期設定
//============================================================================
HRESULT CPlayer::Init()
{
	// 初期状態を設定
	SetState(DBG_NEW CPlayerState_Returning(this));

	// Xオブジェクトクラスの初期設定
	if (FAILED(CObject_X::Init()))
	{
		assert(false && "プレイヤーの初期設定に失敗");
	}
#if SHADER_ACTIVE
	auto pDevice = CRenderer::GetDeviece();  // デバイスの取得

#ifdef _DEBUG
	InitToonShader(pDevice, "Src/Shader/fx/ToonShader.fx");
#else
	InitToonShader(pDevice, "Src/Shader/fxo/ToonShader.fxo");
#endif // _DEBUG

#endif

	// 空間生成
	m_upSpace = make_unique<CSpace>();

	/* 仮として、プレイヤーモデル (拳) を画面奥方向に向けます */
	SetRot({ 0.0f, -D3DX_PI, 0.0f });
	SetRotTarget({ 0.0f, -D3DX_PI, 0.0f });

	// ターゲットの透明度の最大値を決定する
	CTransparent_Info* pTransParent_Info = m_pTarget->GetTransparent_Info(); // 透明度情報
	pTransParent_Info->SetMaxValue(CPlayerState_BeforeThrust::GetNormalAnticipationFrame());
	pTransParent_Info->SetSpeed(0);
	pTransParent_Info->SetUse(true);

	return S_OK;
}

//============================================================================
// 終了処理
//============================================================================
void CPlayer::Uninit()
{
	// Xオブジェクトクラスの終了処理
	CObject_X::Uninit();

	// 移動情報終了処理
	if (m_pMoveInfo != nullptr)
	{
		delete m_pMoveInfo;
		m_pMoveInfo = nullptr;
	}

	// プレイヤー状態終了処理
	if (m_pState)
	{
		delete m_pState;
		m_pState = nullptr;
	}

	// ターゲット終了処理
	if (m_pTarget != nullptr)
	{
		m_pTarget = nullptr; // ポインタを初期化（解放はオブジェクトリストで）
	}

#if SHADER_ACTIVE
	// メッシュの破棄
	if (m_pMesh) {
		m_pMesh->Release();
		m_pMesh = nullptr;
	}
	// シェーダーの破棄
	if (m_pToonEffect) {
		m_pToonEffect->Release();
		m_pToonEffect = nullptr;
	}
	// トゥーンマップの破棄
	if (m_pToonMap) {
		m_pToonMap->Release();
		m_pToonMap = nullptr;
	}
#endif
}

//============================================================================
// 更新処理
//============================================================================
void CPlayer::Update()
{
	// ダメージ状態
	Damage();

	// プレイヤー状態更新
	if (m_pState)
	{
		m_pState->Update(this);
	}

	// 手刀演出更新
	m_upSpace->Update();

	// オブジェクトXの更新処理
	CObject_X::Update();

	// パーティクルとの当たり判定
	CollisionParticle();

	// 視野妨害オブジェクトを吹っ飛ばす
	BlowVisionBlocker();

	// 気の値制限（この処理で気の値を最大最小の範囲に収めた後に手刀で気を消費します。）
	AirPowerLimit();

	// 回転
	Rotation();

#if SHADER_ACTIVE
	// トゥーンシェーダーの更新
	//UpdateToonShader();
#endif
}

//============================================================================
// 描画処理
//============================================================================
void CPlayer::Draw()
{
#if SHADER_ACTIVE
	UINT numPasses = 0;

	m_pToonEffect->SetTechnique("Outline");
	m_pToonEffect->Begin(&numPasses, 0);
	for (UINT i = 0; i < numPasses; i++)
	{
		m_pToonEffect->BeginPass(i);

		Draw_UsingEffect(m_pToonEffect);
		m_pToonEffect->EndPass();
	}
	m_pToonEffect->End();


	//m_pToonEffect->SetTechnique("RimLightOnly");
	//m_pToonEffect->Begin(&numPasses, 0);
	//for (UINT i = 0; i < numPasses; i++)
	//{
	//	m_pToonEffect->BeginPass(i);

	//	Draw_UsingEffect(m_pToonEffect);
	//	m_pToonEffect->EndPass();
	//}
	//m_pToonEffect->End();

	m_pToonEffect->SetTechnique("ToonShading");
	m_pToonEffect->Begin(&numPasses, 0);

	LPDIRECT3DDEVICE9 pDev = CRenderer::GetRenderer()->GetDeviece();

	// ステンシルマスクの影響を受けたくない場所で呼ぶ
	CStencilMask::SetOtherStencilAvoid();

	for (UINT i = 0; i < numPasses; i++)
	{
		m_pToonEffect->BeginPass(i);

		// 通常の Draw() ではなく、Effect用のを呼ぶ
		Draw_UsingEffect(m_pToonEffect);

		m_pToonEffect->EndPass();
	}
	m_pToonEffect->End();

	// ステンシル設定を元に戻す
	CStencilMask::SetOtherResetStencilState();
#endif

	//// Xオブジェクトクラスの描画処理
	//CObject_X::Draw();
}

//============================================================================
// 生成
//============================================================================
CPlayer* CPlayer::Create(D3DXVECTOR3 InitPos)
{
	// インスタンスを生成
	CPlayer* pPlayer = DBG_NEW CPlayer;

	if (pPlayer == nullptr)
	{ // 生成失敗
		assert(false);
	}

	// 初期設定
	pPlayer->Init();

	// 位置の設定
	pPlayer->SetPos(InitPos);

	// モデルを設定
	pPlayer->BindModel(CX_Manager::GetInstance()->RefRegistry().BindAtKey("Player"));

	// サイズを設定
	pPlayer->SetSize(pPlayer->GetModel()->Size);

	// 最大頂点設定
	pPlayer->SetBaseVtxMax(pPlayer->GetModel()->VtxMax);

	// 最小頂点設定
	pPlayer->SetBaseVtxMin(pPlayer->GetModel()->VtxMin);

	return pPlayer;
}

//============================================================================
// 
// privateメンバ
// 
//============================================================================

//============================================================================
// 手刀演出発動処理
//============================================================================
void CPlayer::ExecuteUpSpace()
{
	// このフレームでの空間描画を許可
	CRenderer::GetRenderer()->SetDrawSpace();

	// 空間の状態更新を開始する
	m_upSpace->Start();
}

//============================================================================
// 回転
//============================================================================
void CPlayer::Rotation()
{
	// 向き情報取得
	D3DXVECTOR3 rot = GetRot();

	// ブレーキ力
	float fStopEnergy = 0.1f;

	// 回転反映と回転量の減衰
	if (m_RotTarget.y - rot.y > D3DX_PI)
	{
		rot.y += ((m_RotTarget.y - rot.y) * fStopEnergy + (D3DX_PI * 1.8f));
	}
	else if (m_RotTarget.y - rot.y < -D3DX_PI)
	{
		rot.y += ((m_RotTarget.y - rot.y) * fStopEnergy + (D3DX_PI * -1.8f));
	}
	else
	{
		rot.y += ((m_RotTarget.y - rot.y) * fStopEnergy);
	}

	// 向き情報設定
	SetRot(rot);
}

//============================================================================
// ダメージ処理
//============================================================================
void CPlayer::Damage()
{
	// ダメージフレームが０以上ならダメージ処理を開始
	if (m_nDamageFrame > 0)
	{
		m_nDamageFrame--;

		// ２フレーム事に色を点滅させる
		m_nDamageFrame % 2 == 0 ? SetCol(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f)), SetUseCol(false) : SetUseCol(true);

		// ダメージフレームが０になったら処理を終わり、色合いを使用しない
		if (m_nDamageFrame == 0)
		{
			SetUseCol(false);
		}

		CCamera_Manager* pCManager = CCamera_Manager::GetInstance();
		pCManager->RefRegistry().BindAtKey("Game")->SetVibration(0.01f);
	}
}

//============================================================================
// 視野妨害オブジェクトを吹っ飛ばす
//============================================================================
void CPlayer::BlowVisionBlocker()
{
	CObject* pObj = GetTopObject(CObject::LAYER::DEFAULT); // デフォルトレイヤーの先頭リストを取得
	const D3DXVECTOR3& Pos = GetPos(); // 位置

	// 移動量の長さを求める
	float fMoveVecLength = D3DXVec3Length(&m_Move);

	// 移動量のベクトルの長さが1.0f以下なら処理を行わない
	if (fMoveVecLength < 1.0f)
	{
		return;
	}

	// 視野妨害オブジェクトを検索し吹っ飛ばす
	while (pObj != nullptr)
	{
		CObject* pNext = pObj->GetNext();// 次のオブジェクトを取得

		// 種類が「視野妨害オブジェクト」以外なら処理を中断
		if (pObj->GetType() != CObject::TYPE::VISIONBLOCKER)
		{
			pObj = pNext; // リストを次に進める
			continue;
		}

		// 視野妨害オブジェクトにダウンキャスト
		CVisionBlocker* pVisionBlocker = dynamic_cast<CVisionBlocker*>(pObj);

		// 視野妨害オブジェクトへのポインタが存在しないなら処理を中断
		if (!pVisionBlocker)
		{
			pObj = pNext; // リストを次に進める
			continue;
		}

		const D3DXVECTOR3& VisionBlockerPos = pVisionBlocker->GetPos();

		// 距離を求める
		float fLength = Calculation::Length::PointToPoint(Pos, VisionBlockerPos);

		// 一定距離以下で吹っ飛ばす
		if (fLength < s_fVisionBlockerBlowLength)
		{
			D3DXVECTOR3 Dir = VisionBlockerPos - Pos;
			D3DXVec3Normalize(&Dir, &Dir);
			pVisionBlocker->SetBlowMove(Dir * s_fVisionBlockerBlowSpeed);
			pVisionBlocker->SetBlowInertia(s_fVisionBlockerBlowInertia);
		}

		pObj = pNext;//リストを次に進める
	}

}

//============================================================================
// 気を最大値を超えないようにする
//============================================================================
void CPlayer::AirPowerLimit()
{
	m_nAirPower = Calculation::Value::LimitRange(m_nAirPower, 0, s_nMaxAirPower);
}

//============================================================================
// パーティクルとの当たり判定
//============================================================================
void CPlayer::CollisionParticle()
{
	CObject* pObj = CObject::GetTopObject(CObject::LAYER::DEFAULT);  // デフォルトレイヤーの先頭リストを取得

// === 判定処理開始 ===

// プレイヤーと判定するオブジェクトの検索
	while (pObj != nullptr)
	{
		CObject* pNext = pObj->GetNext();// 次のオブジェクトを取得

		if (pObj->GetType() != CObject::TYPE::PARTICLE)
		{
			pObj = pNext;
			continue;
		}

		CParticle* pParticle = dynamic_cast<CParticle*>(pObj);

		if (pParticle == nullptr)
		{
			pObj = pNext;
			continue;
		}

		// === 判定に使用する情報を宣言 ===

		D3DXVECTOR3 Vec = pParticle->GetPos() - GetPos(); // ベクトル
		float fVecLength = D3DXVec3Length(&Vec);          // ベクトルの長さ
		float fPlayerRadius = GetSize().x;            // プレイヤーの半径
		float fParticleRadius = pParticle->GetSize().x;   // パーティクルの半径

		// === 判定開始 ===

		// 球の当たり判定を行い、当たったらパーティクルを破棄
		// ※１：お互いの半径よりも距離が短ければ当たっている
		// ※２：プレイヤーにすぐ当たるのをさけるため、出現６０フレームより大きければ当たる
		if (fVecLength < fPlayerRadius + fParticleRadius && pParticle->GetCntFrame() > 60)
		{
			pParticle->SetRelease();
		}

		pObj = pNext;
	}
}

//============================================================================
// 気の最大値を取得
//============================================================================
const int& CPlayer::GetMaxAirPower()
{
	return s_nMaxAirPower;
}

//============================================================================
// 移動量取得
//============================================================================
const D3DXVECTOR3& CPlayer::GetMove() const
{
	return m_Move;
}
//============================================================================
// ターゲットへのポインタ取得
//============================================================================
CTarget* CPlayer::GetTarget() const
{
	return m_pTarget;
}


//============================================================================
// 気を取得する
//============================================================================
const int& CPlayer::GetAirPower() const
{
	return m_nAirPower;
}

//============================================================================
// 気を設定する
//============================================================================
void CPlayer::SetAirPower(int nPower)
{
	m_nAirPower = nPower;
}

//============================================================================
// プレイヤー状態取得
//============================================================================
CPlayerState* CPlayer::GetState() const
{
	return m_pState;
}

//============================================================================
// プレイヤー状態設定
//============================================================================
void CPlayer::SetState(CPlayerState* State)
{
	// ポインタが存在するなら破棄する
	if (m_pState != nullptr)
	{
		m_pState->Uninit(this); // 終了処理
		delete m_pState;
	}

	// 新しい状態を設定
	m_pState = State;
	m_pState->Init(this);
}

//============================================================================
// タイプ取得
//============================================================================
const CPlayer::TYPE& CPlayer::GetType() const
{
	return m_Type;
}

//============================================================================
// タイプ設定
//============================================================================
void CPlayer::SetType(TYPE Type)
{
	m_Type = Type;
}

//============================================================================
// ダメージ演出
//============================================================================
void CPlayer::SetDamage(int nFrame)
{
	if (m_nDamageFrame == 0)
	{
		m_nDamageFrame = nFrame; // ダメージ状態にするフレーム数を設定
		SetUseCol(true); // 色合いを使用する

		// 召喚マネージャーへのポインタ取得
		CSummon_Manager* pSummonManager = dynamic_cast<CSummon_Manager*>(CObject::FindSpecificObject(CObject::TYPE::SUMMONMANAGER)); 

		CSummon_Manager_State_Start* pStart = dynamic_cast<CSummon_Manager_State_Start*>(pSummonManager->GetState());

		// ステートなのでない時もあります。例外処理せずに処理を中断
		if (!pStart)
		{
			return;
		}

		CSummonGroupSelector* pGroupSelector = dynamic_cast<CSummonGroupSelector*>(pStart->GetSummonGroupSelector());

		// ダメージを受けたので難易度スコアを大幅に下げます。
		pGroupSelector->SetDifficultyScore(pGroupSelector->GetDifficultyScore() - 4);
		pStart->SetSuccessPhase(false); // 今回のフェーズは失敗状態にします。
	}
}

//============================================================================
// シェーダーの初期化
//============================================================================
bool CPlayer::InitToonShader(LPDIRECT3DDEVICE9 device, const char* fxFile)
{
	m_fxFilePath = fxFile; // パス保存

	LPD3DXBUFFER pErr = nullptr;
#ifdef _DEBUG
	// デバック時
	HRESULT hr = D3DXCreateEffectFromFileA(
		device, fxFile, nullptr, nullptr, D3DXSHADER_DEBUG, nullptr, &m_pToonEffect, &pErr);
#else	
	// リリース時にfxoを読み込む
	HRESULT hr = D3DXCreateEffectFromFileA(
		device,
		fxFile, // .fx ではなく .fxo を指定
		nullptr,
		nullptr,
		D3DXFX_NOT_CLONEABLE,  // バイナリから読み込むときのフラグ(意味：このエフェクトは複製不可である)
		nullptr,
		&m_pToonEffect,
		&pErr);
#endif	

	if (FAILED(hr))
	{
		if (pErr)
		{
			MessageBoxA(nullptr, (char*)pErr->GetBufferPointer(), "FX Compile Error", MB_OK);
			pErr->Release();
		}
		return false;
	}

	// トゥーンマップ読み込み
	if (FAILED(D3DXCreateTextureFromFileA(device, "Data/TEXTURE/ToonMap.png", &m_pToonMap)))
	{
		MessageBoxA(nullptr, "ToonMap.png の読み込みに失敗しました", "Error", MB_OK);
		return false;
	}

	return true;
}

//============================================================================
// シェーダーの描画
//============================================================================
void CPlayer::Draw_UsingEffect(LPD3DXEFFECT pEffect)
{
	auto pDevice = CRenderer::GetDeviece();  // デバイスの取得
	auto model = GetModel();   // モデルの取得
	CCamera* pCamera = CCamera_Manager::GetInstance()->RefRegistry().BindAtKey("Game");  // ゲームシーンのカメラの取得
	D3DXVECTOR3 cameraPos = pCamera->GetPos();  // カメラの位置を取得
	auto pLight = CManager::GetManager()->GetLight();  // ライトのポインタ取得
	D3DXVECTOR3 lightDir = pLight->GetLightDir();   // ライトの位置を取得
	bool bUseCol = GetUseCol();  // 色が変わったか

	// モデルのワールドマトリックスを使って WVP 作成
	D3DXMATRIX world, view, proj, wvp, vp;
	world = GetMtxWorld();
	D3DXMATRIX matWorldNoScale, matWorldInvTrans;

	pDevice->GetTransform(D3DTS_VIEW, &view);
	pDevice->GetTransform(D3DTS_PROJECTION, &proj);
	wvp = world * view * proj;
	vp = view * proj;
	m_pToonEffect->SetMatrix("WorldViewProj", &wvp);
	//m_pToonEffect->SetMatrix("gView", &view);
	//m_pToonEffect->SetMatrix("gProj", &proj);

	// ライトを正規化
	D3DXVec3Normalize(&lightDir, &lightDir);
	m_pToonEffect->SetVector("LightDirection", (D3DXVECTOR4*)&lightDir);
	m_pToonEffect->SetVector("gCameraPos", (D3DXVECTOR4*)&cameraPos);
	m_pToonEffect->SetFloat("RimPower", m_RimPowor);      // 強さ
	m_pToonEffect->SetFloat("RimIntensity", m_RimIntensity);  // 鋭さ
	m_pToonEffect->SetFloat("RimThickness", m_RimThickness);  // 太さ
	m_pToonEffect->SetFloat("OutlineThickness", 1.05f);   // アウトラインの太さ
	//	// エフェクト設定用スケール・回転・平行移動
	//D3DXVECTOR3 scale, translation;
	//D3DXQUATERNION rotation;

	//// モデル行列（World）
	//D3DXMatrixIdentity(&world); // 回転・移動が入る 

	//D3DXVECTOR3 eye(0, 2, -5), at(0, 0, 0), up(0, 1, 0);

	//// ビュー・プロジェクション行列
	//D3DXMatrixLookAtLH(&view,
	//	&eye,  // Eye
	//	&at,   // LookAt
	//	&up);  // Up

	//// 射影行列
	//D3DXMatrixPerspectiveFovLH(&proj, D3DX_PI / 4, 640.0f / 480.0f, 1.0f, 100.0f);

	//D3DXVECTOR3 Scaling = D3DXVECTOR3(1, 1, 1);  // スケール

	//D3DXMatrixDecompose(&scale, &rotation, &translation, &world);
	//D3DXMatrixTransformation(&matWorldNoScale, nullptr, nullptr, &Scaling, nullptr, &rotation, &translation);

	//// 法線変換用：Worldのスケーリングを除外して逆転置行列を作成
	//D3DXMatrixInverse(&matWorldInvTrans, nullptr, &world);
	//D3DXMatrixTranspose(&matWorldInvTrans, &matWorldInvTrans);

	//pEffect->SetMatrix("gWorldInverseTranspose", &matWorldInvTrans);  // 除外したスケーリングをエフェクトに入れる


	// マテリアルデータへのポインタを取得
	D3DXMATERIAL* pMat = (D3DXMATERIAL*)model->pBuffMat->GetBufferPointer();

	for (WORD wCntMat = 0; wCntMat < static_cast<WORD>(model->dwNumMat); wCntMat++)
	{
		// 適用する色（bUseColがtrueならGetCol()、falseなら元の色）
		D3DXCOLOR diffuseColor = bUseCol ? GetCol() : model->apColMat[wCntMat];

		// エフェクトに設定
		pEffect->SetVector("MaterialDiffuse", (D3DXVECTOR4*)&diffuseColor);

		// 一時的にマテリアルを作成して色だけ変更
		D3DMATERIAL9 tempMat = pMat[wCntMat].MatD3D;
		tempMat.Diffuse = diffuseColor;
		pDevice->SetMaterial(&tempMat);  // 元のマテリアルを壊さない

		// テクスチャ設定
		if (model->apTex[wCntMat]) {
			pEffect->SetBool("UseTexture", TRUE);
			pDevice->SetTexture(0, model->apTex[wCntMat]);
			pEffect->SetTexture("DiffuseTexture", model->apTex[wCntMat]);
		}
		else {
			pEffect->SetBool("UseTexture", FALSE);
		}

		// メッシュの描画
		model->pMesh->DrawSubset(wCntMat);
	}
}
