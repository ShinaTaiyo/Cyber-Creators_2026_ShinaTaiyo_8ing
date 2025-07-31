//============================================================================
// 
// ２０２５年５月１５日：タイムスケール情報クラスを作成 [timescale_info.h]
// Author : 椎名太陽
// 
//============================================================================

#ifndef _TIMESCALE_INFO_H_
#define _TIMESCALE_INFO_H_	// 二重インクルード防止

//****************************************************
// usingディレクティブ
//****************************************************
using namespace useful;

//****************************************************
// タイムスケール情報クラス
//****************************************************
class CTimeScale_Info
{
public:
	// === 特殊関数 ===

	CTimeScale_Info();  // コンストラクタ
	~CTimeScale_Info(); // デストラクタ

	// === ライフサイクルメンバ関数 ===

	void Update(); // 更新処理

	// === メンバ関数 ===

	// タイムスケール
	void SetTimeScale(float fTimeScale); // 設定
	const float& GetTimeScale() const;   // 取得

	// 目的のタイムスケールに近づける
	void SetGoalTimeScale(float fGoalTimeScale, float fAdjust);

	// タイムスケールを指定フレーム数だけ変えるように設定
	void SetTimeScaleChangeFrame(float fTimeScale, int nChangeFrame);

private:
	// === メンバ変数 ===

	int 
		m_nChangeFrame,           // タイムスケールを変えるフレーム数
        m_nChangeFrameCnt;        // タイムスケールを変えるフレーム数カウント
	float m_fTimeScale;           // タイムスケール値
	float m_fAdjustGoalTimeScale; // 目的のタイムスケールに近づける補正値
	float m_fGoalTimeScale;       // 目的のタイムスケール
	bool m_bUseChangeFrame;       // タイムスケールを変えるフレーム数を使用するかどうか
	bool m_bUseGoalTimeScale;     // 目的のタイムスケールを使用するかどうか

	// === メンバ関数 ===

	void TimeScaleChangeFrame(); // タイムスケールを指定フレーム数だけ変える処理
	void GoalTimeScale(); // 目的のタイムスケールに近づける
	void ResetInfo(); // 情報初期化
};
#endif // _Summon_MANAGER_H_