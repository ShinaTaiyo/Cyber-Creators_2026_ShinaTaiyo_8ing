//============================================================================
// 
// HayuLib�A�w�b�_�t�@�C�� [HayuLib.hpp]
// Author : ���c����
// 
//============================================================================

// �C���N���[�h�K�[�h
#pragma once

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************

// DirectX9�t�@�C���g�p
#include <d3dx9.h>

//�@JSON�g�p
#include <nlohmann/json.hpp>

// C++
#include <fstream>

// ���̑�
#include <stringapiset.h> 
#include <windows.h>

// �p�X��A������SafeOpenJsonFile���Ăяo��
//nlohmann::json MakePathJson(const std::string& FilePath);

//****************************************************
// ���O���hws�̒�`
//****************************************************
namespace hws
{
	// ���
	template <typename T> void MyDelete(T& Obj);

	// �I������
	template <typename T> void MyRelease(T& Obj);
	template <typename T> void MyUninit(T& Obj);

	// �_�E���L���X�g
	template <typename T1, typename T2> T1* DownCast(T2* pBase);

	// �^����string�^�ɕϊ�
	template <typename T> const std::string& TypeConvetToString(T type);

	// ��������
	template <typename T> T GetRandomValue();
	template <typename T> auto GetRandomValue(T Range) -> decltype(Range);

	// �p�x�n
	void NormalizeAngleToDest(float& fAngle, const float& fDest);						// �ڕW�l�֊p�x�������� - �@
	void NormalizeAngleToDest(float& fAngle, const float& fDest, const float& fRadius);	// �ڕW�l�֊p�x�������� - �A
	void WrapAngleToRange(float& fAngle);												// �p�x��͈͓��Ɏ��߂� - �@
	void WrapAngleToRange(float& fAngle1, float& fAngle2);								// �p�x��͈͓��Ɏ��߂� - �A

	// JSON�t�@�C���֘A
	nlohmann::json	OpenJsonFile(const std::string& FilePath);		// JSON�t�@�C���̓W�J
	nlohmann::json	SafeOpenJsonFile(const std::string& FilePath);	// JSON�t�@�C���̓W�J - ��O���o
	D3DXVECTOR2		JsonConvertToVec2(const nlohmann::json& JSON);	// JSON�f�[�^��D3DXVECTOR2�ɕϊ�
	D3DXVECTOR3		JsonConvertToVec3(const nlohmann::json& JSON);	// JSON�f�[�^��D3DXVECTOR3�ɕϊ�
	D3DXCOLOR		JsonConvertToXCol(const nlohmann::json& JSON);	// JSON�f�[�^��D3DXCOLR�ɕϊ�
	std::string		JsonConvertToSJIS(const nlohmann::json& JSON);	// UTF8��JSON�f�[�^��SJIS�ɕϊ�
};