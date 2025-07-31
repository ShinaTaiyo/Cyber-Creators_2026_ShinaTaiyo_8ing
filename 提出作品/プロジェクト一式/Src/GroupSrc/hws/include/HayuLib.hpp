//============================================================================
// 
// HayuLib、ヘッダファイル [HayuLib.hpp]
// Author : 福田歩希
// 
//============================================================================

// インクルードガード
#pragma once

//****************************************************
// インクルードファイル
//****************************************************

// DirectX9ファイル使用
#include <d3dx9.h>

//　JSON使用
#include <nlohmann/json.hpp>

// C++
#include <fstream>

// その他
#include <stringapiset.h> 
#include <windows.h>

// パスを連結してSafeOpenJsonFileを呼び出す
//nlohmann::json MakePathJson(const std::string& FilePath);

//****************************************************
// 名前空間hwsの定義
//****************************************************
namespace hws
{
	// 解放
	template <typename T> void MyDelete(T& Obj);

	// 終了処理
	template <typename T> void MyRelease(T& Obj);
	template <typename T> void MyUninit(T& Obj);

	// ダウンキャスト
	template <typename T1, typename T2> T1* DownCast(T2* pBase);

	// 型情報をstring型に変換
	template <typename T> const std::string& TypeConvetToString(T type);

	// 乱数生成
	template <typename T> T GetRandomValue();
	template <typename T> auto GetRandomValue(T Range) -> decltype(Range);

	// 角度系
	void NormalizeAngleToDest(float& fAngle, const float& fDest);						// 目標値へ角度を向ける - ①
	void NormalizeAngleToDest(float& fAngle, const float& fDest, const float& fRadius);	// 目標値へ角度を向ける - ②
	void WrapAngleToRange(float& fAngle);												// 角度を範囲内に収める - ①
	void WrapAngleToRange(float& fAngle1, float& fAngle2);								// 角度を範囲内に収める - ②

	// JSONファイル関連
	nlohmann::json	OpenJsonFile(const std::string& FilePath);		// JSONファイルの展開
	nlohmann::json	SafeOpenJsonFile(const std::string& FilePath);	// JSONファイルの展開 - 例外送出
	D3DXVECTOR2		JsonConvertToVec2(const nlohmann::json& JSON);	// JSONデータをD3DXVECTOR2に変換
	D3DXVECTOR3		JsonConvertToVec3(const nlohmann::json& JSON);	// JSONデータをD3DXVECTOR3に変換
	D3DXCOLOR		JsonConvertToXCol(const nlohmann::json& JSON);	// JSONデータをD3DXCOLRに変換
	std::string		JsonConvertToSJIS(const nlohmann::json& JSON);	// UTF8のJSONデータをSJISに変換
};