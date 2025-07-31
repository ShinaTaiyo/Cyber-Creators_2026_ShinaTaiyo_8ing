//============================================================================
// 
// ２０２５年７月１０日：召喚関係のバグの改善とプレイヤー条件分岐をステートパターンにする [PlayerState_Thrusting.h]
// Author : 椎名太陽
// 
//============================================================================

//****************************************************
// インクルード
//****************************************************

// === オブジェクト ===

#include "GamePlayer.h"
#include "target.h"
#include "object.h"
#include "zone.h"
#include "obstacle.h"
#include "HoleObstacle.h"
#include "hole.h"
#include "particle.h"

// === シーン ===

#include "game.h" // ゲーム

// === 状態 ===

#include "PlayerState_Thrusting.h"
#include "summon_manager_state.h" // 召喚マネージャー状態
#include "camera_state_targetzoom.h"
#include "PlayerState_Returning.h" // プレイヤー状態＿戻る
#include "summon_manager_state_start.h" // 召喚マネージャー召喚開始状態
#include "camera_state_normal.h" // カメラ通常状態

// === 情報 ===

#include "OBB.h" // OBB判定
#include "bounding_box.h" // 箱判定
#include "move_info.h"    // 移動
#include "horming_info.h" // ホーミング

// システム
#include "renderer.h"
#include "logger.h"
#include "calculation.h"
#include "sound.h"

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
CPlayerState_Thrusting::CPlayerState_Thrusting(CPlayer* pPlayer, D3DXVECTOR3 GoalPos) :
	CPlayerState(pPlayer),
	m_GoalPos{ GoalPos },
	m_bGoalPosArrived{ false },
	m_bAvoidObstacle{ false },
	m_bCollisionObstacle{false},
	m_bNowCollision{false},
	m_pOBB{DBG_NEW COBB()},
	m_pCollisionInfo_Box{DBG_NEW CCollisionInfo_BoundingBox()}
{
	// 何もなし
}

//============================================================================
// デストラクタ
//============================================================================
CPlayerState_Thrusting::~CPlayerState_Thrusting()
{
	// 箱判定情報終了
	if (m_pCollisionInfo_Box)
	{
		delete m_pCollisionInfo_Box;
		m_pCollisionInfo_Box = nullptr;
	}

	// OBB判定情報終了
	if (m_pOBB)
	{
		delete m_pOBB;
		m_pOBB = nullptr;
	}
}

//============================================================================
// 初期化処理
//============================================================================
void CPlayerState_Thrusting::Init(CPlayer* pPlayer)
{
	const D3DXVECTOR3&
		PlayerPos = pPlayer->GetPos(), // プレイヤー位置
		PlayerRot = pPlayer->GetRot(), // プレイヤー向き
		PlayerSize = pPlayer->GetSize(), // プレイヤーサイズ
		PlayerVtxMax = pPlayer->GetVtxMax(), // プレイヤー最大頂点
		PlayerVtxMin = pPlayer->GetVtxMin(); // プレイヤー最小頂点

	// 箱判定情報を設定
	m_pCollisionInfo_Box->SetUpdateInfo(PlayerPos, VEC3_INIT, VEC3_INIT, PlayerSize, PlayerVtxMax, PlayerVtxMin, false, false, false, false);

	// OBB情報を設定
	m_pOBB->SetPos(PlayerPos);
	m_pOBB->SetVtxMax(PlayerVtxMax);
	m_pOBB->SetVtxMin(PlayerVtxMin);
	m_pOBB->SetRot(PlayerRot);

	// プレイヤー状態をログで表示
	CLogger::GetInstance()->AddDuration(u8"プレイヤーが「突き状態」になりました！", 1);
}

//============================================================================
// 終了処理
//============================================================================
void CPlayerState_Thrusting::Uninit(CPlayer* pPlayer)
{
	// 何もなし
}

