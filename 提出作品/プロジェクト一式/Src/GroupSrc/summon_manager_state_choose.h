//============================================================================
// 
// ２０２５年７月２１日：召喚マネージャー選択召喚状態クラスを作る [summon_Manager_state_Choose.h]
// Author : 椎名太陽
// 
//============================================================================

#ifndef _SUMMON_MANAGER_STATE_CHOOSE_H_
#define _SUMMON_MANAGER_STATE_CHOOSE_H_	// 二重インクルード防止

//****************************************************
// インクルード
//****************************************************
#include "summon_manager_state.h"

//****************************************************
// usingディレクティブ
//****************************************************
using namespace useful;

//****************************************************
// 前方宣言
//****************************************************

//****************************************************
// 召喚マネージャーステート：開始クラス
//****************************************************
class CSummon_Manager_State_Choose : public CSummon_Manager_State
{
public:

	// === 特殊関数 ===

	CSummon_Manager_State_Choose(bool bGuide);           // コンストラクタ
	~CSummon_Manager_State_Choose() override; // デストラクタ

	// === ライフサイクルメンバ関数 ===

	void Update(CSummon_Manager* pManager) override; // 更新処理

	// === メンバ関数 ===

	// ガイドするかどうか
	const bool& GetGuide() const; // 取得
	void SetGuide(bool bGuide);   // 設定

private:
	// === メンバ変数 ===

	int m_nChooseGroup; // 選択グループ番号
	bool m_bGuide;      // ガイドするかどうか

	// === メンバ関数 ===

	void SetAnticipationFrame(const JSON& SummonPattern) override; // 見切り時間を設定する
};
#endif // _Summon_MANAGER_H_