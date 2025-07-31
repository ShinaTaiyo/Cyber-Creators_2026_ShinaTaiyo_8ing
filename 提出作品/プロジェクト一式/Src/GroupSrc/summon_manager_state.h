//============================================================================
// 
// ２０２５年５月１１日：召喚マネージャー状態クラスを作る [summon_manager_state.h]
// Author : 椎名太陽
// 
//============================================================================

#ifndef _SUMMON_MANAGER_STATE_H_
#define _SUMMON_MANAGER_STATE_H_	// 二重インクルード防止

//****************************************************
// インクルード
//****************************************************
#include "object.h"
#include "pch.h"
#include "summon_manager.h"

//****************************************************
// 前方宣言
//****************************************************
class CSummonGroupSelector; // 召喚グループ選択

//****************************************************
// usingディレクティブ
//****************************************************
using namespace useful;

//****************************************************
// 召喚マネージャーステートクラス
//****************************************************
class CSummon_Manager_State
{
public:
	// <special function>
	CSummon_Manager_State();      // コンストラクタ
	virtual ~CSummon_Manager_State() = 0;     // デストラクタ

	// <function>
	virtual void Update(CSummon_Manager * pManager) = 0; // 更新処理

	// <getter>
	const JSON& GetJsonSummonGroup() const; // 召喚情報ジェイソン取得

	// <getter>
	const JSON& GetJsonSummonPatternList() const; // 召喚パターンリスト取得

	// <getter/setter>
	const int& GetChooseGroup() const; // 選んでいるグループ番号取得
	void SetChooseGroup(int nChoose);  // 選んでいるグループ番号設定

	virtual bool SetSummonGroup(int nSummonGroup); // グループ召喚処理

protected:
	void ChengeState(CSummon_Manager_State* pState,CSummon_Manager * pManager); // ステート変更処理
	void ChangeJsonSummonGroup(int nChooseGroup); // ジェイソンの召喚グループを変更する
private:
	// === 静的メンバ変数 ===

	static constexpr float
		s_fNormal_ReleaseRangeZ = -400.0f; // 超えたら破棄するZ範囲

	// === 変数 ===

	int
		m_nGroup, // 召喚グループ番号
		m_nDifficultyScore; // 難易度を上昇させるスコア
	JSON 
		m_JsonSummonGroup, // 召喚グループ情報
	    m_JsonSummonPatternList; // 召喚パターンをまとめたモデルパスのジェイソン
	bool m_bJustSummon; // ちょうど召喚したかどうか
	CSummon_Manager::DIFFICULTY m_DifficultyRank; // 難易度ランク

	// === メンバ関数 ===

	void SetEvent(const JSON& SummonPattern); // イベントを設定する
	virtual void VisionBlockerEvent(); // 視野妨害イベント
	void ReleaseAllVisionBlockSpawner(); // 全ての視野妨害オブジェクトスポナーを消す
	void SmorkEvent(); // 霧イベント
	void ResetEvent(); // イベント設定リセット
	void DestroyOutOfRange(); // 一定範囲外に出た召喚オブジェクトを破棄する
	virtual void SetAnticipationFrame(const JSON& SummonPattern); // 見切り時間を設定する
};
#endif // _Summon_MANAGER_H_