//============================================================================
// 更新処理
//============================================================================
void CPlayerState_Thrusting::Update(CPlayer* pPlayer)
{
	// === マイフレーム初期化する変数

	m_bNowCollision = false; // 現在当たったかどうかを初期化
	m_bCollisionObstacle = false; // 障害物に当たったかどうかを初期化

	// === 更新処理開始 ===

	const D3DXVECTOR3& SelfPos = pPlayer->GetPos();  // 自身の位置
	D3DXVECTOR3 VecSelfToGoal = m_GoalPos - SelfPos; // 自身から目的地へのベクトル

	// ゲームへのカメラのポインタを取得する
	CCamera* pCamera = CCamera_Manager::GetInstance()->RefRegistry().BindAtKey("Game");

	// カメラ通常状態へのポインタを取得する
	CCamera_State_Normal* pCameraState_Normal = dynamic_cast<CCamera_State_Normal*>(pCamera->GetState());

	// 指数減衰で目的地へ移動する
	pPlayer->SetPos(SelfPos + VecSelfToGoal * s_fPlayerMoveAdjust);

	// 箱判定情報更新
	UpdateBoxInfo(pPlayer);

	// 当たり判定
	Collision(pPlayer);

	// 目的地への距離が一定以下ならプレイヤーの拳を元の位置に戻す
	if (D3DXVec3Length(&VecSelfToGoal) < s_fPlayerGoalToleranceDistance && pCameraState_Normal)
	{
		pPlayer->SetPos(m_GoalPos);
		pPlayer->SetState(DBG_NEW CPlayerState_Returning(pPlayer));
	}

	// デバッグ表示
	CRenderer::SetDebugString("プレイヤーの状態：突く");
}

//============================================================================
// 目的地に到達したかどうか取得
//============================================================================
const bool& CPlayerState_Thrusting::GetGoalPosArrived() const
{
	return m_bGoalPosArrived;
}

//============================================================================
// 現在当たったかどうか
//============================================================================
const bool& CPlayerState_Thrusting::GetNowCollision() const
{
	return m_bNowCollision;
}

//============================================================================
// 障害物に当たったかどうか
//============================================================================
const bool& CPlayerState_Thrusting::GetCollisionObstacle() const
{
	return m_bCollisionObstacle;
}

//============================================================================
// 当たり判定
//============================================================================
void CPlayerState_Thrusting::Collision(CPlayer* pPlayer)
{
	// 箱の判定情報は常に更新する
	m_pCollisionInfo_Box->SetLanding(false); // 地面にいるかどうか初期化
	m_pCollisionInfo_Box->SetPushX(false);	 // X軸に押し出されたかどうか初期化
	m_pCollisionInfo_Box->SetPushXOld(m_pCollisionInfo_Box->GetPushX()); // 1f前にX軸に押し出されていたかどうか設定

	// === オブジェクトリスト先頭取得と判定情報の初期化 ===

	CObject* pObj = CObject::GetTopObject(CObject::LAYER::DEFAULT);  // デフォルトレイヤーの先頭リストを取得

	// === 判定処理開始 ===

	CollisionHole(pPlayer); // 先に穴との判定を開始し、障害物を回避するかどうか決める

	// プレイヤーと判定するオブジェクトの検索
	while (pObj != nullptr)
	{
		CObject* pNext = pObj->GetNext();// 次のオブジェクトを取得

		switch (pObj->GetType())
		{
		case CObject::TYPE::OBSTACLE:
			if (!m_bAvoidObstacle)
			{
				CollisionObstacle(pPlayer,pObj); // 障害物との判定開始
			}
			break;
		case CObject::TYPE::ZONE:    // ゾーンとの判定開始
			CollisionZone(pPlayer, pObj);
			break;
		case CObject::TYPE::HOLEOBSTACLE:
			if (!m_bAvoidObstacle)
			{
				CollisionHoleObstacle(pPlayer, pObj); // 穴あき障害物との当たり判定
			}
			break;
		default:
			break;
		}

		pObj = pNext; // リストを次に進める
	}
}

