//============================================================================
// 
// ２０２５年６月１１日：穴が空いた障害物を実装 [HoleObstacle.h]
// Author : 椎名太陽
// 
//============================================================================

//****************************************************
// ２重インクルード防止
//****************************************************
#ifndef _HOLEOBSTACLE_H_
#define _HOLEOBSTACLE_H_

//****************************************************
// インクルードファイル
//****************************************************
#include "obstacle.h" // 障害物

//****************************************************
// 前方宣言
//****************************************************
class CHole; // 穴

//****************************************************
// 穴あき
//****************************************************
class CHoleObstacle : public CObstacle
{
public:
	// === 特殊関数 ===

	CHoleObstacle(bool bIsSummonManagerObj); // コンストラクタ
	~CHoleObstacle() override;	              // デストラクタ

	// === ライフサイクルメンバ関数 ===

	HRESULT Init() override;	// 初期設定
	void	Uninit() override;	// 終了処理
	void	Update() override;	// 更新処理
	void	Draw() override;	// 描画処理

	// === メンバ関数 ===

	void InitFromJson(const JSON& Json) override; // ジェイソン初期化処理
	CHole* GetHole() const; // 穴を取得

	// === 静的メンバ関数 ===

	static CHoleObstacle* Create(const std::string& modelpath, CObstacle::OBSTACLETYPE Type, D3DXVECTOR3 InitPos, bool bIsSummonManagerObj); // 生成

private:
	// === メンバ変数 ===

	CHole* m_pHole; // 穴へのポインタ

};

#endif // _PALYER_H_