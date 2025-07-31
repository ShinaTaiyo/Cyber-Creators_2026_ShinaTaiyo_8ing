//============================================================================
// 
// オブジェクト管理、ヘッダファイル [object.h]
// Author : 福田歩希
// 
//============================================================================

#ifndef _OBJECT_H_
#define _OBJECT_H_	// 二重インクルード防止

//****************************************************
// インクルード
//****************************************************
#include "timescale_info.h" // タイムスケールを使用する

//****************************************************
// オブジェクトクラス
//****************************************************
class CObject
{
public:

	//****************************************************
	// 静的定数を定義
	//****************************************************
	static constexpr int MAX_OBJECT = 2048; // 許容するオブジェクトの最大数

	//****************************************************
	// オブジェクトのレイヤー識別
	//****************************************************
	enum class LAYER : WORD
	{
		BG = 0,		// 背景
		DEFAULT,	// 通常
		FRONT,		// 前面
		STENCIL,	// ステンシル
		UI,			// UI
		MAX
	};
	//****************************************************
    // オブジェクト分類識別
    //****************************************************
	enum class OBJTYPE : WORD
	{
		NONE = 0,   // なし
		TYPE2D,     // 2D(数字から始められないので先頭に"TYPE"とつけている)
		TYPE3D,     // 3D(数字から始められないので先頭に"TYPE"とつけている)
		X,          // X
		MAX,        // 最大
	};

	//****************************************************
	// オブジェクトの種類識別
	//****************************************************
	enum class TYPE : WORD
	{
		NONE = 0,	   // 無し
		PLAYER,		   // プレイヤー
		VISIONBLOCKSPAWNER,// 視野妨害物召喚オブジェクト
		BLOCK,		   // ブロック
		BGMODEL,       // 背景モデル
		OBSTACLE,      // 障害物
		HOLEOBSTACLE,  // 穴あき障害物
		HOLE,          // 穴
		VISIONBLOCKER, // 視野妨害
		ZONE,          // ゾーン
		PARTICLE,      // パーティクル
		TEXTMESH,	   // テキストメッシュ
		TEXTURE	,	   // テクスチャ
		NUMBER,        // ナンバー
		BUTTON,        // ボタン
		STENCILMASK,   // ステンシルマスク
		TARGET,        // ターゲット
		TIMER,         // タイマー
		PARTICLE_2D,   // パーティクル_2D
		ONLYTEXT,	   // テキストのみ
		SPIRIT,        // 「気」の蓄積(シェーダー)
		TONE,          // 色調変化(シェーダー)
		SUMMONMANAGER, // 召喚マネージャー
		NOALLRELEASE,  // 全解放で解放されない
		MAX
	};

	// <special function>
	CObject();									// コンストラクタ
	CObject(LAYER Priority = LAYER::DEFAULT);	// 描画優先度指定コンストラクタ
	virtual ~CObject() = 0;						// デストラクタ

	// <function>
	void SetRelease();	// 解放予約設定
	void Release();		// 個別解放処理

	// <getter>
	const bool& GetDeath() const; // 死亡フラグを取得

	// <virtual function>
	virtual HRESULT	Init() = 0;		// 初期設定
	virtual void	Uninit() = 0;	// 終了処理
	virtual void	Update() = 0;	// 更新処理
	virtual void	Draw() = 0;		// 描画処理

	// <getter/setter>
	const TYPE& GetType() const;	   // タイプを取得
	void SetType(TYPE Type);		   // タイプを設定

	// <getter/setter>
	const OBJTYPE& GetObjType() const; // オブジェクトタイプ取得
	void SetObjType(OBJTYPE ObjType);  // オブジェクト分類タイプ設定

	// <getter/setter>
	const bool& GetRelease() const; // 死亡フラグを取得

	// <getter/setter>
	const bool& GetPause() const; // ポーズフラグ取得
	void SetPause(bool bPause);   // ポーズフラグ設定

    // <getter>
	CTimeScale_Info* GetTimeScaleInfo() const;  // タイムスケール情報取得

	// <getter>
	CObject* GetPrev();	// 前のオブジェクトのポインタを取得
	CObject* GetNext();	// 次のオブジェクトのポインタを取得

	// <static function>
	static void ReleaseAll();		// 全オブジェクト解放処理
	static void AllSetRelease();    // 全てのオブジェクトの死亡フラグを発動する処理
	static void UpdateAll();		// 全オブジェクト更新処理
	static void LateUpdateAll();	// 全オブジェクト後更新処理
	static void DrawAll();			// 全オブジェクト描画処理

	// <static getter>
	static CObject* GetTopObject();					// 先頭オブジェクトのポインタ取得
	static CObject* GetTopObject(WORD wPriority);	// 先頭オブジェクトのポインタ取得
	static CObject* GetTopObject(LAYER Priority);	// 先頭オブジェクトのポインタ取得
	static CObject* FindSpecificObject(TYPE Type);	// 特定タイプのオブジェクト探す
	static int		CountSpecificObject(TYPE Type);	// 特定タイプのオブジェクト数を取得

private:

	// <data>
	WORD	 m_wPriority;	 // 描画優先度
	CObject* m_pPrev;		 // 前のオブジェクトのポインタ
	CObject* m_pNext;		 // 次のオブジェクトのポインタ
	TYPE	 m_Type;		 // タイプ識別 
	OBJTYPE  m_ObjType;      // オブジェクト分類識別
	bool	 m_bDeath;		 // 死亡フラグ
	bool     m_bPause;       // ポーズフラグ（オンなら更新処理を呼ばない）
	bool     m_bIsSummonManagerObj; // 召喚マネージャーが生成したオブジェクトかどうか
	CTimeScale_Info* m_pTimeScaleInfo; // タイムスケール情報

	// <static data>
	static int		m_nNumAll;								// オブジェクト総数
	static CObject* m_pTop[static_cast<WORD>(LAYER::MAX)];	// 先頭オブジェクトのポインタ
	static CObject* m_pCur[static_cast<WORD>(LAYER::MAX)];	// 終端オブジェクトのポインタ
};

#endif // _OBJECT_H_