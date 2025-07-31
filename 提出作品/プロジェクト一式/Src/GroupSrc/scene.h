//============================================================================
// 
// シーン管理、ヘッダファイル [scene.h]
// Author : 福田歩希
// 
//============================================================================

#ifndef _SCENE_H_
#define _SCENE_H_	// 二重インクルード防止

//****************************************************
// シーンクラス
//****************************************************
class CScene
{
public:

	// <virtual function>
	virtual void Update();		// 更新処理
	virtual void Draw();		// 描画処理
	virtual	void To_Next() = 0;	// 次のシーンへ

	// <function>
	void Release();	// 解放

protected:

	// <special function>
	CScene();			// コンストラクタ
	virtual ~CScene();	// デストラクタ

	// <virtual function>
	virtual HRESULT Init();		// 初期設定
	virtual void	Uninit();	// 終了処理
};

//****************************************************
// シーン管理クラス
//****************************************************
class CScene_Manager
{
public:

	// <special function>
	CScene_Manager(const CScene_Manager&) = delete;				// コピーコンストラクタ
	CScene_Manager& operator=(const CScene_Manager&) = delete;	// コピー代入演算子
	CScene_Manager(CScene_Manager&&) = delete;					// ムーブコンストラクタ
	CScene_Manager& operator=(CScene_Manager&&) = delete;		// ムーブ代入演算子

	// <static function>
	static HRESULT	Create();	// 生成
	static void		Release();	// 解放

	// <getter>
	CScene* GetScene();	// シーンを取得

	// <static getter>
	static CScene_Manager* GetInstance();	// シーンマネージャーを取得

	// <static setter>
	static void ChangeScene(CScene* pScene);	// シーン変更
		
private:

	// <special function>
	CScene_Manager();	// コンストラクタ
	~CScene_Manager();	// デストラクタ

	// <function>
	HRESULT	Init();		// 初期設定
	void	Uninit();	// 終了処理

	// <data>
	CScene* m_pScene;	// シーン

	// <static data>
	static CScene_Manager* m_pSceneManager;	// シーンマネージャーの本体
};

#endif // _SCENE_H_