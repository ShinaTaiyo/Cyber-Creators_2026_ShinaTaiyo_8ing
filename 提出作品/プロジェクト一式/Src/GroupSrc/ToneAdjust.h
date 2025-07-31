//==============================================
// 
// ポストエフェクト[色調変化] <ToneAdjust.h>
// Auther 木田威吹
// 
//==============================================
#ifndef _TONEADJUST_H_
#define _TONEADJUST_H_  // 二重インクルード防止

//====================================
// インクルード
//====================================
#include "pch.h"  // プリコンパイル済みヘッダー
#include "object_2D.h"  // オブジェクト2Dの情報を使う

// 頂点フォーマット(色調変化シェーダー専用)
#define FVF_SCREEN_VERTEX (D3DFVF_XYZRHW | D3DFVF_TEX1)

// 色調変化シェーダークラス
class CToneAdjust :public CObject_2D
{
public:
	CToneAdjust();		//コンストラクタ
	~CToneAdjust()override;		//デストラクタ
	HRESULT Init()override;	//初期設定
	void Uninit()override;		//終了処理
	void Update()override;		//更新処理
	void Draw()override;		//描画処理

	static CToneAdjust* Create();  // 生成処理
	static void SetUseDraw(bool bUse); // 描画するかどうか設定

private:
	bool InitToneAdjust(LPDIRECT3DDEVICE9 device, const char* fxFile);  // シェーダーの初期化
	void UninitToneAdjust();   // シェーダーの破棄
	void UpdateToneAdjust(float deltaTime);   // シェーダーの更新

	void RenderToneAdjust(LPDIRECT3DDEVICE9 device);  // シェーダーの描画
	void CreateFullScreenQuad(LPDIRECT3DDEVICE9 device);  // ポリゴンの描画

	LPD3DXEFFECT m_pEffect;    // エフェクト
	std::string m_fxFilePath;             // 読み込む.fxファイルのパス

	// テクスチャの構造体
	struct SCREEN_VERTEX {
		float x, y, z, rhw;
		float u, v;
	};
	bool m_enableEffect;  // エフェクトの表示状態
	bool m_bActive;   // ImGuiの表示状態
	// カラー保持変数
	float m_colorR;
	float m_colorG;
	float m_colorB;

	float m_Alpha;  // 透明度

	// deltaTime の時間を入れる変数
	float m_totalTime;
	bool m_bLevelBarEnabled;
	bool m_bChange;  // 色の切り替え
	int m_nCountFrame;  // カウント用フレーム

	//==== フェード関連の変数 ======
	float m_fadeProgress;  // フェードの進捗具合
	float m_FadeDirection;
	bool m_FadeActive;   // フェードの切り替え

	// 各マクロの定義
	static constexpr float FADE_SPEED = 0.008f;  // フェード自体の速度
	static constexpr float FADE_SPEED_DOWN = 0.015f;  // フェード自体の速度

	static bool s_bUseDraw; // 処理を使用するかどうか

};

#endif