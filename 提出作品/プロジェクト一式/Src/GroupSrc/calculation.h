//============================================================================
// 
// �Q�O�Q�T�N�T���P�X���A�v�Z�p�֐� [calculation.h]
// Author : ���c����
// 
//============================================================================

#ifndef _CALCULATION_H_
#define _CALCULATION_H_	// ��d�C���N���[�h�h�~

//****************************************************
// ���O���Calculation���`
//****************************************************
namespace Calculation
{
	//****************************************************
	// using�f�B���N�e�B�u
	//****************************************************
	using namespace useful;

	// �������O���
	namespace Length
	{
		// === �֐� ===

	    float PointToPoint(D3DXVECTOR3 SelfPos, D3DXVECTOR3 OtherPos); // �_�Ɠ_�̋��������
		float PointToPoint_1D(float fSelfPos, float fOtherPos); // �P�����̋��������
		float PointtoPoint_2D(D3DXVECTOR2 SelfPos, D3DXVECTOR3 OtherPos); // �Q�����̋��������
	}

	// �p�x���O���
	namespace Rot
	{
		// === �֐� ===

		void Adjust(float & fSelfRot); // �p�x�𒲐�
		float Yaw(D3DXVECTOR3 SelfPos, D3DXVECTOR3 OtherPos);   // Yaw�����߂�
		float Pitch(D3DXVECTOR3 SelfPos, D3DXVECTOR3 OtherPos); // Pitch�����߂�
		float AdjustAngleDifference(float fSelfRot, float fOtherRot); // �p�x���𒲐�
	}

	// ���l
	namespace Value
	{
		// === �֐� ===

		bool IsAccessSafeForArray(int nNum,int nMax); // �z��O�A�N�Z�X�����Ă��Ȃ����`�F�b�N����
		int NumDigit(int nValue); // �������v�Z����
		int LimitRange(int nValue, int nMin, int nMax); // ���l���w��͈͓��ɐ�������
		D3DXVECTOR3 RandVector3(D3DXVECTOR3 Min, D3DXVECTOR3 Max); // �����_����vector3��Ԃ�
	}

	namespace Convert
	{
		std::wstring StrToWStr(std::string Str);
		std::string Utf8ToSjis(std::string Str);
	}

	// �ʒu���O���
	namespace Pos
	{
		// === �֐� ===

		// ���[���h���W���X�N���[�����W�ɕϊ�
		D3DXVECTOR3 WorldToScreen(
			D3DXVECTOR3 worldPos, // ���[���h���W
			D3DXMATRIX viewMatrix, // �r���[�}�g���b�N�X
			D3DXMATRIX projectionMatrix, // �v���W�F�N�V�����}�g���b�N�X
			float screenWidth, // �X�N���[������
			float screenHeight // �X�N���[������
		);

		// �X�N���[�����W�����[���h���W�ɕϊ�
		D3DXVECTOR3* ScreenToWorld(D3DXVECTOR3* pout,
			float Sx,  // �X�N���[��X���W
			float Sy,  // �X�N���[��Y���W
			float fZ,  // �ˉe��Ԃł�Z�l�i0�`1�j
			int Screen_w, // �X�N���[������
			int Screen_h, // �X�N���[������
			D3DXMATRIX* View, // �r���[�}�g���b�N�X
			D3DXMATRIX* Prj   // �v���W�F�N�V�����}�g���b�N�X
		);

	}
};
#endif