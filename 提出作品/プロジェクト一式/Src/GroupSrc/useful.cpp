//============================================================================
// 
// ユースフル [useful.cpp]
// Author : 福田歩希
// 
//============================================================================

//****************************************************
// インクルードファイル
//****************************************************
#include "useful.hpp"
#include "logger.h"

//****************************************************
// usingディレクティブ
//****************************************************
using namespace useful;

//============================================================================
// UVマッピング初期化
//============================================================================
void useful::InitializeTextureMappingInVertex(VERTEX_2D*& refVtx)
{
	refVtx[0].tex = { 0.0f, 0.0f };
	refVtx[1].tex = { 1.0f, 0.0f };
	refVtx[2].tex = { 0.0f, 1.0f };
	refVtx[3].tex = { 1.0f, 1.0f };
}

//============================================================================
// UVマッピング初期化
//============================================================================
void useful::InitializeTextureMappingInVertex(VERTEX_3D*& refVtx)
{
	refVtx[0].tex = { 0.0f, 0.0f };
	refVtx[1].tex = { 1.0f, 0.0f };
	refVtx[2].tex = { 0.0f, 1.0f };
	refVtx[3].tex = { 1.0f, 1.0f };
}

//============================================================================
// 回転ベクトルの角度制限
//============================================================================
void AdjustDirectionYawPitch(Vec3& vec3)
{
	/* 追加予定 */
}

//============================================================================
// ワールド行列計算
//============================================================================
void useful::UpdateMtxWorld(Mtx& mtx, const Vec3& scale, const Vec3& rot, const Vec3& pos)
{
	// 計算用行列
	Mtx mtxScale, mtxRot, mtxTrans;

	// ワールド行列を初期化
	D3DXMatrixIdentity(&mtx);

	if (scale != VEC3_INIT)
	{
		// 拡縮行列作成
		D3DXMatrixScaling(&mtxScale, scale.y, scale.x, scale.z);

		// 拡縮行列との掛け算
		D3DXMatrixMultiply(&mtx, &mtx, &mtxScale);
	}

	if (rot != VEC3_INIT)
	{
		// 回転行列作成
		D3DXMatrixRotationYawPitchRoll(&mtxRot, rot.y, rot.x, rot.z);

		// 回転行列との掛け算
		D3DXMatrixMultiply(&mtx, &mtx, &mtxRot);
	}

	if (pos != VEC3_INIT)
	{
		// 平行移動行列作成
		D3DXMatrixTranslation(&mtxTrans, pos.x, pos.y, pos.z);

		// 平行移動行列との掛け算
		D3DXMatrixMultiply(&mtx, &mtx, &mtxTrans);
	}
}

//============================================================================
// ImGuiのウィンドウ生成 - ①
//============================================================================
bool useful::MIS::MyImGuiShortcut_BeginWindow(const char* name)
{
	ImGui::SetNextWindowSize({ -1, -1 });
	ImGui::SetNextWindowPos({ 0, 0 }, ImGuiCond_FirstUseEver);

	return ImGui::Begin(name);
}

//============================================================================
// ImGuiのウィンドウ生成 - ②
//============================================================================
bool useful::MIS::MyImGuiShortcut_BeginWindow(const char* name, const ImVec2& rect)
{
	ImGui::SetNextWindowSize(rect);
	ImGui::SetNextWindowPos({ 0, 0 }, ImGuiCond_FirstUseEver);

	return ImGui::Begin(name);
}

//============================================================================
// ImGuiのウィンドウ生成 - ③
//============================================================================
bool useful::MIS::MyImGuiShortcut_BeginWindow(const char* name, const ImVec2& rect, const ImVec2& pos)
{
	ImGui::SetNextWindowSize(rect);
	ImGui::SetNextWindowPos(pos);

	return ImGui::Begin(name);
}

