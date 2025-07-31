//============================================================================
// 
// ２０２５年５月９日：ゾーンクラスを作成 [zone.h]
// Author : 椎名太陽
// 
//============================================================================

#ifndef _ZONE_H_
#define _ZONE_H_	// 二重インクルード防止

//****************************************************
// インクルードファイル
//****************************************************
#include "object_3D.h"      // 3Dポリゴンを出したい
#include "collision.h"      // 当たり判定情報
#include "bounding_box.h"   // 正方形当たり判定
#include "move_info.h"      // 移動情報
#include "Vec3LerpInfo.h" // ３次元ベクトル線形補間情報
#include "SummonCommonData.h" // 召喚オブジェクト共通データ

//****************************************************
// 前方宣言
//****************************************************

//****************************************************
// ゾーンクラス
//****************************************************
class CZone : public CObject_3D
{
public:
	enum class TYPE
	{
		SAFEZONE = 0,
		MAX
	};

	// <special function>
	CZone(bool bIsSummonManagerObj); // コンストラクタ
	~CZone() override;	// デストラクタ

	// <function>
	HRESULT Init() override;	// 初期設定
	void	Uninit() override;	// 終了処理
	void	Update() override;	// 更新処理
	void	Draw() override;	// 描画処理
	void InitFromJson(const JSON& Json); // ジェイソン初期化処理

	// <static function>
	static CZone* Create(TYPE Type,D3DXVECTOR3 InitPos,D3DXVECTOR3 Size,bool bIsSummmonManagerObj);	// 生成

	//< Getter>
	CCollisionInfo* GetCollisionInfo() const; // 判定情報取得
	
    // <getter/setter>
	const bool& GetSummonManagerObj() const; // 召喚マネージャーが生成したオブジェクトかどうか取得

	// 召喚オブジェクト共通データ
	CSummonCommonData& GetSummonCommonData(); // 取得
	void SetSummonCommonData(CSummonCommonData SummonCommonData); // 設定
private:

	//****************************************************
	// 静的定数を定義
	//****************************************************
	static const std::string s_aSafeZoneTextureName[static_cast<int>(TYPE::MAX)]; // テクスチャ名
	static constexpr float s_fAdjustScale = 0.5f; // 拡大率補正値（エディタト合わせるためです）

	// <Data>
	bool m_bIsSummonManagerObj; // 召喚マネージャーが生成したオブジェクトかどうか
	CCollisionInfo_BoundingBox * m_pCollisionInfo_Box; // 判定情報
	CSummonCommonData m_SummonCommonData; // 召喚オブジェクト共通データ

	// === メンバ関数 ===

	void UpdateFromSummonCommonData(); // 召喚オブジェクト共通データから更新する
};

#endif // _PALYER_H_