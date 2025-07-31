//============================================================================
// 
// ベジェ曲線マネージャー、ヘッダファイル [bezier_manager.h]
// Author : 福田歩希
// 
//============================================================================

// インクルードガード
#pragma once

//****************************************************
// インクルードファイル
//****************************************************
#include "singleton.hpp"

// コンポジション
#include "registry.hpp"
#include "quadratic_bezier.h"

//****************************************************
// ベジェ曲線マネージャークラスの定義
//****************************************************
class CBezier_Manager final : public CSingleton<CBezier_Manager>
{
	//****************************************************
	// フレンド宣言
	//****************************************************
	friend struct std::default_delete<CBezier_Manager>;
	friend CBezier_Manager* CSingleton<CBezier_Manager>::GetInstance();
	friend void CSingleton<CBezier_Manager>::ExplicitRelease();

	//****************************************************
	// 静的メンバ変数の定義
	//****************************************************
	static constexpr const char* INITIALIZE_PATH = "Data\\JSON\\INITIALIZE\\Bezier_Manager.json";
	static std::string RELOAD_PATH;
	
public:

	//****************************************************
	// function
	//****************************************************

	/* 応急処置 */
	static inline void SetReloadPath(const char* path) { RELOAD_PATH = path; };

	/* 応急処置 */
	void ExplicitInitialize();

	/**
	 * 保有している全てのベジェ曲線インスタンスの更新処理を行います
	 * 返り値は更新の完了状況、リバースフラグ不使用の曲線の更新が完了しているとtrueを返します
	 */
	bool Update();

	/**
	 * 保有している全てのベジェ曲線インスタンスの描画処理を行います
	 * 返り値無し
	 */
	void Draw();

	// 更新中のベジェ曲線の二次軌跡上の移動点の座標を取得
	inline const D3DXVECTOR3& GetPosTracePointOnCurrentBeizier() { return m_CurrentPos; };

	// レジストリの参照の取得
	inline CRegistry<std::unique_ptr<CQuadratic_Bezier>>& RefRegistry() const { return *m_upRegistry; };

#if 0
	/* GetTracePointOnTrajectoryLineのラップ */
	inline CObject_X* GetTracePointOnTrajectoryLine() const
	{
		// 要素のインデックス
		int nElementIdx = 0;

		// 全インスタンスの更新処理
		for (auto& it : m_Register->RefDynamicResource())
		{
			// この要素が対象のインデックスでない場合無視
			if (m_nOnIdx != nElementIdx)
			{
				// 要素のインデックスをインクリメント
				++nElementIdx;

				continue;
			}

			return it.second->GetTracePointOnTrajectoryLine();
		}
		
		return nullptr;
	}
#endif

private:

	//****************************************************
	// special function
	//****************************************************
	CBezier_Manager();				// デフォルトコンストラクタ
	~CBezier_Manager() override;	// デストラクタ

	//****************************************************
	// function
	//****************************************************
	bool Initialize();	// 初期化処理
	void Finalize();	// 終了処理

	//****************************************************
	// data
	//****************************************************
	std::unique_ptr<CRegistry<std::unique_ptr<CQuadratic_Bezier>>> m_upRegistry;	// ベジェ曲線用レジストリ

	int m_nIdxCurrentBeizier;	// 更新中のベジェ曲線のインデックス
	int m_nMaxSizeRegistry;		// 要素の最大数
	D3DXVECTOR3 m_CurrentPos;	/* 更新中の座標 */
};