//============================================================================
// ImGuiのウィンドウ生成 - ④
//============================================================================
bool useful::MIS::MyImGuiShortcut_BeginWindow(const char* name, const ImVec2& rect, const ImVec2& pos, ImGuiWindowFlags flags)
{
	ImGui::SetNextWindowSize(rect);
	ImGui::SetNextWindowPos(pos);

	return ImGui::Begin(name, nullptr, flags);
}

//============================================================================
// 
//============================================================================
ImVec2 useful::MIS::CalcButtonSpacing(unsigned short num)
{
	// ウィンドウとウィジェットの幅をコピー
	float
		fWindowWidth = ImGui::GetContentRegionAvail().x,
		fSeparate = ImGui::GetStyle().ItemSpacing.x,
		fNumButtons = static_cast<float>(1.0f * num);

	// 1つあたりのボタンのサイズ
	ImVec2 Rect =
	{
		(fWindowWidth + -fSeparate * (fNumButtons - 1.0f)) / fNumButtons,
		25.0f
	};

	return Rect;
}

//============================================================================
// 
//============================================================================
ImVec2 useful::MIS::CalcButtonSpacing(unsigned short num, float height)
{
	// ウィンドウとウィジェットの幅をコピー
	float
		fWindowWidth = ImGui::GetContentRegionAvail().x,
		fSeparate = ImGui::GetStyle().ItemSpacing.x,
		fNumButtons = static_cast<float>(1.0f * num);

	// 1つあたりのボタンのサイズ
	ImVec2 Rect =
	{
		(fWindowWidth + -fSeparate * fNumButtons) / (fNumButtons + 1.0f),
		height
	};

	return Rect;
}

//============================================================================
// Vector系構造体のドラッグ操作 - ①
//============================================================================
void useful::MIS::MyImGuiShortcut_DragVector(const char* label, Vec2& vec2, float speed)
{
	std::string sLabel = static_cast<std::string>("##") + label;

	ImGui::Text(label);
	ImGui::SameLine();
	ImGui::SetNextItemWidth(-1.0f);
	ImGui::DragFloat2(sLabel.c_str(), vec2, speed);
}

//============================================================================
// Vector系構造体のドラッグ操作 - ②
//============================================================================
void useful::MIS::MyImGuiShortcut_DragVector(const char* label, Vec3& vec3, float speed)
{
	std::string sLabel = static_cast<std::string>("##") + label;

	ImGui::Text(label);
	ImGui::SameLine();
	ImGui::SetNextItemWidth(-1.0f);
	ImGui::DragFloat3(sLabel.c_str(), vec3, speed);
}

//============================================================================
// Vector系構造体のリセット操作 - ①
//============================================================================
void useful::MIS::MyImGuiShortcut_ResetVector(const char* label, Vec2& vec2)
{
	std::string
		sX = static_cast<std::string>("X##") + label,
		sY = static_cast<std::string>("Y##") + label,
		sAll = static_cast<std::string>("All##") + label;

	// 先行してテキストを描画、残りの隙間に自動でボタンを敷き詰める
	ImGui::Text(u8"リセット :");
	ImGui::SameLine();

	// 1つあたりのボタンのサイズを取得
	const ImVec2& Rect = CalcButtonSpacing(3);

	if (ImGui::Button(sX.c_str(), Rect))
	{
		vec2.x = 0.0f;
	}
	ImGui::SameLine();
	if (ImGui::Button(sY.c_str(), Rect))
	{
		vec2.y = 0.0f;
	}
	ImGui::SameLine();
	if (ImGui::Button(sAll.c_str(), Rect))
	{
		vec2 = { 0.0f, 0.0f };
	}
}

//============================================================================
// 召喚グループの形式変換
//============================================================================
void useful::OldModelPathConverterForSummonPettern(JSON& json)
{
	// モデルパスを取得
	const std::string& sModelPath = json["ModelName"];

	// 正しいパス指定の形式か検証
	decltype(sModelPath.find("MODEL")) FindResult = sModelPath.find("MODEL");

	// 正しい形式なら何もしない
	if (FindResult != -1)
	{
		return;
	}

	// 古い形式の場合、後半のファイル名を取得する
	decltype(sModelPath.find_last_of("\\")) LastSlashPos = sModelPath.find_last_of("\\");

	// パスを再構成
	json["ModelName"] = FRONT_PATH_FOR_CONVERTER + sModelPath.substr(LastSlashPos);
}