//============================================================================
// 障害物との当たり判定
//============================================================================
void CPlayerState_Thrusting::CollisionObstacle(CPlayer* pPlayer, CObject* pOther)
{
	CObstacle* pObstacle = dynamic_cast<CObstacle*>(pOther); // 障害物にダウンキャスト

    // 障害物へのポインタが存在しないので処理を終了
	if (pObstacle == nullptr)
	{
		return;
	}

	// 障害物の判定を箱の判定にダウンキャスト
	//CCollisionInfo_BoundingBox* pOtherBox = dynamic_cast<CCollisionInfo_BoundingBox*>(pObstacle->GetCollisionInfo());
	COBB* pOtherOBB = pObstacle->GetOBB(); // 障害物の判定をOBB判定にダウンキャスト

	// どちらかのOBB判定情報が存在していなかったら処理を終了
	if (m_pOBB == nullptr || pOtherOBB == nullptr)
	{
		return;
	}

	// OBB判定を行い、当たったらダメージ状態にする
	if (collision::HitOBBToOBB(m_pOBB, pOtherOBB))
	{
		// 被弾SE
		CSound::GetInstance()->Play(CSound::LABEL::HIT_0);
		CSound::GetInstance()->Play(CSound::LABEL::GUKI);

		pPlayer->SetDamage(60);
		pOther->SetRelease(); // 当たったオブジェクトは破棄します
		m_bNowCollision = true; // 現在当たったというフラグをオンにする
		m_bCollisionObstacle = true; // 障害物に当たったというフラグをオンにする
	}

}

//============================================================================
// ゾーンとの当たり判定
//============================================================================
void CPlayerState_Thrusting::CollisionZone(CPlayer* pPlayer, CObject* pOther)
{
	CZone* pZone = dynamic_cast<CZone*>(pOther); // 障害物にダウンキャスト
	const D3DXVECTOR3& PlayerPos = pPlayer->GetPos(); // 位置

	// ゾーンがない時点で処理を終了
	if (pZone == nullptr)
	{
		return;
	}

	// ゾーンの判定を箱の判定にダウンキャスト
	CCollisionInfo_BoundingBox* pOtherBox = dynamic_cast<CCollisionInfo_BoundingBox*>(pZone->GetCollisionInfo());

	// どちらかの箱判定情報が存在していなかったら処理を終了
	if (m_pCollisionInfo_Box == nullptr || pOtherBox == nullptr)
	{
		return;
	}

	// 箱Vs箱押し出し判定を行う
	if (collision::HitBoxToBox(m_pCollisionInfo_Box, pOtherBox))
	{
		// チャリン音
		CSound::GetInstance()->Play(CSound::LABEL::CHARIN);

		pOther->SetRelease(); // セーフゾーンを破棄
		pPlayer->SetAirPower(pPlayer->GetAirPower() + s_nHitSafeZoneToAddPower); // 気を増やす
		m_bNowCollision = true; // 現在当たったというフラグをオンにする

		// 球状にパーティクルを飛ばす
		for (int nCnt = 0; nCnt < s_nZoneParticleSummonNum; nCnt++)
		{
			// 単位球状のランダム方向を生成→rand()だけだとRANDMAXまでの乱数が返されるので、その値をRANDMAXで割り正規化→その値に円周を掛ける
			float theta = static_cast<float>(rand()) / RAND_MAX * D3DX_PI * 2.0f; // Yaw（0～2π）
			float z = static_cast<float>(rand()) / RAND_MAX * 2.0f - 1.0f;        // cos(Pitch)と等価（-1～1）
			float r = sqrtf(1.0f - z * z);  // ３次元ベクトルの式→√（X2乗＋Y２乗＋Z２乗）　＝　１
											// XZ平面の半径がrとすると、r2乗 + z２乗 = 1の２乗 = 1
											// なので r = √(1 - z２乗)

			// 方向ベクトル
			D3DXVECTOR3 dir(
				r * cosf(theta),
				z,
				r * sinf(theta)
			);

			// 任意のスケールで速度を調整
			dir *= s_fZoneParticleSpeed;

			// パーティクル生成
			CParticle* pParticle = CParticle::Create(pZone->GetPos(), D3DXVECTOR3(5.0f, 5.0f, 0.0f), D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f), 600, 600);
			CMove_Info* pMove_Info = pParticle->GetMoveInfo(); // 移動情報へのポインタ
			CHorming_Info* pHorming_Info = pParticle->GetHormingInfo(); // ホーミング情報へのポインタ

			// 移動情報設定
			pMove_Info->SetMove(dir);        // 移動量設定
			pMove_Info->SetInertia(0.001f);    // 慣性をつける
			pMove_Info->SetUseInertia(true); // 慣性を使用する

			// ホーミング情報設定
			pHorming_Info->SetYaw(Calculation::Rot::Yaw(PlayerPos, PlayerPos + dir));     // 現在の位置と1f後の位置からYawを求める
			pHorming_Info->SetPitch(Calculation::Rot::Pitch(PlayerPos, PlayerPos + dir)); // 現在の位置と1f後の位置からPitchを求める
			pHorming_Info->SetUseHorming(true);  // ホーミングを使用する
			pHorming_Info->SetAdjust(0.5f);      // 補正値
			pHorming_Info->SetSpeed(1.0f);       // 速度
			pHorming_Info->SetGoalPos(PlayerPos); // 目標位置
			pHorming_Info->SetAddSpeed(0.2f);    // 加算速度設定
			pHorming_Info->SetAddAdjust(0.01f); // 加算補正値設定
			pHorming_Info->SetLockOnType(CHorming_Info::LOCKONTYPE::PLAYER); // プレイヤーを狙う

			// 障害物の拡散
			SpreadObstacle(pPlayer);
		}
		CRenderer::SetDebugString("セーフ！");   // セーフゾーン通過時の効果をデバッグ表示
	}
}

