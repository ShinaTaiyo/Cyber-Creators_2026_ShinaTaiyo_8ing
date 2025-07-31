//============================================================================
// 
// ２０２５年５月２２日：パーティクルを作成 [particle.h]
// Author : 椎名太陽
// 
//============================================================================

#ifndef _PARTICLE_H_
#define _PARTICLE_H_	// 二重インクルード防止

//****************************************************
// インクルードファイル
//****************************************************
#include "object_Billboard.h" // ビルボード

//****************************************************
// 前方宣言
//****************************************************
class CMove_Info;    // 移動情報
class CHorming_Info; // ホーミング情報

//****************************************************
// パーティクルクラス
//****************************************************
class CParticle : public CObject_Billboard
{
public:

	// === 特別関数 ===

	CParticle();	       // コンストラクタ
	~CParticle() override; // デストラクタ

	// === ライフサイクルメンバ関数 ===

	HRESULT Init() override;	// 初期設定
	void	Uninit() override;	// 終了処理
	void	Update() override;	// 更新処理
	void	Draw() override;	// 描画処理

	// === 関数 ===

	CMove_Info* GetMoveInfo() const; // 移動情報を取得する
	CHorming_Info* GetHormingInfo() const; // ホーミング情報を取得する

	// 最大サイズ
	const D3DXVECTOR3& GetMaxSize() const; // 取得
	void SetMaxSize(D3DXVECTOR3 MaxSize);     // 設定

	// 体力
	const int& GetLife() const; // 取得
	void SetLife(int nLife);    // 設定

	// 最大体力
	const int& GetMaxLife() const; // 取得
	void SetMaxLife(int nMaxLife); // 設定

	const int& GetCntFrame() const; // フレーム数を取得する

	// === 静的メンバ関数 ===

	static CParticle* Create(D3DXVECTOR3 InitPos,D3DXVECTOR3 InitSize,D3DXCOLOR Col,int nLife,int nMaxLife); // 生成
private:

	// === メンバ変数 ===

	int m_nLife,                    // 体力
		m_nMaxLife,                 // 最大体力
		m_nCntFrame;                // フレーム数をカウントする
	float m_fHormingYaw,            // ホーミングする際のYaw値
	      m_fHormingPitch;          // ホーミングする際のPitch値
	bool m_bHorming;                // ホーミングするかどうか
	D3DXVECTOR3 m_HormingGoalPos,   // ホーミングする際の目的値
	            m_MaxSize;          // 最大サイズ
	CMove_Info* m_pMove_Info;       // 移動情報
	CHorming_Info* m_pHorming_Info; // ホーミング情報
};

#endif // _OBJECT_BILLBOARD_H_