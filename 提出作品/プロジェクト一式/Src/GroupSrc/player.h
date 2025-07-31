//============================================================================
// 
// (旧)プレイヤー、ヘッダファイル [player.h]
// Author : 福田歩希
// 
//============================================================================

#ifndef _PLAYER_H_
#define _PLAYER_H_	// 二重インクルード防止

//****************************************************
// インクルードファイル
//****************************************************
#include "object_X.h"  // オブジェクトX
#include "camera_manager.h"
#include "space.h"

//****************************************************
// 前方宣言
//****************************************************
class CTarget; // ターゲット
class CMove_Info; // 移動情報
class CPlayerState; // プレイヤー状態

//****************************************************
// プレイヤークラス
//****************************************************
class CPlayer : public CObject_X
{
public:
	// === 列挙型 ===
	enum class TYPE
	{
		TUTORIAL = 0, // チュートリアル
		GAME, // ゲーム
		MAX
	};

	// === 特殊関数 ===

	CPlayer();				// コンストラクタ
	~CPlayer() override;	// デストラクタ

	// === ライフサイクルメンバ関数 ===

	HRESULT Init() override;	// 初期設定
	void	Uninit() override;	// 終了処理
	void	Update() override;	// 更新処理
	void	Draw() override;	// 描画処理

	// === メンバ関数 ===

	// 気
	const int& GetAirPower() const; // 取得
	void SetAirPower(int nPower); // 設定

	const D3DXVECTOR3& GetMove() const; // 移動量取得

	void SetDamage(int nFrame); // ダメージ演出
	CTarget* GetTarget() const; // ターゲット取得

	// プレイヤー状態
	CPlayerState* GetState() const;     // 取得
	void SetState(CPlayerState* State); // 設定

	// タイプ
	const TYPE& GetType() const; // 取得
	void SetType(TYPE Type);     // 設定

	// === 静的メンバ関数 ===

	static CPlayer* Create(D3DXVECTOR3 InitPos);	// 生成
	static const int& GetMaxAirPower(); // 気の最大値を取得

	// 目標向きを設定
	inline void SetRotTarget(const Vec3& rotTarget) { m_RotTarget = rotTarget; }

	void ExecuteUpSpace(); // 手刀演出発動
private:
	// === メンバ関数 ===

	void Rotation();	// 回転
	void Damage(); // ダメージ処理
	void BlowVisionBlocker(); // 視野妨害オブジェクトを吹っ飛ばす
	void AirPowerLimit(); // 気の値制限 
	void CollisionParticle(); // パーティクルとの当たり判定

	// === シェーダー用メンバ関数 ===

	bool InitToonShader(LPDIRECT3DDEVICE9 device, const char* fxFile);
	void Draw_UsingEffect(LPD3DXEFFECT pEffect);

	// === 静的メンバ変数 ===

	static constexpr float
		MAX_VELOCITY = 0.5f,// 加速度上限
		JUMP_FORCE = 6.0f,  // ジャンプ力
		s_fVisionBlockerBlowInertia = 0.01f, // 視野妨害オブジェクトを吹っ飛ばす補正値
		s_fVisionBlockerBlowLength = 100.0f, // 視野妨害オブジェクトを吹っ飛ばす距離
		s_fVisionBlockerBlowSpeed = 5.0f; // 視野妨害オブジェクトを吹っ飛ばす速度
	static constexpr int
		s_nMaxAirPower = 100,          // 気の最大パワー
		s_nINITAIRPOWER = 0;           // 最初の気のポイント

	// === プレイヤー用メンバ変数 ===

	int
		m_nAirPower,          // 気の力
		m_nDamageFrame;       // ダメージ状態のフレーム数
	D3DXVECTOR3
		m_Move,       // 移動量
		m_Velocity,   // 加速度
		m_RotTarget;  // 目標向き
	TYPE m_Type;      // タイプ

	std::unique_ptr<CSpace>			m_upSpace;		// 空間制御処理合成先

	CTarget* m_pTarget; // ターゲットへのポインタ

	CMove_Info* m_pMoveInfo; // 移動情報
	CPlayerState* m_pState; // プレイヤー状態

	// === シェーダー用メンバ変数 ===

	LPD3DXEFFECT m_pToonEffect;
	float m_RimPowor;  // 鋭さ
	float m_RimIntensity;  // 明るさ
	float m_RimThickness;  // 太さ
	LPDIRECT3DTEXTURE9 m_pToonMap;
	std::string m_fxFilePath;             // 読み込む.fxファイルのパス

	LPD3DXMESH m_pMesh;			//メッシュへのポインタ

};

#endif // _PALYER_H_