//============================================================================
// 
// ２０２５年６月５日：召喚グループ管理クラスを作る [SummonGroupSelector.h]
// Author : 椎名太陽
// 
//============================================================================

//****************************************************
// 2重インクルード防止
//****************************************************
#ifndef _SUMMONGROUPSELECTOR_H_
#define _SUMMONGROUPSELECTOR_H_	// 二重インクルード防止

//****************************************************
// インクルード
//****************************************************
#include "summon_manager.h" // 召喚マネージャーの情報を使用

//****************************************************
// 召喚グループ選択クラス
//****************************************************
class CSummonGroupSelector
{
public:
	// === 列挙型 ===

	// イベント
	enum class EVENT
	{
		NONE = 0,
		LEAF,
		SILHOUETTE,
		MAX
	};

	// === 特殊関数 ===

	CSummonGroupSelector();  // コンストラクタ
	~CSummonGroupSelector(); // デストラクタ

	// === ライフサイクルメンバ関数 ===

	void Update(); // 更新処理

	// === メンバ関数 ===

	// フェーズ成功回数
	const int& GetSuccessPhaseNum() const; // 取得
	void SetSuccessPhaseNum(int nNum);     // 設定

	// 手刀使用回数
	const int& GetNumUseHandSword() const; // 取得
	void SetNumUseHandSword(int nNum);     // 設定

	// 難易度スコア設定
	const int& GetDifficultyScore() const; // 取得
	void SetDifficultyScore(int nScore);   // 設定

	// 召喚グループ番号
	const int& GetSummonGroupNum() const;   // 取得
	void SetSummonGroupNum(int nNum); // 設定

	void DecideSummonGroup();    // 召喚グループを決定する

	const CSummon_Manager::DIFFICULTY& GetDifficultyRank() const; // 現在の難易度ランクの取得

	static const int& GetMaxDifficultyScore(); // 難易度スコア最大値を取得
private:
	// === 静的メンバ変数 ===

	// 難易度を上昇させるボーダーとなるスコア
	static constexpr int
		s_nDifficultyBorderScore[static_cast<int>(CSummon_Manager::DIFFICULTY::MAX)] =
	{
		1,20,40,80
	},
		s_nMaxDifficultyScore = 100; // 難易度最大スコア

	// === メンバ変数 ===

	int
		m_nSuccessPhaseNum, // フェーズ成功回数
		m_nNumUseHandSword, // 手刀使用回数
		m_nDifficultyScore, // 難易度を上昇させるスコア
		m_nSummonGroupNum;  // 召喚グループ番号
	CSummon_Manager::DIFFICULTY m_DifficultyRank; // 難易度ランク

	// === メンバ関数 ===

	void UpdateDifficultyRank();  // 難易度ランクを更新する
};
#endif // _Summon_MANAGER_H_