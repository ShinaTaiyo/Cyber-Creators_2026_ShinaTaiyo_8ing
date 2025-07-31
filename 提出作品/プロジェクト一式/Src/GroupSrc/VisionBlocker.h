//============================================================================
// 
// ２０２５年６月１２日：視野妨害処理を実装 [VisionBlocker.h]
// Author : 椎名太陽
// 
//============================================================================

//****************************************************
// ２重インクルード防止
//****************************************************
#ifndef _VISIONBLOCKER_H_
#define _VISIONBLOCKER_H_

//****************************************************
// インクルードファイル
//****************************************************
#include "object_X.h"     // オブジェクトX

//****************************************************
// 前方宣言
//****************************************************
class CMove_Info; // 移動情報

//****************************************************
// 視野妨害クラス
//****************************************************
class CVisionBlocker : public CObject_X
{
public:
	// === 列挙型 ===

	// 種類
	enum class TYPE
	{
		LEAF00 = 0, // 葉っぱ００
		LEAF01,     // 葉っぱ０１
		LEAF02,     // 葉っぱ０２
		MAX
	};

	// === 特殊関数 ===

	CVisionBlocker(); // コンストラクタ
	~CVisionBlocker() override;	// デストラクタ

	// === ライフサイクルメンバ関数 ===

	HRESULT Init() override;	// 初期設定
	void	Uninit() override;	// 終了処理
	void	Update() override;	// 更新処理
	void	Draw() override;	// 描画処理

	// === メンバ関数 ===

	// 種類
	const TYPE& GetType() const; // 取得
	void SetType(TYPE Type);     // 設定

	// 葉っぱが揺れる角度
	const float& GetLeafSwayAngle() const; // 取得
	void SetLeafSwayAngle(float fAngle);   // 設定

	// 葉っぱを揺らすために加算する角度
	const float& GetLeafSwayAddAngle() const;  // 取得
	void SetLeafSwayAddAngle(float fAddAngle); // 設定

	// 葉っぱを揺らす速度
	const float& GetLeafSwaySpeed() const;   // 取得
	void SetLeafSwaySpeed(float fSwaySpeed); // 設定

	// 加算角度
	const D3DXVECTOR3& GetAddRot() const; // 取得
	void SetAddRot(D3DXVECTOR3 AddRot);   // 設定

	// 吹っ飛ばし移動量
	const D3DXVECTOR3& GetBlowMove() const; // 取得
	void SetBlowMove(D3DXVECTOR3 Move);     // 設定

	// 吹っ飛ばし慣性
	const float& GetBlowInertia() const; // 取得
	void SetBlowInertia(float fInertia); // 設定

	CMove_Info* GetMoveInfo() const; // 移動情報取得

	// 体力
	const int& GetLife() const; // 取得
	void SetLife(int nLife);    // 設定

	// === 静的メンバ関数 ===

	static CVisionBlocker* Create(TYPE Type, D3DXVECTOR3 InitPos); // 生成
private:
	// === 静的メンバ変数 ===

	static const std::string s_aVisionBlockerModelType[static_cast<int>(TYPE::MAX)]; // 視野妨害モデルタイプ

	// === メンバ変数 ===

	CMove_Info* m_pMove_Info; // 移動情報
	float
		m_fLeafSwayAngle,    // 葉っぱが揺れる角度
		m_fLeafSwayAddAngle, // 葉っぱを揺らすために加算する角度
		m_fLeafSwaySpeed;    // 葉っぱを揺らす速度
	D3DXVECTOR3
		m_AddRot,   // 加算角度
		m_BlowMove; // 吹っ飛ばし移動量
	int m_nLife; // 体力
	float m_fBlowInertia; // 吹っ飛ばし慣性
	TYPE m_Type; // 視野妨害タイプ

	// === メンバ関数 ===

	void Blow(); // 吹っ飛ばし処理
};

#endif // _PALYER_H_