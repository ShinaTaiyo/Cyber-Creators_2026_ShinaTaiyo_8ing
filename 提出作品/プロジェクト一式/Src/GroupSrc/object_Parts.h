//============================================================================
// 
// パーツオブジェクト、ヘッダファイル [object_Parts.h]
// Author : 福田歩希
// 
//============================================================================

#ifndef _OBJECT_PARTS_H_
#define _OBJECT_PARTS_H_	// 二重インクルード防止

//****************************************************
// インクルードファイル
//****************************************************
#include "object.h"

// Xモデル情報用
#include "X_manager.h"

//****************************************************
// パーツオブジェクトクラス
//****************************************************
class CObject_Parts : public CObject
{
public:

	// <special function>
	CObject_Parts(LAYER Priority);	// 描画優先度指定コンストラクタ
	~CObject_Parts() override;		// デストラクタ

	// <function>
	HRESULT	Init() override;	// 初期設定
	void	Uninit() override;	// 終了処理
	void	Update() override;	// 更新処理
	void	Draw() override;	// 描画処理

	// <setter>
	void SetSyncRot(D3DXVECTOR3 Rot);	// シンクロ向きの設定
	void SetSyncPos(D3DXVECTOR3 Pos);	// シンクロ座標の設定
	
	// <getter/setter>
	const CObject_Parts *const GetParent() const;	// 親パーツのポインタ取得
	void SetParent(CObject_Parts* pParent);			// 親パーツのポインタ設定

	// <getter/setter>
	CX_Manager::MODEL* GetModel();				// モデル取得
	void BindModel(CX_Manager::MODEL* pModel);	// モデル割当

	// <getter/setter>
	const D3DXVECTOR3& GetScaleOffset() const;	// 縮尺オフセット取得
	void SetScaleOffset(D3DXVECTOR3 Scale);		// 縮尺オフセット設定

	// <getter/setter>
	const D3DXVECTOR3& GetScale() const;	// 縮尺取得
	void SetScale(D3DXVECTOR3 Scale);		// 縮尺設定

	// <getter/setter>
	const D3DXVECTOR3& GetRotOffset() const;	// 向きオフセット取得
	void SetRotOffset(D3DXVECTOR3 Rot);			// 向きオフセット設定

	// <getter/setter>
	const D3DXVECTOR3& GetRot() const;	// 向き取得
	void SetRot(D3DXVECTOR3 Rot);		// 向き設定

	// <getter/setter>
	const D3DXVECTOR3& GetPosOffset() const;	// 座標オフセット取得
	void SetPosOffset(D3DXVECTOR3 Pos);			// 座標オフセット設定

	// <getter/setter>
	const D3DXVECTOR3& GetPos() const;	// 座標取得
	void SetPos(D3DXVECTOR3 Pos);		// 座標設定

	// <getter/setter>
	const D3DXCOLOR& GetCol() const;	// 色を取得
	void SetCol(D3DXCOLOR Col);			// 色を設定

	// <getter/setter>
	const bool& GetUseCol() const;	// 色反映を取得
	void SetUseCol(bool Col);		// 色反映を設定

	// <getter>
	const D3DXMATRIX& GetMtxWorld() const;	// ワールド行列を取得
	void SetMtxWorld(D3DXMATRIX Mtx);		// ワールド行列を設定

	// <virtual getter>
	virtual D3DXVECTOR3 GetSize() const;	// サイズを取得
	virtual float GetRadius() const;		// 半径を取得
	virtual float GetHeight() const;		// 高さを取得

	// <static function>
	static CObject_Parts* Create(CObject::LAYER Layer, const std::string& key, CObject_Parts* pParent);	// 生成

private:

	// <function>
	void AdjustRotAngle();	// 向きを調整する
	void SetMtxWorld();		// ワールド行列設定

	// <data>
	D3DXVECTOR3			m_RotSync;		// シンクロ向き
	D3DXVECTOR3			m_PosSync;		// シンクロ座標
	CObject_Parts*		m_pParent;		// 親パーツのポインタ
	CX_Manager::MODEL*	m_pModel;		// モデル情報
	D3DXVECTOR3			m_ScaleOffset;	// 縮尺オフセット
	D3DXVECTOR3			m_Scale;		// 縮尺
	D3DXVECTOR3			m_RotOffset;	// 向きオフセット
	D3DXVECTOR3			m_Rot;			// 向き
	D3DXVECTOR3			m_PosOffset;	// 座標オフセット
	D3DXVECTOR3			m_Pos;			// 座標
	D3DXCOLOR			m_Col;			// 色
	bool				m_bUseCol;		// 色反映
	D3DXMATRIX			m_MtxWorld;		// ワールド行列
};

#endif // _OBJECT_PARTS_H_