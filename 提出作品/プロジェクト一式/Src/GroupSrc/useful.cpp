//============================================================================
// 
// ���[�X�t�� [useful.cpp]
// Author : ���c����
// 
//============================================================================

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "useful.hpp"
#include "logger.h"

//****************************************************
// using�f�B���N�e�B�u
//****************************************************
using namespace useful;

//============================================================================
// UV�}�b�s���O������
//============================================================================
void useful::InitializeTextureMappingInVertex(VERTEX_2D*& refVtx)
{
	refVtx[0].tex = { 0.0f, 0.0f };
	refVtx[1].tex = { 1.0f, 0.0f };
	refVtx[2].tex = { 0.0f, 1.0f };
	refVtx[3].tex = { 1.0f, 1.0f };
}

//============================================================================
// UV�}�b�s���O������
//============================================================================
void useful::InitializeTextureMappingInVertex(VERTEX_3D*& refVtx)
{
	refVtx[0].tex = { 0.0f, 0.0f };
	refVtx[1].tex = { 1.0f, 0.0f };
	refVtx[2].tex = { 0.0f, 1.0f };
	refVtx[3].tex = { 1.0f, 1.0f };
}

//============================================================================
// ��]�x�N�g���̊p�x����
//============================================================================
void AdjustDirectionYawPitch(Vec3& vec3)
{
	/* �ǉ��\�� */
}

//============================================================================
// ���[���h�s��v�Z
//============================================================================
void useful::UpdateMtxWorld(Mtx& mtx, const Vec3& scale, const Vec3& rot, const Vec3& pos)
{
	// �v�Z�p�s��
	Mtx mtxScale, mtxRot, mtxTrans;

	// ���[���h�s���������
	D3DXMatrixIdentity(&mtx);

	if (scale != VEC3_INIT)
	{
		// �g�k�s��쐬
		D3DXMatrixScaling(&mtxScale, scale.y, scale.x, scale.z);

		// �g�k�s��Ƃ̊|���Z
		D3DXMatrixMultiply(&mtx, &mtx, &mtxScale);
	}

	if (rot != VEC3_INIT)
	{
		// ��]�s��쐬
		D3DXMatrixRotationYawPitchRoll(&mtxRot, rot.y, rot.x, rot.z);

		// ��]�s��Ƃ̊|���Z
		D3DXMatrixMultiply(&mtx, &mtx, &mtxRot);
	}

	if (pos != VEC3_INIT)
	{
		// ���s�ړ��s��쐬
		D3DXMatrixTranslation(&mtxTrans, pos.x, pos.y, pos.z);

		// ���s�ړ��s��Ƃ̊|���Z
		D3DXMatrixMultiply(&mtx, &mtx, &mtxTrans);
	}
}

//============================================================================
// ImGui�̃E�B���h�E���� - �@
//============================================================================
bool useful::MIS::MyImGuiShortcut_BeginWindow(const char* name)
{
	ImGui::SetNextWindowSize({ -1, -1 });
	ImGui::SetNextWindowPos({ 0, 0 }, ImGuiCond_FirstUseEver);

	return ImGui::Begin(name);
}

//============================================================================
// ImGui�̃E�B���h�E���� - �A
//============================================================================
bool useful::MIS::MyImGuiShortcut_BeginWindow(const char* name, const ImVec2& rect)
{
	ImGui::SetNextWindowSize(rect);
	ImGui::SetNextWindowPos({ 0, 0 }, ImGuiCond_FirstUseEver);

	return ImGui::Begin(name);
}

//============================================================================
// ImGui�̃E�B���h�E���� - �B
//============================================================================
bool useful::MIS::MyImGuiShortcut_BeginWindow(const char* name, const ImVec2& rect, const ImVec2& pos)
{
	ImGui::SetNextWindowSize(rect);
	ImGui::SetNextWindowPos(pos);

	return ImGui::Begin(name);
}

