//============================================================================
// 
// �Q�O�Q�T�N�V���P�O���F�����֌W�̃o�O�̉��P�ƃv���C���[����������X�e�[�g�p�^�[���ɂ��� [PlayerState_BeforeThrust.h]
// Author : �Ŗ����z
// 
//============================================================================

//****************************************************
// �Q�d�C���N���[�h�h�~
//****************************************************
#ifndef _PLAYERSTATE_BEFORETHRUST_H_
#define _PLAYERSTATE_BEFORETHRUST_H_	

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "PlayerState.h"

//****************************************************
// �v���C���[��ԃN���X
//****************************************************
class CPlayerState_BeforeThrust : public CPlayerState
{
public:
	// === ����֐� ===

	CPlayerState_BeforeThrust(CPlayer* pPlayer,int nAnticipationFrame); // �R���X�g���N�^
    ~CPlayerState_BeforeThrust() override; // �f�X�g���N�^

	// === ���C�t�T�C�N�������o�֐� ===
	void Init(CPlayer* pPlayer) override;   // ����������
	void Uninit(CPlayer* pPlayer) override; // �I������
	void Update(CPlayer* pPlayer) override; // �X�V����

	// === �����o�֐� ===

	// ���؂�t���[����
	const int& GetAnticipationFrame() const; // �擾
	void SetAnticipationFrame(int nFrame);   // �ݒ�

	// === �ÓI�����o�֐� ===

	static const int& GetHandSwordCost(); // �C�̏���ʂ��擾
	static const int& GetNormalAnticipationFrame(); // �ʏ팩�؂�t���[�����擾
protected:
	// === �����o�֐� ===

	void DecisionGoal(CPlayer* pPlayer); // �ړI�n���菈��
private:
	// === �ÓI�����o�ϐ� ===
	static constexpr float
		s_fCameraZoom_PosAdjust = 0.25f, // �J�����̃Y�[�����J�n���鎞�̈ʒu�␳�l
		s_fCameraZoom_DistanceAdjust = 0.12f, // �J�����̃Y�[�����J�n���鎞�̋����␳�l
		s_fAddSummonObjectSlowStartPos_Z = 10.0f, // �^�[�Q�b�g���ǂꂭ�炢�O����ɃX���[���J�n���邩
		s_fSummonObjectSlowRange = 20.0f, // �������ꂽ�I�u�W�F�N�g���X���[��Ԃɂ���͈�
		s_fSummonObjLerpLength = 20.0f, // �������ꂽ�I�u�W�F�N�g�����[�v�������苗��
		s_fPlayerMoveAdjust = 0.5f; // �v���C���[�ړ��ʕ␳�l
	static constexpr int
		s_nNormalAnticipationFrame = 90, // �ʏ팩�؂�t���[����
		s_nHandSwordCost = 5; // �蓁���ɏ����C�̃R�X�g

	// === �����o�ϐ� ===

	int m_nAnticipationFrame; // ���؂�t���[����
	bool
		m_bStartAnticipation; // ���؂�J�n

	// === �����o�֐� ===

	virtual bool HandSword(CPlayer * pPlayer); // �蓁����
	void FixedPos(CPlayer* pPlayer);   // �ʒu�Œ菈��
	void JudgeLerpPosToSummonObj(CPlayer* pPlayer); // �����I�u�W�F�N�g���`��ԏ��������邩�ǂ������f����
	void SetLerpPosToSummonObj(CPlayer* pPlayer);   // �����I�u�W�F�N�g���`��ԏ�����ݒ肷��
	void ClearLerpPosToSummonObj();       // �����I�u�W�F�N�g���`��ԏ�������������
	virtual bool ControlDecisionGoal();    // �ړI�n�����߂鑀��
	void StopObstacleAddRot(); // ��Q���̌����̉��Z���~�߂�
	void SummonObjSlow(int nSlowFrame);        // �����I�u�W�F�N�g���X���[��Ԃɂ���
	void SummonObjClearSlow(); // �X�e�[�W�̃I�u�W�F�N�g�̃X���[��Ԃ�����
};
#endif // _COLLISION_H_