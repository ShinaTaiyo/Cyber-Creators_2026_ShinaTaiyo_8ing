//============================================================================
// 
// ２０２５年６月１２日：視野妨害オブジェクトを召喚するクラスを作成 [VisionBlockSpawner.h]
// Author : 椎名太陽
// 
//============================================================================

//****************************************************
// 2重インクルード防止
//****************************************************
#ifndef _VISIONBLOCKSPAWNER_H_
#define _VISIONBLOCKSPAWNER_H_	// 二重インクルード防止

//****************************************************
// インクルード
//****************************************************
#include "object.h" // オブジェクト
#include "VisionBlocker.h" // 視野妨害オブジェクト

//****************************************************
// 召クラス
//****************************************************
class CVisionBlockSpawner : public CObject
{
public:
	// === 特殊関数 ===

	CVisionBlockSpawner();  // コンストラクタ
	~CVisionBlockSpawner(); // デストラクタ

	// === ライフサイクルメンバ関数 ===

	HRESULT Init() override;	// 初期設定
	void	Uninit() override;	// 終了処理
	void	Update() override;	// 更新処理
	void	Draw() override;	// 描画処理

	// === メンバ関数 ===

	// 召喚レート
	const int& GetSpawnRate() const; // 取得
	void SetSpawnRate(int nRate);    // 設定

	const int& GetCntFrame() const; // フレーム数カウント取得

	// 召喚の基点となる位置
	const D3DXVECTOR3& GetSpawnPoint() const;   // 取得
	void SetSpawnPoint(D3DXVECTOR3 SpawnPoint); // 設定

	// 召喚範囲最大
	const D3DXVECTOR3& GetMaxSpawnRange() const;     // 取得
	void SetMaxSpawnRage(D3DXVECTOR3 MaxSpawnRange); // 設定

	// 召喚範囲最小
	const D3DXVECTOR3& GetMinSpawnRange() const;    // 取得
	void SetMinSpawnRage(D3DXVECTOR3 MinSpawnRange); // 設定

	// ベースとなる移動量
	const D3DXVECTOR3& GetBaseMove() const; // 取得
	void SetBaseMove(D3DXVECTOR3 BaseMove); // 設定

	// 葉っぱ最大速度
	const float& GetMaxLeafSpeed() const;  // 取得
	void SetMaxLeafSpeed(float fMaxSpeed); // 設定

	// 葉っぱ最低速度
	const float& GetMinLeafSpeed() const;  // 取得
	void SetMinLeafSpeed(float fMinSpeed); // 設定

	// 葉っぱを揺らす角度加算量最大
	const float& GetMaxLeafSwayAddAngle() const;     // 取得
	void SetMaxLeafSwayAddAngle(float fMaxAddAngle); // 設定

	// 葉っぱを揺らす角度加算量最小
	const float& GetMinLeafSwayAddAngle() const;     // 取得
	void SetMinLeafSwayAddAngle(float fMinAddAngle); // 設定

	// 葉っぱ加算角度範囲
	const float& GetLeafAddRotRange() const; // 取得
	void SetLeafAddRotRange(float fRange);   // 設定

	// 葉っぱ体力
	const int& GetLeafLife() const; // 取得
	void SetLeafLife(int nLife);    // 設定

	// 葉っぱ種類
	const CVisionBlocker::TYPE& GetLeafType() const; // 取得
	void SetLeafType(CVisionBlocker::TYPE Type);     // 設定

	// === 静的メンバ関数 ===

	// 生成処理
	static CVisionBlockSpawner* Create(
		D3DXVECTOR3 SpawnPoint, // 召喚基点
		D3DXVECTOR3 BaseMove,   // ベース移動量
		int nSpawnRate, // 召喚レート
		D3DXVECTOR3 MaxSpawnRange, // 召喚範囲最大
		D3DXVECTOR3 MinSpawnRange, // 召喚範囲最小
		float fMaxLeafSpeed, // 葉っぱ速度最大
		float fMinLeafSpeed, // 葉っぱ速度最低
		float fMaxLeafSwayAddAngle, // 葉っぱを揺らす角度加算量最大
		float fMinLeafSwayAddAngle, // 葉っぱを揺らす角度加算量最小
		float fLeafAddRotRange, // 葉っぱの向き加算範囲
		int nLeafLife,
		CVisionBlocker::TYPE LeafType // 葉っぱの種類
	);

private:
	// === メンバ変数 ===

	int
		m_nSpawnRate, // 召喚レート
		m_nCntFrame,  // フレーム数カウント
		m_nLeafLife;  // 体力
	float
		m_fMaxLeafSpeed,          // 葉っぱ最大速度
		m_fMinLeafSpeed,          // 葉っぱ最低速度
		m_fMaxLeafSwayAddAngle,   // 葉っぱを揺らす角度加算量最大
		m_fMinLeafSwayAddAngle,   // 葉っぱを揺らす角度加算量最小
		m_fLeafAddRotRange;       // 葉っぱ加算角度範囲
	D3DXVECTOR3
		m_SpawnPoint,    // 基点
		m_MaxSpawnRange, // 召喚範囲最大
		m_MinSpawnRange, // 召喚範囲最小
		m_BaseMove;      // ベースとなる移動量
	CVisionBlocker::TYPE m_LeafType; // 召喚視野妨害オブジェクトタイプ

	// === メンバ関数 ===

	D3DXVECTOR3 DecideSpawnPos();   // 召喚位置決定
	float DecideLeafSpeed();        // 速度決定
	float DecideLeafSwayAddAngle(); // 葉っぱが揺れる角度加算量決定
	D3DXVECTOR3 DecideLeafAddRot(); // 葉っぱ加算角度決定

};
#endif // _VISIONBLOCKSPAWNER_H_