//============================================================================
// 穴との当たり判定
//============================================================================
void CPlayerState_Thrusting::CollisionHole(CPlayer* pPlayer)
{
	CObject* pObj = CObject::GetTopObject(CObject::LAYER::DEFAULT);  // デフォルトレイヤーの先頭リストを取得

    // プレイヤーと判定するオブジェクトの検索
	while (pObj != nullptr)
	{
		CObject* pNext = pObj->GetNext(); // リストの次のポインタ取得

		// タイプが「穴あきオブジェクト」以外ならリストを次に進める
		if (pObj->GetType() != CObject::TYPE::HOLEOBSTACLE)
		{
			pObj = pNext;
			continue;
		}

		CHoleObstacle* pHoleObstacle = dynamic_cast<CHoleObstacle*>(pObj); // 穴あき障害物にダウンキャスト

		// 穴あき障害物へのポインタが存在しないので処理を終了
		if (pHoleObstacle == nullptr)
		{
			return;
		}

		// 穴あき障害物の穴の回転円柱情報へのポインタ
		CRotatingCylinder* pOtherRotCylinder = pHoleObstacle->GetHole()->GetRotatingCylinder();

		// 取得した判定情報が存在しなかったら処理を終了
		if (pOtherRotCylinder == nullptr)
		{
			return;
		}

		// ワールド空間を考慮した回転円柱との当たり判定を行い、
		// 一度穴に入ったらそのフェーズはずっと障害物に当たらないことにする
		if (!m_bAvoidObstacle)
		{
			m_bAvoidObstacle = collision::HitAABBToIsWorldRotatingCylinder_Sweat(m_pCollisionInfo_Box, pOtherRotCylinder,
				pHoleObstacle->GetPos(), pHoleObstacle->GetRot());
		}
		if (m_bAvoidObstacle)
		{
			CRenderer::SetDebugString("穴の中なので当たっていない！");
			return;
		}

		// リストを次に進める
		pObj = pNext;
	}
}

//============================================================================
// 穴あき障害物との当たり判定
//============================================================================
void CPlayerState_Thrusting::CollisionHoleObstacle(CPlayer* pPlayer, CObject* pOther)
{
	CHoleObstacle* pHoleObstacle = dynamic_cast<CHoleObstacle*>(pOther); // 穴あき障害物にダウンキャスト

    // 穴あき障害物へのポインタが存在しないので処理を終了
	if (pHoleObstacle == nullptr)
	{
		return;
	}

	// 穴あき障害物のOBB判定情報へのポインタ
	COBB* pOtherOBB = pHoleObstacle->GetOBB();

	// 取得した判定情報が存在しなかったら処理を終了
	if (pOtherOBB == nullptr)
	{
		return;
	}

	// 今回のフレームで成功した場合、他の穴あきオブジェクトの穴以外にも当たらないので穴あき障害物とは当たらない
	// OBB同士の判定を行う
	if (collision::HitOBBToOBB(m_pOBB, pOtherOBB))
	{
		m_bNowCollision = true; // 現在当たったというフラグをオンにする
		m_bCollisionObstacle = true; // 障害物に当たったというフラグをオンにする
		CRenderer::SetDebugString("当たっている！");
		pPlayer->SetDamage(60);
		pOther->SetRelease(); // 当たったオブジェクトは破棄します
	}
}

