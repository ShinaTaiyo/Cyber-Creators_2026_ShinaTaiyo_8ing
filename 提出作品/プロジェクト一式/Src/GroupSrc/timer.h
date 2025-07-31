//============================================================================
// 
// ２０２５年５月１２日：タイマーによるシーン遷移実装 [timer.h]
// Author : 椎名太陽
// 
//============================================================================

#ifndef _TIMER_H_
#define _TIMER_H_	// 二重インクルード防止

//****************************************************
// インクルードファイル
//****************************************************
#include "object.h"
#include "number.h"
//****************************************************
// タイマークラス
//****************************************************
class CTimer : public CObject
{
public:
	// <special function>
	CTimer();		    // コンストラクタ
	~CTimer() override;	// デストラクタ

	// <function>
	HRESULT Init() override;    // 初期設定
	void	Uninit() override;	// 終了処理
	void	Update() override;	// 更新処理
	void	Draw() override;   	// 描画処理

	// <getter/setter>
	const D3DXVECTOR3& GetPos() const; // 位置取得
	void SetPos(D3DXVECTOR3 Pos); // 位置設定

	// <getter/setter>
	const D3DXVECTOR3& GetNumberSize() const; // 数字のサイズ取得
	void SetNumberSize(D3DXVECTOR3 Size);     // 数字のサイズ設定
	
	// <getter/setter>
	const int& GetNowTime() const; // 現在の時間取得
	void SetNowTime(int nTime);    // 現在の時間設定

	// <getter>
	const bool& GetEnd() const; // 終了したかどうか取得

	// <static function>
	static CTimer* Create(D3DXVECTOR3 InitPos,D3DXVECTOR3 InitSize);	// 生成
private:
	//****************************************************
    // 静的定数を定義
    //****************************************************
	static constexpr int s_nMaxTime = 60; // 最大時間
	// <Function>
	void NumberDisp();               // 数字表示更新

	// <Data>
	D3DXVECTOR3 m_Pos;                // 位置
	D3DXVECTOR3 m_NumberSize;         // サイズ
	int m_nNowTime;                   // 現在の時間
	int m_nNumDigit;                  // 桁数
	bool m_bEnd;                      // 終了フラグ
	int m_nCntFrame;                  // フレーム数カウント
	std::vector<CNumber*> m_vpNumber; // 数字動的配列 
};

#endif // _PALYER_H_