//============================================================================
// 
// ２０２５年６月９日：OBB判定を実装 [GamePlayer.h]
// Author : 椎名太陽
// 
//============================================================================

//****************************************************
// ２重インクルード防止
//****************************************************
#ifndef _GAMEPLAYER_H_
#define _GAMEPLAYER_H_

//****************************************************
// インクルードファイル
//****************************************************
#include "player.h" // 基底クラスがある

//****************************************************
// 前方宣言
//****************************************************
class CTarget;        // ターゲット情報
class CMove_Info;     // 移動情報
class CCollisionInfo; // コリジョン情報
class CCollisionInfo_BoundingBox;      // 箱判定情報
class CCollisionInfo_BoundingCylinder; // 円柱判定情報
class COBB; // OBB情報
class CPlayerState; // プレイヤー状態

//****************************************************
// ゲームプレイヤークラス
//****************************************************
class CGamePlayer : public CPlayer
{
public:

	// === 特殊関数 ===

	CGamePlayer();				// コンストラクタ
	~CGamePlayer() override;	// デストラクタ

	// === ライフサイクルメンバ関数 ===

	HRESULT Init() override;	// 初期設定
	void	Uninit() override;	// 終了処理
	void	Update() override;	// 更新処理
	void	Draw() override;	// 描画処理

	//// === 静的メンバ関数 ===

	static CGamePlayer* Create(D3DXVECTOR3 InitPos); // 生成
private:
};

#endif // _PALYER_H_