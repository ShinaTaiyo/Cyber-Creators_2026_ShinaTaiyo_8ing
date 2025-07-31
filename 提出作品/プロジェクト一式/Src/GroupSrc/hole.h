//============================================================================
// 
// ２０２５年６月１０日：回転円柱とAABBの当たり判定を実装する [Hole.h]
// Author : 椎名太陽
// 
//============================================================================

#ifndef _HOLE_H_
#define _HOLE_H_	// 二重インクルード防止

//****************************************************
// インクルードファイル
//****************************************************
#include "object_X.h"

//****************************************************
// 前方宣言
//****************************************************
class CRotatingCylinder; // 回転円柱情報

//****************************************************
// 穴クラス
//****************************************************
class CHole : public CObject_X
{
public:

	// === 特殊関数 ===

	CHole();// コンストラクタ
	~CHole() override;	// デストラクタ

	// === ライフサイクルメンバ関数 ===

	HRESULT Init() override;	// 初期設定
	void	Uninit() override;	// 終了処理
	void	ExtraUpdate();	    // 更新処理（オブジェクトクラスで呼ばない）
	void	Draw() override;	// 描画処理

	// === メンバ関数 ===

	CRotatingCylinder* GetRotatingCylinder() const; // 回転円柱情報取得

	void SetParentPos(D3DXVECTOR3 ParentPos); // 親の位置を設定
	void SetParentRot(D3DXVECTOR3 ParentRot); // 親の向きを設定

	// === 静的メンバ関数 ===

	static CHole* Create(D3DXVECTOR3 InitPos); // 生成

private:
	// === メンバ変数 ===

	CRotatingCylinder* m_pRotating_Cylinder; // 回転円柱情報 
	D3DXVECTOR3
		m_ParentPos,   // 位置
		m_ParentRot;   // 向き

	// === メンバ関数 ===

	void UpdateMtxWorld() override; // ワールドマトリックスを設定する
};

#endif // _PALYER_H_