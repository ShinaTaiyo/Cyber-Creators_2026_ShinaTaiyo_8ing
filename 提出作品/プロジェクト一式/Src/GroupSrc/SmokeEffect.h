//==============================================
// 
// ポストエフェクト[煙] <SmokeEffect.h>
// Auther 木田威吹
// 
//==============================================
#ifndef _SMOKEEFFECT_H_
#define _SMOKEEFFECT_H_


#include "pch.h"
#include "object_2D.h"

// 霧の頂点フォーマット
#define SMOKE_FVF (D3DFVF_XYZRHW | D3DFVF_TEX1)


class CSmokeEffect :public CObject_2D
{
public:
	// === 特殊関数 ===

	CSmokeEffect();		// コンストラクタ
	~CSmokeEffect()override;		// デストラクタ

	// === ライフサイクルメンバ関数 ===

	HRESULT Init()override;	// 初期設定
	void Uninit()override;		// 終了処理
	void Update()override;		// 更新処理
	void Draw()override;		// 描画処理

	// === 静的メンバ関数 ===

	static CSmokeEffect* Create();
	static void SetUseDraw(bool bUse); // 描画するかどうか設定
private:
	// === 静的メンバ変数 ===
	static bool s_bUseDraw; // 処理を使用するかどうか

	bool InitSmokeEffect(LPDIRECT3DDEVICE9 device, const char* fxFile);
	void UpdateSmokeEffect(float deltaTime);
	void RenderSmokeEffect(LPDIRECT3DDEVICE9 device);
	void InitScreenQuad(LPDIRECT3DDEVICE9 device);

	struct Vertex {
		float x, y, z, rhw;
		float u, v;
	};
	LPD3DXEFFECT m_pEffect;    // エフェクト
	std::string m_fxFilePath;             // 読み込む.fxファイルのパス

	LPDIRECT3DTEXTURE9 m_noiseTex;
	LPDIRECT3DVERTEXBUFFER9 m_quadVB;

	float m_time;
	float m_Opacity;
	static constexpr int SCR_WIDTH = 1280;
	static constexpr int SCR_HEIGHT = 720;

	// ImGui
	bool m_enableEffect;  // エフェクトの表示状態
	bool m_bActive;
};

#endif