//============================================================================
// ボックス判定情報更新
//============================================================================
void CPlayerState_Thrusting::UpdateBoxInfo(CPlayer* pPlayer)
{
	if (m_pCollisionInfo_Box != nullptr)
	{
		const D3DXVECTOR3& SelfPos = pPlayer->GetPos();   // 自身の位置
		const D3DXVECTOR3& Rot = pPlayer->GetRot();       // 向き
		const D3DXVECTOR3& Size = pPlayer->GetSize();     // サイズ
		const D3DXVECTOR3& VtxMax = pPlayer->GetVtxMax(); // 最大頂点
		const D3DXVECTOR3& VtxMin = pPlayer->GetVtxMin(); // 最小頂点

		const D3DXVECTOR3& BoxPosOld = m_pCollisionInfo_Box->GetPosOld();  // 箱判定の1f前の位置
		const bool& bIsLanding = m_pCollisionInfo_Box->GetLanding();       // 箱判定の地面にいるかどうか
		const bool& bIsLandingOld = m_pCollisionInfo_Box->GetLandingOld(); // 1f前に地面にいるかどうか
		const bool& bPushX = m_pCollisionInfo_Box->GetPushX();             // X軸に押し出されていたかどうか
		const bool& bPushXOld = m_pCollisionInfo_Box->GetPushXOld();       // 1f前にX軸に押し出されていたかどうか
		m_pCollisionInfo_Box->SetPosOld(SelfPos);                          // 判定情報の1f前の位置を更新
		m_pCollisionInfo_Box->SetLandingOld(bIsLanding);                   // 1f前に地面にいるかどうか設定

		// 箱判定情報更新（なぜここに書くか？→位置の更新がここなので)
		m_pCollisionInfo_Box->SetUpdateInfo(SelfPos, BoxPosOld, VEC3_INIT, Size, VtxMax, VtxMin, bIsLanding, bIsLandingOld, bPushX, bPushXOld);

		// OBB判定情報更新
		m_pOBB->SetPos(SelfPos);
		m_pOBB->SetVtxMax(VtxMax);
		m_pOBB->SetVtxMin(VtxMin);
		m_pOBB->SetRot(Rot);
	}
}

//============================================================================
// 障害物を吹き飛ばす
//============================================================================
void CPlayerState_Thrusting::SpreadObstacle(CPlayer* pPlayer)
{
	// デフォルトレイヤーの先頭リストを取得
	CObject* pObj = CObject::GetTopObject(CObject::LAYER::DEFAULT);

	// この時点で存在する障害物タイプのオブジェクト全てに拡散処理を生成
	while (pObj != nullptr)
	{
		// 障害物タイプのオブジェクトなら
		if (pObj->GetType() == CObject::TYPE::OBSTACLE)
		{
			CObstacle* pObstacle = dynamic_cast<CObstacle*>(pObj);

			// ダウンキャスト成功で
			if (pObstacle)
			{
				// 進行速度を作成
				float fRate = 0.1f;

				// 吹き飛び方向を作成
				Vec3 Dir = pObstacle->GetPos() - pPlayer->GetPos();

				// 拡散処理を合成
				pObstacle->ComposeSpreader([fRate, Dir]()
					{
						up<CObstacle::CSpreader> up = make_unique<CObstacle::CSpreader>();
						up->SetRate(fRate);
						up->SetDir(Dir);
						return up;
					}
				);
			}
		}

		pObj = pObj->GetNext();
	}
}