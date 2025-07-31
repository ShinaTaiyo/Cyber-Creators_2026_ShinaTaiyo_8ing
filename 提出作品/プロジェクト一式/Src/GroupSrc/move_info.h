//============================================================================
// 
// ２０２５年５月１６日：移動情報クラスを作成 [move_info.h]
// Author : 椎名太陽
// 
//============================================================================

#ifndef _MOVE_INFO_H_
#define _MOVE_INFO_H_	// 二重インクルード防止

//****************************************************
// インクルード
//****************************************************
#include "object.h"

//****************************************************
// 召喚マネージャーステートクラス
//****************************************************
class CMove_Info
{
public:
	// <special function>
	CMove_Info();  // コンストラクタ
	~CMove_Info(); // デストラクタ

	// <function>
	void Update(D3DXVECTOR3 & Pos,CObject * pObj); // 更新処理（このクラスの処理を読んで位置情報を更新します）タイムスケールも反映させたいのでオブジェクトのポインタも引数に

	// <getter/setter>
	void SetMove(D3DXVECTOR3 Move);     // 移動量設定
	const D3DXVECTOR3& GetMove() const; // 移動量取得

	// <getter/setter>
	void SetInertia(float fInertia); // 慣性設定
	const float& GetInertia() const; // 慣性取得

	// <getter/setter>
	void SetUseMove(bool bUse);     // 移動するかどうか設定
	const bool& GetUseMove() const; // 移動するかどうか取得

	// <getter/setter>
	void SetUseInertia(bool bUse);     // 慣性を使用するかどうか設定
	const bool& GetUseInertia() const; // 慣性を使用するかどうか取得

	void ControlInfo(); // 情報操作

private:
	// <function>
	void CalcInertia(); // 慣性計算

	// <Data>
	D3DXVECTOR3 m_Move; // 移動量
	float m_fInertia;   // 慣性
	bool 
		m_bUseInertia, // 慣性を使用するかどうか
	    m_bUseMove;    // 移動量を使用するかどうか
};
#endif // _Summon_MANAGER_H_