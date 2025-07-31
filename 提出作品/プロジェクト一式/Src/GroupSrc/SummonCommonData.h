//============================================================================
// 
// ２０２５年６月２０日：召喚グループ共通データクラスを作る[SummonCommonData.h]
// Author : 椎名太陽
// 
//============================================================================

//****************************************************
// 二重インクルード防止
//****************************************************
#ifndef _SUMMONCOMMONDATA_H_
#define _SUMMONCOMMONDATA_H_

//****************************************************
// インクルードファイル
//****************************************************
#include "Vec3LerpInfo.h" // ３次元ベクトル線形補間情報
#include "move_info.h" // 移動情報
#include "OBB.h" // OBB情報
#include "timescale_info.h" // タイムスケール情報
#include "Vec3ExponertialInfo.h" // 指数減衰情報

//****************************************************
// 召喚オブジェクトが共通で持っているデータクラス
//****************************************************
class CSummonCommonData
{
public:
	// === 特殊関数 ===
	CSummonCommonData(); // デフォルトコンストラクタ
	CSummonCommonData(
		CObject * pHostObj // このクラスを持っている宿主へのポインタ
	); // コンストラクタ
	~CSummonCommonData(); // デストラクタ

	// === ライフサイクルメンバ関数 ===
	void Update(); // 更新処理

	// === メンバ関数 ===

	// 位置
	const D3DXVECTOR3& GetPos() const; // 取得
	void SetPos(D3DXVECTOR3 Pos); // 設定

	// 向き
	const D3DXVECTOR3& GetRot() const; // 取得
	void SetRot(D3DXVECTOR3 Rot); // 設定

	// 拡大率
	const D3DXVECTOR3& GetScale() const; // 取得
	void SetScale(D3DXVECTOR3 Scale); // 拡大率

	// 移動情報
	CMove_Info& GetMoveInfo(); // 取得

	// 位置線形補間情報
	CVec3LerpInfo& GetLerpPosInfo(); // 取得

	// 向き指数減衰情報
	CVec3ExponertialInfo& GetExponertialRotInfo(); // 取得

	// ジェイソン初期化
	void InitFromJson(JSON& Json);

	// 名前
	const std::string& GetName() const; // 取得
	void SetName(std::string Name); // 設定

private:
	// === 静的メンバ変数 ===
	static constexpr float
		s_fNormalMoveZ = -15.0f, // 通常のZ移動量
		s_fRandumRotRate = 20.0f; // 最初の向きの乱数値

	// === メンバ変数 ===
	D3DXVECTOR3
		m_Pos,   // 位置
		m_Rot,   // 向き
		m_Scale; // 拡大率
	std::string m_Name; // 名前
	CMove_Info m_MoveInfo; // 移動情報
	CVec3LerpInfo m_LerpPosInfo; // 線形補間位置情報
	CVec3LerpInfo m_LerpScaleInfo; // 線形補間拡大情報
	CVec3ExponertialInfo m_ExponertialRotInfo; // 指数減衰回転情報
	CTimeScale_Info m_TimeScaleInfo; // タイムスケール情報
	CObject* m_pHostObj; // このクラスを持っている宿主へのポインタ
	bool m_bUse; // 使用するかどうか

	// === メンバ関数 ===
	void Moving(); // 移動処理
	void Rotating(); // 回転処理
	void Scaling(); // 拡大処理
};
#endif // _COLLISION_H_