//============================================================================
// Vector系構造体のリセット操作 - ②
//============================================================================
void useful::MIS::MyImGuiShortcut_ResetVector(const char* label, Vec2& vec2, float initVal)
{
	std::string
		sX = static_cast<std::string>("X##") + label,
		sY = static_cast<std::string>("Y##") + label,
		sAll = static_cast<std::string>("All##") + label;

	// 先行してテキストを描画、残りの隙間に自動でボタンを敷き詰める
	ImGui::Text(u8"リセット :");
	ImGui::SameLine();

	// 1つあたりのボタンのサイズを取得
	const ImVec2& Rect = CalcButtonSpacing(3);

	if (ImGui::Button(sX.c_str(), Rect))
	{
		vec2.x = initVal;
	}
	ImGui::SameLine();
	if (ImGui::Button(sY.c_str(), Rect))
	{
		vec2.y = initVal;
	}
	ImGui::SameLine();
	if (ImGui::Button(sAll.c_str(), Rect))
	{
		vec2 = { initVal, initVal };
	}
}

//============================================================================
// Vector系構造体のリセット操作 - ③
//============================================================================
void useful::MIS::MyImGuiShortcut_ResetVector(const char* label, Vec3& vec3)
{
	std::string
		sX = static_cast<std::string>("X##") + label,
		sY = static_cast<std::string>("Y##") + label,
		sZ = static_cast<std::string>("Z##") + label,
		sAll = static_cast<std::string>("All##") + label;

	// 先行してテキストを描画、残りの隙間に自動でボタンを敷き詰める
	ImGui::Text(u8"リセット :");
	ImGui::SameLine();

	// 1つあたりのボタンのサイズを取得
	const ImVec2& Rect = CalcButtonSpacing(4);

	if (ImGui::Button(sX.c_str(), Rect))
	{
		vec3.x = 0.0f;
	}
	ImGui::SameLine();
	if (ImGui::Button(sY.c_str(), Rect))
	{
		vec3.y = 0.0f;
	}
	ImGui::SameLine();
	if (ImGui::Button(sZ.c_str(), Rect))
	{
		vec3.z = 0.0f;
	}
	ImGui::SameLine();
	if (ImGui::Button(sAll.c_str(), Rect))
	{
		vec3 = { 0.0f, 0.0f, 0.0f };
	}
}

//============================================================================
// Vector系構造体のリセット操作 - ④
//============================================================================
void useful::MIS::MyImGuiShortcut_ResetVector(const char* label, Vec3& vec3, float initVal)
{
	std::string
		sX = static_cast<std::string>("X##") + label,
		sY = static_cast<std::string>("Y##") + label,
		sZ = static_cast<std::string>("Z##") + label,
		sAll = static_cast<std::string>("All##") + label;

	// 先行してテキストを描画、残りの隙間に自動でボタンを敷き詰める
	ImGui::Text(u8"リセット :");
	ImGui::SameLine();

	// 1つあたりのボタンのサイズを取得
	const ImVec2& Rect = CalcButtonSpacing(4);

	if (ImGui::Button(sX.c_str(), Rect))
	{
		vec3.x = initVal;
	}
	ImGui::SameLine();
	if (ImGui::Button(sY.c_str(), Rect))
	{
		vec3.y = initVal;
	}
	ImGui::SameLine();
	if (ImGui::Button(sZ.c_str(), Rect))
	{
		vec3.z = initVal;
	}
	ImGui::SameLine();
	if (ImGui::Button(sAll.c_str(), Rect))
	{
		vec3 = { initVal, initVal, initVal };
	}
}