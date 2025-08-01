//============================================================================
// 
// ２０２５年７月１０日：召喚関係のバグの改善とプレイヤー条件分岐をステートパターンにする [TutorialState.h]
// Author : 椎名太陽
// 
//============================================================================

//****************************************************
// ２重インクルード防止
//****************************************************
#ifndef _TUTORIALSTATE_H_
#define _TUTORIALSTATE_H_	

//****************************************************
// インクルードファイル
//****************************************************

//****************************************************
// 前方宣言
//****************************************************
class CTutorial; // チュートリアル

//****************************************************
// チュートリアル状態クラス
//****************************************************
class CTutorialState
{
public:
	// === 特殊関数 ===

	CTutorialState(CTutorial* pTutorial); // コンストラクタ
	virtual ~CTutorialState() = 0; // デストラクタ

	// === ライフサイクルメンバ関数 ===

	virtual HRESULT Init(CTutorial* pTutorial) = 0;   // 初期化処理
	virtual void Uninit(CTutorial* pTutorial) = 0; // 終了処理
	virtual void Update(CTutorial* pTutorial) = 0; // 更新処理
private:
};
#endif // _COLLISION_H_