//============================================================================
// ImGui�̃E�B���h�E���� - �C
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
	// �E�B���h�E�ƃE�B�W�F�b�g�̕����R�s�[
	float
		fWindowWidth = ImGui::GetContentRegionAvail().x,
		fSeparate = ImGui::GetStyle().ItemSpacing.x,
		fNumButtons = static_cast<float>(1.0f * num);

	// 1������̃{�^���̃T�C�Y
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
	// �E�B���h�E�ƃE�B�W�F�b�g�̕����R�s�[
	float
		fWindowWidth = ImGui::GetContentRegionAvail().x,
		fSeparate = ImGui::GetStyle().ItemSpacing.x,
		fNumButtons = static_cast<float>(1.0f * num);

	// 1������̃{�^���̃T�C�Y
	ImVec2 Rect =
	{
		(fWindowWidth + -fSeparate * fNumButtons) / (fNumButtons + 1.0f),
		height
	};

	return Rect;
}

//============================================================================
// Vector�n�\���̂̃h���b�O���� - �@
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
// Vector�n�\���̂̃h���b�O���� - �A
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
// Vector�n�\���̂̃��Z�b�g���� - �@
//============================================================================
void useful::MIS::MyImGuiShortcut_ResetVector(const char* label, Vec2& vec2)
{
	std::string
		sX = static_cast<std::string>("X##") + label,
		sY = static_cast<std::string>("Y##") + label,
		sAll = static_cast<std::string>("All##") + label;

	// ��s���ăe�L�X�g��`��A�c��̌��ԂɎ����Ń{�^����~���l�߂�
	ImGui::Text(u8"���Z�b�g :");
	ImGui::SameLine();

	// 1������̃{�^���̃T�C�Y���擾
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
// �����O���[�v�̌`���ϊ�
//============================================================================
void useful::OldModelPathConverterForSummonPettern(JSON& json)
{
	// ���f���p�X���擾
	const std::string& sModelPath = json["ModelName"];

	// �������p�X�w��̌`��������
	decltype(sModelPath.find("MODEL")) FindResult = sModelPath.find("MODEL");

	// �������`���Ȃ牽�����Ȃ�
	if (FindResult != -1)
	{
		return;
	}

	// �Â��`���̏ꍇ�A�㔼�̃t�@�C�������擾����
	decltype(sModelPath.find_last_of("\\")) LastSlashPos = sModelPath.find_last_of("\\");

	// �p�X���č\��
	json["ModelName"] = FRONT_PATH_FOR_CONVERTER + sModelPath.substr(LastSlashPos);
}

//============================================================================
// Vector�n�\���̂̃��Z�b�g���� - �A
//============================================================================
void useful::MIS::MyImGuiShortcut_ResetVector(const char* label, Vec2& vec2, float initVal)
{
	std::string
		sX = static_cast<std::string>("X##") + label,
		sY = static_cast<std::string>("Y##") + label,
		sAll = static_cast<std::string>("All##") + label;

	// ��s���ăe�L�X�g��`��A�c��̌��ԂɎ����Ń{�^����~���l�߂�
	ImGui::Text(u8"���Z�b�g :");
	ImGui::SameLine();

	// 1������̃{�^���̃T�C�Y���擾
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
// Vector�n�\���̂̃��Z�b�g���� - �B
//============================================================================
void useful::MIS::MyImGuiShortcut_ResetVector(const char* label, Vec3& vec3)
{
	std::string
		sX = static_cast<std::string>("X##") + label,
		sY = static_cast<std::string>("Y##") + label,
		sZ = static_cast<std::string>("Z##") + label,
		sAll = static_cast<std::string>("All##") + label;

	// ��s���ăe�L�X�g��`��A�c��̌��ԂɎ����Ń{�^����~���l�߂�
	ImGui::Text(u8"���Z�b�g :");
	ImGui::SameLine();

	// 1������̃{�^���̃T�C�Y���擾
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
// Vector�n�\���̂̃��Z�b�g���� - �C
//============================================================================
void useful::MIS::MyImGuiShortcut_ResetVector(const char* label, Vec3& vec3, float initVal)
{
	std::string
		sX = static_cast<std::string>("X##") + label,
		sY = static_cast<std::string>("Y##") + label,
		sZ = static_cast<std::string>("Z##") + label,
		sAll = static_cast<std::string>("All##") + label;

	// ��s���ăe�L�X�g��`��A�c��̌��ԂɎ����Ń{�^����~���l�߂�
	ImGui::Text(u8"���Z�b�g :");
	ImGui::SameLine();

	// 1������̃{�^���̃T�C�Y���擾
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