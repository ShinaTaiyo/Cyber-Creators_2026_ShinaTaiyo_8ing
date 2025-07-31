//============================================================================
// 
// ２０２５年４月２８日：コリジョン系クラス [bounding_box.h]
// Author : 椎名太陽
// 
//============================================================================

#ifndef _BOUNDING_BOX_H_
#define _BOUNDING_BOX_H_	// 二重インクルード防止

//****************************************************
// インクルードファイル
//****************************************************
#include "collision.h" //CollisionStrategy基底クラスがあるので

//****************************************************
// ボックス境界当たり判定クラス
//****************************************************
class CCollisionInfo_BoundingBox : public CCollisionInfo
{
public:
	CCollisionInfo_BoundingBox();                       // コンストラクタ
	~CCollisionInfo_BoundingBox() override;             // デストラクタ

	//< Setter>
	void SetUpdateInfo(D3DXVECTOR3 Pos, D3DXVECTOR3 PosOld,D3DXVECTOR3 Move,D3DXVECTOR3 Size,D3DXVECTOR3 VtxMax,D3DXVECTOR3 VtxMin,
		bool bIsLanding,bool bIsLandingOld,bool bPushX,bool bPushXOld);// 情報更新用（判定前に全ての情報を手動で更新）
    //< Setter/Getter>
	void SetSize(const D3DXVECTOR3 Size);                   // サイズ設定
	const D3DXVECTOR3& GetSize() const;                     // サイズ取得

	//<setter/getter>
	void SetVtxMax(D3DXVECTOR3 VtxMax);                     // 最大頂点設定
	const D3DXVECTOR3& GetVtxMax() const;                   // 最大頂点取得

	//<setter/getter>
	void SetMove(D3DXVECTOR3 Move);                         // 移動量設定
	const D3DXVECTOR3& GetMove() const;                     // 移動量取得

	//<setter/getter>
	void SetVtxMin(D3DXVECTOR3 VtxMin);                     // 最小頂点設定
	const D3DXVECTOR3& GetVtxMin() const;                   // 最小頂点取得

    //<setter/getter>
	void SetPushX(bool bPush);                              // X軸に押し出されていたかどうか設定
	const bool& GetPushX() const;                           // X軸に押し出されていたかどうか取得

	//<setter/getter>
	void SetPushXOld(bool bPush);                           // 1f前にX軸に押し出されていたかどうか設定
	const bool& GetPushXOld() const;                        // 1f前にX軸に押し出されていたかどうか取得

private:
	D3DXVECTOR3
		m_Size,   // サイズ
	    m_Move,   // 移動量
	    m_VtxMax, // 最大頂点
	    m_VtxMin; // 最小頂点
	bool 
		m_bPushX,        // X軸に押し出されていたかどうか
	    m_bPushXOld;     // 1f前にX軸に当たっていたかどうか
};
#endif // _COLLISION_H_