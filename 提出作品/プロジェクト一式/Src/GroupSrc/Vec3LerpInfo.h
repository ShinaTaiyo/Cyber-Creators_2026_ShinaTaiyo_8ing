//============================================================================
// 
// �Q�O�Q�T�N�V���P�R���F���`��ԃN���X����� [Vec3LerpInfo.h]
// Author : �Ŗ����z
// 
//============================================================================

#ifndef _LERPSCALE_INFO_H_
#define _LERPSCALE_INFO_H_	// ��d�C���N���[�h�h�~

//****************************************************
// �C���N���[�h
//****************************************************
#include "object.h"

//****************************************************
// �R�����x�N�g�����`��ԃN���X
//****************************************************
class CVec3LerpInfo
{
public:
	// === ���ʊ֐� ===
	CVec3LerpInfo();  // �R���X�g���N�^
	~CVec3LerpInfo(); // �f�X�g���N�^

	// === ���C�t�T�C�N�������o�֐� ===

	void Update(D3DXVECTOR3& Vec, CObject* pObj); // �X�V����

	// === �����o�֐� ===

	// �������֐�
	void SetInitialize(int nDuration, D3DXVECTOR3 StartVec, D3DXVECTOR3 GoalVec);

	// �g�p���邩�ǂ���
	const bool& GetUse() const; // �擾
	void SetUse(bool bUse); // �ݒ�

private:
	// === �ÓI�����o�ϐ� ===
	static constexpr int s_nNormalLerpDuration = 5; // �ʏ���`��ԓ��B����

	// === �����o�ϐ� ===

	int m_nLerpDuration; // ���`��ԓ��B����
	int m_nCntDuration; // ���B���ԃJ�E���g�p
	bool m_bUse; // �g�p���邩�ǂ���
	D3DXVECTOR3 m_StartVec; // �J�n�g�嗦
	D3DXVECTOR3 m_GoalVec; // �ڕW�g�嗦
};
#endif
