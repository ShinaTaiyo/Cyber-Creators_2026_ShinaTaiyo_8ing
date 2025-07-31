//============================================================================
// 
// �Q�O�Q�T�N�V���X���F�R�����x�N�g���w�������N���X����� [Vec3ExponertialInfo.h]
// Author : �Ŗ����z
// 
//============================================================================

#ifndef _VEC3EXPONERTIAL_INFO_H_
#define _VEC3EXPONERTIAL_INFO_H_	// ��d�C���N���[�h�h�~

//****************************************************
// �C���N���[�h
//****************************************************
#include "object.h"

//****************************************************
// �R�����x�N�g���w�������N���X
//****************************************************
class CVec3ExponertialInfo
{
public:
	// === ���ʊ֐� ===

	CVec3ExponertialInfo();  // �R���X�g���N�^
	~CVec3ExponertialInfo(); // �f�X�g���N�^

	// === ���C�t�T�C�N�������o�֐� ===

    void Update(D3DXVECTOR3& Self,CObject * pObj); // �X�V����

	// === �����o�֐� ===

	// �����ݒ�
	void SetInitialize(
		float fExpDecayRate, // �w�������{��
		D3DXVECTOR3 Target   // �ڕW�l
	);

	// �ڕW�l
	const D3DXVECTOR3& GetTarget() const; // �擾
	void SetTarget(D3DXVECTOR3 Target); // �ݒ�

	// �g�p���邩�ǂ���
	const bool& GetUse() const; // �擾
	void Setuse(bool bUse); // �ݒ�
		 
private:
	// === �ÓI�����o�ϐ� ===
	static constexpr float s_fNormalExpDecayRate = 0.5f; // �ʏ�w�������{��
	static constexpr float s_fCompleteDistance = 0.01f; // �����������Ƃ��鋗��

	// === �����o�ϐ� ===
	float m_fExpDecayRate; // �w�������{��
	bool m_bUse; // �g�p���邩�ǂ���
	D3DXVECTOR3 m_Target; // �ڕW�l
};
#endif
