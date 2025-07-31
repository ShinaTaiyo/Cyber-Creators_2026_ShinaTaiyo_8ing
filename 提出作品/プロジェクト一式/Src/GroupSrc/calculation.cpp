//============================================================================
// 
// �Q�O�Q�T�N�T���P�X�� [calculation.cpp]
// Author : ���c����
// 
////============================================================================

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "calculation.h" // ���N���X
#include "pch.h"         // �v���R���p�C��
#include "renderer.h"    // �����_���[
#include <string>
#include <locale>
#include <codecvt>

//****************************************************
// using�f�B���N�e�B�u
//****************************************************
using namespace useful;

//****************************************************
// �������O��Ԋ֐�
//****************************************************

//====================================================
// �_�Ɠ_�̋��������
//====================================================
float Calculation::Length::PointToPoint(D3DXVECTOR3 SelfPos, D3DXVECTOR3 OtherPos)
{
	// �v�Z
	float fLength = sqrtf(powf(OtherPos.x - SelfPos.x, 2) + powf(SelfPos.y - OtherPos.y, 2) + powf(SelfPos.z - OtherPos.z, 2));
	return fLength;
}

//====================================================
// �P�����̓_�Ɠ_�̋��������
//====================================================
float Calculation::Length::PointToPoint_1D(float fSelfPos, float fOtherPos)
{
	// �v�Z
	float fLength = sqrtf(powf(fOtherPos - fSelfPos, 2));
	return fLength;
}

//====================================================
// �Q�����̓_�Ɠ_�̋��������
//====================================================
float Calculation::Length::PointtoPoint_2D(D3DXVECTOR2 SelfPos, D3DXVECTOR3 OtherPos)
{
	// �v�Z
	float fLength = sqrtf(powf(OtherPos.x - SelfPos.x, 2) + powf(SelfPos.y - OtherPos.y, 2));
	return fLength;
}

//****************************************************
// �p�x���O��Ԋ֐�
//****************************************************

//====================================================
// �p�x�𒲐�����
//====================================================
void Calculation::Rot::Adjust(float & fSelfRot)
{
	if (fSelfRot > D3DX_PI)
	{
		fSelfRot -= D3DX_PI * 2;
	}
	else if (fSelfRot < -D3DX_PI)
	{
		fSelfRot += D3DX_PI * 2;
	}
}

//====================================================
// Yaw�����߂�
//====================================================
float Calculation::Rot::Yaw(D3DXVECTOR3 SelfPos, D3DXVECTOR3 OtherPos)
{
	D3DXVECTOR3 Vec = OtherPos - SelfPos; // �x�N�g��
	return atan2f(Vec.x, Vec.z);
}

//====================================================
// Pitch�����߂�
//====================================================
float Calculation::Rot::Pitch(D3DXVECTOR3 SelfPos, D3DXVECTOR3 OtherPos)
{
	D3DXVECTOR3 Vec = OtherPos - SelfPos; // �x�N�g��
	return atan2f(Vec.y,sqrtf(powf(Vec.x,2) + powf(Vec.z,2)));
}

//====================================================
// �p�x���𒲐�����
//====================================================
float Calculation::Rot::AdjustAngleDifference(float fSelfRot, float fOtherRot)
{
	// === �ϐ� ===

	float fRotDiff = fOtherRot - fSelfRot; // �p�x����

	// === ���� ===

	// ���߂��p�x�����֒���
	if (fRotDiff > D3DX_PI)
	{
		fRotDiff -= D3DX_PI * 2;
	}
	else if (fRotDiff < -D3DX_PI)
	{
		fRotDiff += D3DX_PI * 2;
	}

	return fRotDiff;
}

