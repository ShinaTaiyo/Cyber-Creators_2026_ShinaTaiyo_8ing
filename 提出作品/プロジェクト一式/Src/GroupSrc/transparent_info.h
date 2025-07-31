//============================================================================
// 
// ２０２５年５月１５日：透明情報クラスを作成 [transparent_info.h]
// Author : 椎名太陽
// 
//============================================================================

#ifndef _TRANSPARENT_INFO_H_
#define _TRANSPARENT_INFO_H_ // 二重インクルード防止

//****************************************************
// 透明情報クラス
//****************************************************
class CTransparent_Info
{
public:
	// <special function>
	CTransparent_Info(int nMaxValue,int nValue,bool bUse,int nSpeed);  // コンストラクタ
	~CTransparent_Info(); // デストラクタ

	// <function>
	void Update(D3DXCOLOR & Col); // 更新処理

	// <getter/setter>
	const int& GetValue() const; // 値を取得
	void SetValue(int nValue);   // 値を設定

	// <getter/setter>
	const int& GetMaxValue() const;  // 最大値を取得
	void SetMaxValue(int nMaxValue); // 最大値を設定

	// <getter/setter>
	const int& GetSpeed() const; // 値増加速度取得
	void SetSpeed(int nSpeed);   // 値増加速度設定

	// <getter/setter>
	const bool& GetUse() const; // 使用するかどうか取得
	void SetUse(bool bUse);     // 使用するかどうか設定

private:
	// <Data>
	int 
		m_nValue,    // 透明にする値をカウントする
	    m_nMaxValue, // 透明にする最大値
	    m_nSpeed; // 値増加速度
	bool m_bUse;  // 透明にするかどうか
};
#endif // _Summon_MANAGER_H_