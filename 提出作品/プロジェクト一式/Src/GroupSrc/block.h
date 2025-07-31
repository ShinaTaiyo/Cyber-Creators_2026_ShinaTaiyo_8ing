//============================================================================
// 
// ２０２５年４月２８日：ブロッククラスを作成 [block.h]
// Author : 椎名太陽
// 
//============================================================================

#ifndef _BLOCK_H_
#define _BLOCK_H_	// 二重インクルード防止

//****************************************************
// インクルードファイル
//****************************************************
#include "object_X.h" // オブジェクトX

//****************************************************
// 前方宣言
//****************************************************
class CCollisionInfo_BoundingBox; // 箱判定情報
class CCollisionInfo; // 判定情報

//****************************************************
// プレイヤークラス
//****************************************************
class CBlock : public CObject_X
{
public:

	// === 特殊関数 ===

	CBlock();				// コンストラクタ
	~CBlock() override;	// デストラクタ

	// === ライフサイクルメンバ関数 ===

	HRESULT Init() override;	// 初期設定
	void	Uninit() override;	// 終了処理
	void	Update() override;	// 更新処理
	void	Draw() override;	// 描画処理
	
	// === メンバ関数 ===

	CCollisionInfo* GetCollisionInfo() const;   // 判定情報取得

	// === 静的メンバ関数 ===

	static CBlock* Create(D3DXVECTOR3 InitPos);	// 生成

private:

	// === メンバ変数 ===

	CCollisionInfo_BoundingBox * m_pCollisionInfo_Box; // 箱判定情報
};

#endif // _PALYER_H_