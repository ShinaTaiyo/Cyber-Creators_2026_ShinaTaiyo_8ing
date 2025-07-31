//============================================================================
// 
// ２０２５年５月１１日：召喚マネージャーを作る [summon_manager.h]
// Author : 椎名太陽
// 
//============================================================================

#ifndef _SUMMON_MANAGER_H_
#define _SUMMON_MANAGER_H_	// 二重インクルード防止

//****************************************************
// インクルード
//****************************************************
#include "object.h" 

//****************************************************
// 前方宣言
//****************************************************
class CSummon_Manager_State; // 召喚マネージャー状態クラス

//****************************************************
// usingディレクティブ
//****************************************************
using namespace useful;

//****************************************************
// 召喚マネージャークラス
//****************************************************
class CSummon_Manager : public CObject // 継承しない
{
public:
	// === 列挙型 ===

    // 召喚情報
	struct SummonInfo
	{
		std::string Name;              // 名前
		int nSummonFrame = 0;          // 召喚フレーム
		D3DXVECTOR3 Pos = VEC3_INIT;   // 位置
		D3DXVECTOR3 Move = VEC3_INIT;  // 移動処理 
		D3DXVECTOR3 Scale = VEC3_INIT; // 拡大率
	};

	// 難易度
	enum class DIFFICULTY
	{
		EASY = 0, // 簡単
		NORMAL,   // 普通
		HARD,     // 難しい
		EXPERT,   // 激ムズ
		MAX
	};

	// 召喚グループ
	enum class SUMMONGROUP
	{
		PATTERN_1 = 0,
		PATTERN_2,
		PATTERN_3,
		PATTERN_4,
		PATTERN_5,
		PATTERN_6,
		PATTERN_7,
		PATTERN_8,
		PATTERN_11,
		TAIYO_1,
		MAX
	};

	// === 特殊関数 ===

	CSummon_Manager();  // コンストラクタ
	~CSummon_Manager(); // デストラクタ

	// === ライフサイクルメンバ関数 ===

	HRESULT	Init() override; // 初期化処理
	void Uninit() override;  // 終了処理
	void Update() override;  // 更新処理
	void Draw() override;    // 描画処理

	// === メンバ関数 ===

	// ステート
	void SetChengeState(CSummon_Manager_State* pState); // 変更
	CSummon_Manager_State* GetState() const; // 取得

	// === 静的メンバ関数 ===
	static CSummon_Manager* Create();
	static void AddNumObj(); // 召喚マネージャーのオブジェクト数を増やす
	static void SubNumObj(); // 召喚マネージャーのオブジェクト数を減らす
	static int GetNumObj();  // 召喚マネージャー生成オブジェクト数取得
	static void StopSummonObj(); // 召喚オブジェクトの動きを止める
private:	
	// === 静的メンバ変数 ===

	static int s_nNumObj; // 召喚オブジェクト数をカウントする

	// === メンバ変数 ===

	CSummon_Manager_State* m_pSummonManagerState; // 召喚マネージャーステート
};
#endif // _Summon_MANAGER_H_