//====================================================
// ���[���h���W���X�N���[�����W�ɕϊ�����
//====================================================
D3DXVECTOR3 Calculation::Pos::WorldToScreen(D3DXVECTOR3 worldPos, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix, float screenWidth, float screenHeight)
{
	//���[���h���W���N���b�v���W�ɕϊ�
	D3DXVECTOR4 ClipSpacePos;
	D3DXMATRIX mtxTrans;

	mtxTrans = viewMatrix * projectionMatrix; // �r���[�|�[�g�A�v���W�F�N�V�����ϊ�
	D3DXVec3Transform(&ClipSpacePos, &worldPos, &mtxTrans); // ��L�̕ϊ��������̃��[���h���W�̈ʒu�����߂�

	//�������Z�i�N���b�v���W����NDC��Ԃցj�i�{����GPU������ɂ�邪�A�f�[�^�Ƃ��Ă͏o�Ȃ��̂Ŏ����ŋ��߂�)
	if (ClipSpacePos.w != 0.0f)
	{//X�AY�AZ�́A�J��������̑��ΓI�Ȉʒu��\���Ă���i�ϊ����ꂽ�ʒu)�B
	 //W�́A�J��������̋����i�[�x���j��\���Ă���
		ClipSpacePos.x /= ClipSpacePos.w;
		ClipSpacePos.y /= ClipSpacePos.w;
		ClipSpacePos.z /= ClipSpacePos.w;
	}

	//�X�N���[�����W�֕ϊ�
	D3DXVECTOR3 ScreenPos;
	ScreenPos.x = (ClipSpacePos.x * 0.5f + 0.5f) * screenWidth;           //�^�񒆂��O�ɂ��邽�߁{�O�D�T���A�[�P�Ȃ獶�[�A�{�P�Ȃ�E�[�ƂȂ�
	ScreenPos.y = (1.0f - (ClipSpacePos.y * 0.5f + 0.5f)) * screenHeight; //�^�񒆂��O�ɂ��邽�߁{�O�D�T���A�������ƂȂ�A�オ�O�ƂȂ�̂ŁA�P�̎��͂O�ɂȂ�A�[�P�̎��͂P�ƂȂ�
	ScreenPos.z = ClipSpacePos.z;                                         //�[�x�l�i�O�`�P�j�͈̔�
	return ScreenPos;
}

//===================================================================================================================
// �X�N���[�����W�����[���h���W�ɕϊ�
//===================================================================================================================
D3DXVECTOR3* Calculation::Pos::ScreenToWorld(D3DXVECTOR3* pout, float Sx, float Sy, float fZ, int Screen_w, int Screen_h, D3DXMATRIX* View, D3DXMATRIX* Prj)
{
	// �e�s��̋t�s����Z�o�i�r���[�A�v���W�F�N�V�����}�g���b�N�X�̋t�s���������̂́A�J�����̈ʒu��2D��UI���o�Ă���ƒ�`�ł��邩��)
	// �t�s��Ƃ́A�l�ɒl-1�������A�|�����킳���O�ɖ߂�����
	D3DXMATRIX InvView, InvPrj, VP, InvViewport; // �v�Z�p�}�g���b�N�X

	//�r���[�}�g���b�N�X�Ƃ̋t����������ă��[���h���W�����߂�i�r���[�}�g���b�N�X�̌�����ʒu�ɉ����ĕϊ�����Ă���̂Łj
	D3DXMatrixInverse(&InvView, NULL, View);

	// �v���W�F�N�V�����}�g���b�N�X�Ƃ̋t�s��i�������i���s���e�A����p�Ȃǁj��ς��Ă���̂ŁA�t�s����|�����킹�A���Ƃɖ߂��K�v������j
	D3DXMatrixInverse(&InvPrj, NULL, Prj);
	D3DXMatrixIdentity(&VP);

	// �X�P�[�����O�̒l��ς��Ă���B�X�N���[�����W�̒��S����ʒ����ɂ���iNDC���W���g�p���ĕϊ�����̂ŁA�X�N���[�����W�͂O�`�P�Q�W�O�A�Ȃ��A
	//NDC���W�́[�P�`�P������A�ϊ�����ۂɍ��킹��K�v������(�P�Q�W�O�@���@�U�S�O�j�A�i�O�@���@�[�U�S�O)
	VP._11 = Screen_w / 2.0f; VP._22 = -Screen_h / 2.0f; // �X�P�[�����O�����iX���̂܂܁AY�͏�𐳂Ɂj
	VP._41 = Screen_w / 2.0f; VP._42 = Screen_h / 2.0f;  // ���s�ړ������i���S�����_�ɍ��킹��)
	D3DXMatrixInverse(&InvViewport, NULL, &VP); // NDC���W�ɍ��킹��

	//����
	D3DXVECTOR3 MyPos = D3DXVECTOR3(Sx, Sy, fZ);//�X�N���[�����W�Ɛ[�x�l

	// �t�ϊ�
	D3DXMATRIX tmp = InvViewport * InvPrj * InvView;// ���[���h���W�����߂�iInvViewport�u�X�N���[�����W��NDC���W�v�AInvPrj�uNDC���W���r���[��ԁv�AInvView�u�r���[��ԁ����[���h���W�v)
													//���̍s��ɐ[�x�l�ƃX�N���[�����W�����킹�邱�ƂŁA�[�x�l�ɍ��킹�����W�ϊ����\�ɂȂ�)
	D3DXVec3TransformCoord(pout, &MyPos, &tmp); // ���W�ϊ���̈ʒu�����߂�
	return pout;
}

