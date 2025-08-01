//============================================================================
// 
// ２０２５年５月２０日：カメラ状態クラスを作る [camera_state_Normal.h]
// Author : 椎名太陽
// 
//============================================================================

#ifndef _CAMERA_STATE_NORMAL_H_
#define _CAMERA_STATE_NORMAL_H_	// 二重インクルード防止

//****************************************************
// インクルード
//****************************************************
#include "camera_state.h" // カメラ状態

//****************************************************
// usingディレクティブ
//****************************************************
using namespace useful;

//****************************************************
// カメラステートクラス
//****************************************************
class CCamera_State_Normal : public CCamera_State
{
public:
	// === 特殊関数 ===

	CCamera_State_Normal();  // コンストラクタ
	~CCamera_State_Normal(); // デストラクタ

	// === ライフサイクル関数 ===

	void Update(CCamera* pCamera) override; // 更新処理
private:
};
#endif // _Summon_MANAGER_H_