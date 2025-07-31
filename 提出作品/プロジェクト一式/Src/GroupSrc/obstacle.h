//============================================================================
// 
// ２０２５年６月９日：OBB判定を実装 [obstacle.h]
// Author : 椎名太陽
// 
//============================================================================

#ifndef _OBSTACLE_H_
#define _OBSTACLE_H_	// 二重インクルード防止

//****************************************************
// インクルードファイル
//****************************************************
#include "object_X.h"     // オブジェクトX
#include "Vec3LerpInfo.h" // ３次元ベクトル線形補間情報
#include "SummonCommonData.h" // 召喚オブジェクト共通データ

//****************************************************
// 前方宣言
//****************************************************
class COBB;       // OBB情報
class CMove_Info; // 移動情報
class CCollisionInfo; // 判定情報
class CCollisionInfo_BoundingBox; // 箱判定情報

//****************************************************
// プレイヤークラス
//****************************************************
class CObstacle : public CObject_X
{
public:
	// === 列挙型 ===

	// 障害物タイプ
	enum class OBSTACLETYPE
	{
		TYPE00_BLOCK = 0, // タイプ０：ブロック
		MAX
	};

	// === 特殊関数 ===

	CObstacle(bool bIsSummonManagerObj); // コンストラクタ
	~CObstacle() override;	// デストラクタ

	// === ライフサイクルメンバ関数 ===

	HRESULT Init() override;	// 初期設定
	void	Uninit() override;	// 終了処理
	void	Update() override;	// 更新処理
	void	Draw() override;	// 描画処理

	// === メンバ関数 ===

	COBB* GetOBB() const; // OBB情報を取得
	virtual void InitFromJson(const JSON& Json); // ジェイソン初期化処理

	// タイプ
	const OBSTACLETYPE& GetObstacleType() const; // 取得
	void SetObstacleType(OBSTACLETYPE Type);     // 設定

	// 破壊可能かどうか
	const bool& GetCanbeDestroy() const; // 破壊可能かどうか取得
	void SetCanbeDestroy(bool bDestroy); // 破壊可能かどうか設定

	// 向き加算量
	void SetAddRot(D3DXVECTOR3 AddRot);   // 設定
	const D3DXVECTOR3& GetAddRot() const; // 取得

	// 向きを加算し続けるかどうか
	const bool& GetUseAddRot() const; // 取得
	void SetUseAddRot(bool bUse); // 設定

	// 召喚オブジェクト共通データ
	CSummonCommonData& GetSummonCommonData(); // 取得
	void SetSummonCommonData(CSummonCommonData SummonCommonData); // 設定

	const bool& GetSummonManagerObj() const; // 召喚マネージャーが生成したオブジェクトかどうか取得

	// === 静的メンバ関数 ===

	static CObstacle* Create(const std::string& modelpath, OBSTACLETYPE Type,D3DXVECTOR3 InitPos,bool bIsSummonManagerObj); // 生成

private:

	// === メンバ変数 ===

	OBSTACLETYPE m_Type; // 種類
	bool 
		m_bCanbeDestroy,       // 破壊可能かどうか
		m_bUseAddRot,          // 向きを加算し続けるかどうか
	    m_bIsSummonManagerObj; // 召喚マネージャーが生成したオブジェクトかどうか
	CCollisionInfo_BoundingBox* m_pCollisionInfo_Box; // 判定情報
	COBB* m_pOBB; // OBB判定情報
	D3DXVECTOR3 m_AddRot; // 加算する向き
	CSummonCommonData m_SummonCommonData; // 召喚オブジェクト共通データ

	// === メンバ関数 ===

	void ControlInfo_Pos();  // 位置編集
	void UpdateCollisionInfo(); // 判定情報を更新する
	void UpdateFromSummonCommonData(); // 召喚オブジェクト共通データから更新する
	void Rotating(); // 回転処理
	/* ↓ 拡散用処理勝手に追加失礼します */
public:

	//****************************************************
	// 拡散処理クラスを定義
	//****************************************************
	class CSpreader
	{
	public:

		// デフォルトコンストラクタ (イニシャライザー用)
		CSpreader() :
			m_fRate(0.0f),
			m_Dir(VEC3_INIT)
		{}

		// 拡散
		void Spread(CObstacle& r);

		// 進行速度をセット
		inline void SetRate(float rate) { m_fRate = rate; }

		// 吹き飛び方向をセット
		inline void SetDir(const Vec3& dir) { m_Dir = dir; }

	private:
		
		float m_fRate;	// 進行速度
		Vec3  m_Dir;	// 吹き飛び方向
	};

	// 拡散処理を合成
	template <typename T> inline void ComposeSpreader(T&& fpFactory) { m_upSpreader = std::forward<T>(fpFactory)(); }

private:

	// 拡散処理追加用
	useful::up<CSpreader> m_upSpreader;
};

#endif // _PALYER_H_