//****************************************************
// ���l���O��Ԋ֐�
//****************************************************

//====================================================
// �z��O�A�N�Z�X�����Ă��Ȃ����`�F�b�N����
//====================================================
bool Calculation::Value::IsAccessSafeForArray(int nNum, int nMax)
{
	if (nNum < 0 || nNum >= nMax)
	{
		return false;
	}
	return true;
}

//====================================================
// �������v�Z����
//====================================================
int Calculation::Value::NumDigit(int nValue)
{
	int nNumDivision = 0; // ����

	// �����v�Z����
	while (nValue > 0)
	{
		nValue /= 10;    // ���������炷
		nNumDivision++;  // �����J�E���g����
	}

	return nNumDivision;
}

//====================================================
// ���l���w��͈͓��ɐ�������
//====================================================
int Calculation::Value::LimitRange(int nValue, int nMin, int nMax)
{
	// �Œ�l����������炻�̒l�ɌŒ肷��
	if (nValue <= nMin)
	{
		nValue = nMin;
	}
	// �ő�l���������炻�̒l�ɌŒ肷��
	if (nValue >= nMax)
	{
		nValue = nMax;
	}

	return nValue;
}

//====================================================
// �����_����vector3��Ԃ�
//====================================================
D3DXVECTOR3 Calculation::Value::RandVector3(D3DXVECTOR3 Min, D3DXVECTOR3 Max)
{
	float fX = Min.x + static_cast<float>(rand()) / RAND_MAX * (Max.x - Min.x);
	float fY = Min.y + static_cast<float>(rand()) / RAND_MAX * (Max.y - Min.y);
	float fZ = Min.z + static_cast<float>(rand()) / RAND_MAX * (Max.z - Min.z);
	return D3DXVECTOR3(fX,fY,fZ);
}

//****************************************************
// �ϊ����O���
//****************************************************

//====================================================
// String��wString�ɕϊ�����
//====================================================
std::wstring Calculation::Convert::StrToWStr(std::string Str)
{
	int len = MultiByteToWideChar(CP_UTF8, 0, Str.c_str(), -1, nullptr, 0);
	std::wstring wstr(len, L'\0');
	MultiByteToWideChar(CP_UTF8, 0, Str.c_str(), -1, &wstr[0], len);
	// null ����������̂ŏ���
	if (!wstr.empty() && wstr.back() == L'\0') {
		wstr.pop_back();
	}
	return wstr;
}

//====================================================
// String��Sjis�ɕϊ�����
//====================================================
std::string Calculation::Convert::Utf8ToSjis(std::string Str)
{
	// UTF-8 �� UTF-16�i���ԁj
	int wlen = MultiByteToWideChar(CP_UTF8, 0, Str.c_str(), -1, NULL, 0);
	std::wstring wstr(wlen, L'\0');
	MultiByteToWideChar(CP_UTF8, 0, Str.c_str(), -1, &wstr[0], wlen);

	// UTF-16 �� Shift-JIS
	int slen = WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, NULL, 0, NULL, NULL);
	std::string sjisStr(slen, '\0');
	WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, &sjisStr[0], slen, NULL, NULL);

	return sjisStr;
}
