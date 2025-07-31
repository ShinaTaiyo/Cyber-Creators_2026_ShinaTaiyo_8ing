//================================
//
//「気」の蓄積(EffectAccumulation.h)
// Auther 木田威吹
// 
//================================
#ifndef _EFFECTACCUMULATION_H_
#define _EFFECTACCUMULATION_H_

//====================================
// インクルード
//====================================

#include "pch.h"  // プリコンパイル済みヘッダー
#include "object_2D.h"  // オブジェクト2Dの情報を使う
#include "summon_manager.h"

//====================================
// 前方宣言
//====================================


// 頂点フォーマット(気シェーダー専用)
#define FVF_SCREENVERTEX (D3DFVF_XYZRHW | D3DFVF_TEX1)

class CEffectAccumulation :public CObject_2D
{
public:
	CEffectAccumulation();	// コンストラクタ
	~CEffectAccumulation() override;	// デストラクタ
	HRESULT Init() override;			// 初期設定
	void Uninit() override;			// 終了処理
	void Update() override;			// 更新処理
	void Draw() override;			// 描画処理
	static CEffectAccumulation* Create();	// 生成

private:
	LPDIRECT3DTEXTURE9 m_sceneRenderTarget;
	LPDIRECT3DTEXTURE9 m_fireTexture;
	LPD3DXEFFECT m_fireEffect;
	LPDIRECT3DSURFACE9 m_sceneSurface;
	LPDIRECT3DVERTEXBUFFER9 m_fullscreenVB; // フルスクリーンクアッド用
	float m_fireTime;
	std::string m_fxFilePath;             // 読み込む.fxファイルのパス

	CSummon_Manager* m_pSuummonManager;

	struct DeformWave {
		float time = 0.0f;
		float duration = 0.05f; // 秒
		float power = 0.03f;    // この波の強さ
		float timeScale = 0.025f;  // 加算倍率
		bool active = false;   // 現在アクティブか
	};
	DeformWave currentWave;

	// 頂点の構造体
	struct ScreenVertex {
		D3DXVECTOR4 pos;
		D3DXVECTOR2 uv;
	};


	// カラー保持変数
	float m_colorR;
	float m_colorG;
	float m_colorB;
	// 円と尖りの強弱変数
	float m_circlePowor;
	float m_sharpnessPowor;
	float m_totalDeformPower;
	float m_circleSize;

	bool m_enableEffect;  // エフェクトの表示状態
	bool m_bActive;
	bool m_bSwitch;



	int pressCount;
	float powerPerPress;   // 押すごとの増加量
	float maxPower;
	float baseValue;

	bool InitPostEffect(LPDIRECT3DDEVICE9 device, const char* fxFile);  // シェーダーと各処理の初期化
	void ShutdownPostEffect();  // 変数の破棄
	void UpdatePostEffect(float deltaTime);  // シェーダーの更新
	void RenderEffect(LPDIRECT3DDEVICE9 device);  // シェーダーの描画

	float GetWaveStrength(float t);
	float EaseInSmooth(float t);



	static constexpr int TEX_SIZE_X = 1280;  // テクスチャ座標 X
	static constexpr int TEX_SIZE_Y = 720;   // テクスチャ座標 Y

	static constexpr float TEX_F_SIZE_X = 1280;  // テクスチャ座標 X
	static constexpr float TEX_F_SIZE_Y = 720;   // テクスチャ座標 Y

	static constexpr float MAX_SIZE = 5.5f;   // エフェクトの最大サイズ
	static constexpr float MAX_POWOR = 0.5f;   // 円と尖りの変更後の最大の強さ
	static constexpr float NORMAL_POWOR_CIRCLE = 0.3f;   // 円の通常の強さ
	static constexpr float NORMAL_POWOR_SHARPNESS = 0.1f;   // 尖りの通常の強さ
	static constexpr float VALUE = 4.0f;   // 強弱が変わるときの一定値

	static constexpr float MIN_SIZE = 2.0f;  // エフェクトサイズの最小値
	static constexpr float START_RATIO = 0.4f;   // エフェクト開始位置

};


#endif