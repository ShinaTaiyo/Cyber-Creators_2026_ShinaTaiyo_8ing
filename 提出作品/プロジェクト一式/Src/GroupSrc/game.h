//============================================================================
// 
// ２０２５年５月３０日：プレイヤーをシーンごとに派生させる [game.h]
// Author : 福田歩希
// 
//============================================================================

#ifndef _GAME_H_
#define _GAME_H_	// 二重インクルード防止

//****************************************************
// インクルードファイル
//****************************************************
#include "scene.h"
#include "summon_manager.h"
#include "timer.h" // タイマー
#include "EffectAccumulation.h" // 「気」の蓄積シェーダー
#include "ToneAdjust.h"  // 色調変化のシェーダー
#include "SmokeEffect.h"  // 霧のシェーダー

//****************************************************
// 前方宣言
//****************************************************
class CGameState;  // ゲーム状態クラス
class CGamePlayer; // ゲームプレイヤー
class CVisionBlockSpawner; // 視野妨害オブジェクトスポナー
class CEffectAccumulation;  // 「気」も蓄積シェーダー

class CSummon_Manager_State_Start;
class CObject_X;
class CMotion_Set;
class CSpace;			// 空間制御処理用
class CSwayingPlayer;	// 揺れ再生処理用

//****************************************************
// ゲームクラス
//****************************************************
class CGame : public CScene
{
	//****************************************************
	// 静的メンバ定数の宣言
	//****************************************************
	static const D3DXVECTOR3 POS_HARISENBON;	// ハリセンボンの座標

public:

	// === ライフサイクルメンバ関数 ===

	void Update() override;		// 更新処理
	void Draw() override;		// 描画処理

	// === メンバ関数 ===

	void To_Next() override; // 次のシーンへ

	// === 静的メンバ関数 ===

	static CGame* Create();	// ゲームを生成

	// ハリセンボンの座標を取得
	inline const D3DXVECTOR3& GetPosHarisenbon() { return POS_HARISENBON; }

private:
	// === 特殊関数 ===
	CGame();			// コンストラクタ
	~CGame() override;	// デストラクタ

	// === 構造体 ===

	// 基本召喚情報
	struct SummonInfo
	{
		std::string Name;                       // 名前
	    D3DXVECTOR3 Pos = { 0.0f,0.0f,0.0f };   // 位置
		D3DXVECTOR3 Move = { 0.0f,0.0f,0.0f };  // 移動量
	    D3DXVECTOR3 Scale = { 0.0f,0.0f,0.0f }; // 拡大率
		int nSummonFrame = 0;                   // 召喚フレーム
	};

	// === ライフサイクルメンバ関数 ===

	HRESULT Init() override;	            // 初期設定
	void	Uninit() override;	            // 終了処理

	// === メンバ関数 ===

	void GameOver();	// ゲームオーバー
	void UpdateInCooltime(const CSummon_Manager_State_Start& ref);	// クールタイム時の更新
	void UpdateMountain(int nowPhase);	// 山更新

	// === メンバ変数 ===

	bool m_bEnd;                            // ゲームシーン終了フラグ
	int m_nCntFrame;                        // フレーム数をカウントする
	bool m_bEditSummon;                     // 召喚するオブジェクトを変種する
	CEffectAccumulation* m_pEffectAccumulation; // 「気」の蓄積シェーダーのポインタ
	CToneAdjust* m_pToneAdjust;             // 色調変化のシェーダーのポインタ
	CSmokeEffect* m_pSmokeEffect;           // 霧のシェーダーのポインタ

	CMotion_Set*					m_pEnemy;		// 敵モーション
	CMotion_Set*					m_pFakeField;	// 疑似床モーション
	CObject_X*						m_pMountain;	// 山モデル
	std::unique_ptr<CSwayingPlayer>	m_upSwaying;	// 揺れ再生処理合成先
};

#endif // _GAME_H_