//============================================================================
// 
// �Q�O�Q�T�N�V���P�O���F�����֌W�̃o�O�̉��P�ƃv���C���[����������X�e�[�g�p�^�[���ɂ��� [PlayerState_GuideBeforeThrust.h]
// Author : �Ŗ����z
// 
//============================================================================

//****************************************************
// �Q�d�C���N���[�h�h�~
//****************************************************
#ifndef _PLAYERSTATE_GUIDEBEFORETHRUST_H_
#define _PLAYERSTATE_GUIDEBEFORETHRUST_H_	

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "PlayerState_BeforeThrust.h"
#include "Vec3LerpInfo.h"

//****************************************************
// �v���C���[�˂��O�K�C�h�N���X
//****************************************************
class CPlayerState_GuideBeforeThrust : public CPlayerState_BeforeThrust
{
public:
	// === ����֐� ===

	CPlayerState_GuideBeforeThrust(
		CPlayer* pPlayer,       // �v���C���[�ւ̃|�C���^
		int nAnticipationFrame // ���؂�t���[����
	); // �R���X�g���N�^
	~CPlayerState_GuideBeforeThrust() override; // �f�X�g���N�^

	// === ���C�t�T�C�N�������o�֐� ===

	void Init(CPlayer* pPlayer) override;   // ����������
	void Uninit(CPlayer* pPlayer) override; // �I������
	void Update(CPlayer* pPlayer) override; // �X�V����
private:
	// === �ÓI�����o�֐� ===
	static constexpr float
		s_fSummonObjectSlowRange = 20.0f, // �������ꂽ�I�u�W�F�N�g���X���[��Ԃɂ���͈�
		s_fSummonObjLerpLength = 20.0f; // �������ꂽ�I�u�W�F�N�g�����[�v�������苗��

	// === �����o�ϐ� ===

	D3DXVECTOR3 m_GoalPos; // �ړI�n
	JSON m_JsonMotion; // ���[�V�����W�F�C�\���t�@�C��
	int m_nNowMotion; // ���݂̃��[�V�����ԍ�
	int m_nMotionFrame; // ���[�V�����t���[����
	bool m_bNowEndMotionAfterThrust; // ���݂̃��[�V�������I�������˂��𔭓����邩�ǂ���
	CVec3LerpInfo m_Vec3LerpInfo; // 3�������`��ԏ��

	// === �����o�֐� ===

	void LerpProcess(CPlayer * pPlayer); // ���`��ԏ���
	bool ControlDecisionGoal() override; // �ړI�n�����߂鑀��
	bool HandSword(CPlayer* pPlayer) override; // �蓁
	bool NextMotion(CPlayer * pPlayer);  // ���[�V���������ɐi�߂�
};
#endif // _COLLISION_H_