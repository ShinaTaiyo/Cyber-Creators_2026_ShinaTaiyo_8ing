//============================================================================
// 
// モーションセット、ヘッダファイル [motion_set.h]
// Author : 福田歩希
// 
//============================================================================

#ifndef _MOTION_SET_H_
#define _MOTION_SET_H_	// 二重インクルード防止

//****************************************************
// インクルードファイル
//****************************************************
#include "motion_set.h"
#include "object.h"

//****************************************************
// 前方宣言
//****************************************************
class CObject_Parts;

//****************************************************
// モーションセットクラス
//****************************************************
class CMotion_Set : public CObject
{
public:

	//****************************************************
	// 動作目標値の構造体を定義
	//****************************************************
	struct KeyDest
	{
		D3DXVECTOR3 ScaleTarget;	// 目標縮尺
		D3DXVECTOR3 RotTarget;		// 目標向き
		D3DXVECTOR3 PosTarget;		// 目標座標
	};

	//****************************************************
	// キー情報の構造体を定義
	//****************************************************
	struct Key
	{
		int			nMaxFrame;	// キーの総フレーム数
		KeyDest*	apDest;		// パーツごとの動作目標値
	};

	//****************************************************
	// モーション情報の構造体を定義
	//****************************************************
	struct Motion
	{
		bool bLoop;				// ループフラグ
		WORD wMaxKey;			// モーションの総キー数
		std::vector<Key> vpKey;	// キー情報
	};

	// <special function>
	CMotion_Set(LAYER Layer);	// コンストラクタ
	~CMotion_Set() override;	// デストラクタ

	// <function>
	HRESULT	Init() override;	// 初期設定
	void	Uninit() override;	// 終了処理
	void	Update() override;	// 更新処理
	void	Draw() override;	// 描画処理

	// <setter>
	void SetSyncRot(D3DXVECTOR3 Rot);	// シンクロ向きを設定
	void SetSyncPos(D3DXVECTOR3 Pos);	// シンクロ座標を設定

	// <getter/setter>
	D3DXVECTOR3 GetRot() const;		// 向きを取得
	void SetRot(D3DXVECTOR3 Rot);	// 向きを設定
	
	// <getter/setter>
	D3DXVECTOR3 GetPos() const;		// 座標を取得
	void SetPos(D3DXVECTOR3 Pos);	// 座標を設定

	// <getter/setter>
	WORD GetNowMotion() const;		// 再生中のモーション番号を取得
	void SetNowMotion(WORD wIdx);	// 再生中のモーション番号を設定

	// <getter>
	bool GetStopState() const;			// ストップフラグを取得
	CObject_Parts* GetParentParts();	// 親パーツを取得

	// <static function>
	static CMotion_Set* Create(LAYER Layer, TYPE Type, JSON Json);	// 生成

protected:

	// <function>
	void SetMotion(LAYER Layer, JSON Json);	// モーション情報をセット

private:

	// <function>
	void Animation();		// 動作
	void CountFrame();		// フレームをカウント
	void CorrectTarget();	// 目標値への補正

	// <getter>
	CMotion_Set::Motion* const GetNowMotionPtr();	// 現在のモーションのポインタを取得
	CMotion_Set::Key* const GetNowKey();			// 現在のキーのポインタを取得

	// <data>
	D3DXVECTOR3					m_Rot;			// 向き
	D3DXVECTOR3					m_Pos;			// 座標
	bool						m_bStop;		// ストップフラグ
	WORD						m_wNowMotion;	// 再生中のモーション
	WORD						m_wNowKey;		// 再生中のキー
	WORD						m_wNowFrame;	// 再生中のフレーム
	std::vector<CObject_Parts*>	m_vpModelParts;	// モデルパーツ
	CObject_Parts*				m_pMostParent;	// 親パーツ
	WORD						m_wMaxParts;	// モデルの総パーツ数
	WORD						m_wMaxMotion;	// 総モーション数
	std::vector<Motion>			m_vpMotion;		// モーション情報
};

#endif	// _MOTION_SET_