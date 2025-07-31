//============================================================================
// 
// ２０２５年５月１１日：召喚マネージャー状態クラスを作る [summon_Manager_state_start.h]
// Author : 椎名太陽
// 
//============================================================================

#ifndef _SUMMON_MANAGER_STATE_START_H_
#define _SUMMON_MANAGER_STATE_START_H_	// 二重インクルード防止

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
class CSummonGroupSelector; // 召喚グループ選択クラス

//****************************************************
// 召喚マネージャーステート：開始クラス
//****************************************************
class CSummon_Manager_State_Start : public CSummon_Manager_State
{
public:

	// <special function>
	CSummon_Manager_State_Start();           // デフォルトコンストラクタ
	CSummon_Manager_State_Start(int nMaxPhase);           // コンストラクタ
	~CSummon_Manager_State_Start() override; // デストラクタ

	// <function>
	void Update(CSummon_Manager* pManager) override; // 更新処理

	// フェーズに勝利したかどうか
	const bool& GetSuccessPhase() const; // 取得
	void SetSuccessPhase(bool bSuccess); // 設定

	// フェーズ可否判定フレームかどうか
	const bool& GetPhaseJudgmentFrame() const; // 取得

	// フェーズを開始したかどうか
	const bool& GetStartPhase() const; // 取得

	// 全てのフェーズが終了したかどうか
	const bool& GetAllPhaseComplete() const; // 取得

	// 召喚クールタイムかどうか
	const bool& GetSummonCoolTime() const; // 取得

	// 連続成功回数の取得
	int GetPhaseComboCnt() const { return m_nPhaseComboCnt; }

	// 現在の召喚回数の取得
	int GetNowPhase() const { return m_nNumPhase; }

	// 召喚クールタイムカウントを取得
	int GetSummonCoolTimeCnt() const { return m_nSummonCoolTimeCnt; };

	CSummonGroupSelector* GetSummonGroupSelector() const; // 召喚グループ選択オブジェクトを取得

	// <staticfunction>
	static const int& GetMaxSummonCoolTime(); // 最大召喚クールタイムを取得する
private:
	// === 静的メンバ変数 ===

	static constexpr int
		s_nMaxRandomSummonRate = 120, // ランダム召喚最大レート
		s_nInitialSummonRate = 90,    // 初期ランダム召喚レート
		s_nMaxSummonCoolTime = 120,    // 最大召喚クールタイム
		s_nNormalSlowFrame = 90,      // 通常スロー時間
		s_nNormalMaxPhase = 20,       // 通常最大フェーズ数
		s_nMaxPlayerControlFrame = s_nNormalSlowFrame,    // 通常のプレイヤー操作可能最大フレーム数（スロー時間だけ）
		s_nAddDifficultyScore = 4; // フェーズクリア時に加算する難易度スコア

	// === メンバ変数 ===

	bool
		m_bSuccessPhase, // フェーズに勝利したかどうか
		m_bPhaseJudgmentFrame,   // フェーズ可否判定フレーム中かどうか
		m_bStartPhase,      // フェーズ開始
		m_bAllPhaseComplete, // 全てのフェーズが終了したかどうか
		m_bSummonCoolTime;  // 召喚クールタイム中かどうか
	int
		m_nSummonCoolTimeCnt,   // 召喚にクールタイムを持たせる
		m_nSummonNum,           // 召喚回数
		m_nMaxPhase,            // 最大フェーズ数
		m_nPhaseComboCnt,       // フェーズ連続成功回数カウント
		m_nNumPhase,            // 何フェーズ目か
		m_nSummonCoolTimeFrame; // 召喚クールタイムのフレーム数
	CSummonGroupSelector* m_pSummonGroupSelector; // 召喚グループ番号を選択するオブジェクト

	// === メンバ関数 ===

	bool RandumSummon();  // ランダム召喚
	bool CoolTimeAfterChooseSummon(); // 召喚クールタイム後に選択グループを召喚する
	void VisionBlockerEvent() override; // 視野妨害オブジェクトイベント（start状態では、現在の難易度によって量が変わる）
	bool SetSummonGroup(int nSummonGroup) override; // グループ召喚
};
#endif // _Summon_MANAGER_H_