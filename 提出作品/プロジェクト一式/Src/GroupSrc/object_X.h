//============================================================================
// 
// Xオブジェクト、ヘッダファイル [object_X.h]
// Author : 福田歩希
// 
//============================================================================

#ifndef _OBJECT_X_H_
#define _OBJECT_X_H_	// 二重インクルード防止

//****************************************************
// インクルードファイル
//****************************************************
#include "object.h"
// Xモデル情報用
#include "X_manager.h"

//****************************************************
// 前方宣言
//****************************************************
class CRender_Collision;

//****************************************************
// Xオブジェクトクラス
//****************************************************
class CObject_X : public CObject
{
public:

	// <special function>
	CObject_X(LAYER Priority = LAYER::BG);	// 描画優先度指定コンストラクタ
	~CObject_X() override;					// デストラクタ

	// <function>
	HRESULT	Init() override;	// 初期設定
	void	Uninit() override;	// 終了処理
	void	Update() override;	// 更新処理
	void	Draw() override;	// 描画処理

	// <getter/setter>
	CX_Manager::MODEL* GetModel();				// モデル取得
	void BindModel(CX_Manager::MODEL* pModel);	// モデル割当

	// <getter/setter>
	const D3DXVECTOR3& GetRot() const;	// 向き取得
	void SetRot(D3DXVECTOR3 Rot);		// 向き設定

	// <getter/setter>
	const D3DXVECTOR3& GetPos() const;	// 座標取得
	void SetPos(D3DXVECTOR3 Pos);		// 座標設定

	// <getter/setter>
	const D3DXVECTOR3& GetScale() const;	// 縮尺取得
	void SetScale(D3DXVECTOR3 Scale);		// 縮尺設定

	// <getter/setter>
	const D3DXVECTOR3& GetVtxMax() const; // 最大頂点取得
	void SetVtxMax(D3DXVECTOR3 VtxMax);   // 最大頂点設定


	// <getter/setter>
	const D3DXVECTOR3& GetVtxMin() const; // 最小頂点取得
	void SetVtxMin(D3DXVECTOR3 VtxMin);   // 最小頂点設定

	// <getter/setter>
	const D3DXVECTOR3& GetBaseVtxMax() const;   // 元の最大頂点取得
	void SetBaseVtxMax(D3DXVECTOR3 BaseVtxMax); // 元の最大頂点設定

	// <getter/setter>
	const D3DXVECTOR3& GetBaseVtxMin() const;   // 元の最小頂点取得
	void SetBaseVtxMin(D3DXVECTOR3 BaseVtxMin); // 元の最小頂点設定

	// <getter/setter>
	const D3DXCOLOR& GetCol() const;	// 色を取得
	void SetCol(D3DXCOLOR Col);			// 色を設定

	// <setter>
	void SetSize(D3DXVECTOR3 Size);     // サイズ設定

	// <getter/setter>
	const bool& GetUseCol() const;	// 色反映を取得
	void SetUseCol(bool Col);		// 色反映を設定

	// <virtual getter>
	virtual D3DXVECTOR3 GetSize() const;	// サイズを取得
	virtual float GetRadius() const;		// 半径を取得
	virtual float GetHeight() const;		// 高さを取得

	// <getter/setter>
	const D3DXMATRIX GetMtxWorld() const; // ワールドマトリックスを取得
	void SetMtxWorld(D3DXMATRIX Mtx);     // ワールドマトリックスを設定

	// <static function>
	static CObject_X* Create(const std::string& key);					// 生成
	static CObject_X* Create(LAYER Priority, const std::string& key);	// 生成
	static CObject_X* Create(const JSON& Json);							// 生成

	static std::unique_ptr<CObject_X> upCreate(const JSON& Json);	// 生成

	// Vector系補間ポインタを参照
	inline up<useful::CVectorInterpolator>& RefVectorInterpolator() { return m_upVectorInterpolator; }

private:

	/// <summary> アルファ値初期化用 </summary>
	static constexpr float ALPHA_INIT = 1.0f;

	// <data>
	CX_Manager::MODEL*	m_pModel;	              // モデル情報
	D3DXVECTOR3			m_Rot;		              // 向き
	D3DXVECTOR3			m_Pos;		              // 座標
	D3DXVECTOR3			m_Scale;	              // 縮尺
	D3DXVECTOR3         m_Size;                   // サイズ
	D3DXVECTOR3         m_VtxMax;                 // 最大頂点
	D3DXVECTOR3         m_BaseVtxMax;             // 元の最大頂点
	D3DXVECTOR3         m_VtxMin;                 // 最小頂点
	D3DXVECTOR3         m_BaseVtxMin;             // 元の最小頂点
	D3DXCOLOR			m_Col;		              // 色
	bool				m_bUseCol;	              // 色反映
	D3DXMATRIX			m_MtxWorld;	              // ワールド行列

	// <function>
	void AdjustVtx();        // 頂点調整
	void AdjustRotAngle();	 // 向きを調整する
    virtual void UpdateMtxWorld();		// ワールド行列設定

	// Vector系補間合成先
	useful::up<CVectorInterpolator> m_upVectorInterpolator;
 };

#endif // _OBJECT_X_H_