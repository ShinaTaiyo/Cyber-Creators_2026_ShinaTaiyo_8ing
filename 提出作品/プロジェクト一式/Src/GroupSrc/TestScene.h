//============================================================================
// 
// ２０２５年５月３０日：プレイヤーをシーンごとに派生させる [TestScene.h]
// Author : 福田歩希
// 
//============================================================================

#ifndef _TestScene_H_
#define _TestScene_H_	// 二重インクルード防止

//****************************************************
// インクルードファイル
//****************************************************
#include "scene.h"
#include "summon_manager.h"
#include "timer.h" // タイマー
#include "EffectAccumulation.h" // 「気」の蓄積シェーダー
#include "ToneAdjust.h"  // 色調変化のシェーダー
#include "SmokeEffect.h"  // 霧のシェーダー

//****************************************************
// 前方宣言
//****************************************************
class CVisionBlockSpawner; // 視野妨害オブジェクトスポナー
class CEffectAccumulation;  // 「気」も蓄積シェーダー

//****************************************************
// ゲームクラス
//****************************************************
class CTestScene : public CScene
{
public:

	// === ライフサイクルメンバ関数 ===

	void Update() override;		// 更新処理
	void Draw() override;		// 描画処理

	// === メンバ関数 ===

	void To_Next() override; // 次のシーンへ

	// === 静的メンバ関数 ===

	static CTestScene* Create();	// ゲームを生成
	static bool& GetTest(); // テスト配置を確認するかどうか
private:
	// === 静的メンバ変数 ===
	static bool s_bTest; // テスト配置を確認するかどうか

	// === 特殊関数 ===
	CTestScene();			// コンストラクタ
	~CTestScene() override;	// デストラクタ

	// === 構造体 ===

	// 基本召喚情報
	struct SummonInfo
	{
		std::string Name;                       // 名前
		D3DXVECTOR3 Pos = { 0.0f,0.0f,0.0f };   // 位置
		D3DXVECTOR3 Move = { 0.0f,0.0f,0.0f };  // 移動量
		D3DXVECTOR3 Scale = { 0.0f,0.0f,0.0f }; // 拡大率
		int nSummonFrame = 0;                   // 召喚フレーム
	};

	// === ライフサイクルメンバ関数 ===

	HRESULT Init() override;	            // 初期設定
	void	Uninit() override;	            // 終了処理

	// === メンバ変数 ===

	bool m_bEnd;                            // ゲームシーン終了フラグ
	CEffectAccumulation* m_pEffectAccumulation; // 「気」の蓄積シェーダーのポインタ
	CToneAdjust* m_pToneAdjust;             // 色調変化のシェーダーのポインタ
	CSmokeEffect* m_pSmokeEffect;           // 霧のシェーダーのポインタ
};

#endif // _TestScene_H_