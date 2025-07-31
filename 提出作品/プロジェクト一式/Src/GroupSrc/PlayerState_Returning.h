//============================================================================
// 
// �Q�O�Q�T�N�V���P�O���F�����֌W�̃o�O�̉��P�ƃv���C���[����������X�e�[�g�p�^�[���ɂ��� [PlayerState_Returning.h]
// Author : �Ŗ����z
// 
//============================================================================

//****************************************************
// �Q�d�C���N���[�h�h�~
//****************************************************
#ifndef _PLAYERSTATE_RETURNING_H_
#define _PLAYERSTATE_RETURNING_H_	

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "PlayerState.h"

//****************************************************
// �v���C���[��ԃN���X
//****************************************************
class CPlayerState_Returning : public CPlayerState
{
public:
	// === ����֐� ===

	CPlayerState_Returning(CPlayer* pPlayer); // �R���X�g���N�^
	~CPlayerState_Returning() override; // �f�X�g���N�^

	// === ���C�t�T�C�N�������o�֐� ===

	void Init(CPlayer* pPlayer) override;   // ����������
	void Uninit(CPlayer* pPlayer) override; // �I������
	void Update(CPlayer* pPlayer) override; // �X�V����

	// === �����o�֐� ===

	// ���̈ʒu�֖߂�̂������������ǂ���
	const bool& GetIsReturnComplete() const; // �擾

private:
	// === �ÓI�����o�ϐ� ===
	static constexpr float
		s_fCameraZoom_PosAdjust = 0.15f, // �J�����̃Y�[�����J�n���鎞�̈ʒu�␳�l
		s_fCameraZoom_DistanceAdjust = 0.05f, // �J�����̃Y�[�����J�n���鎞�̋����␳�l
		s_fPlayerMoveAdjust = 0.18f, // �v���C���[�ړ��ʕ␳�l
		s_fPlayerGoalToleranceDistance = 0.1f; // �v���C���[���ړI�n�ɓ��B�����Ƃ݂Ȃ����e����

	// === �����o�ϐ� ===
	D3DXVECTOR3 m_GoalPos; // �ړI�n
	bool m_bIsReturnComplete; // ���̈ʒu�ɖ߂�̂������������ǂ���

};
#endif // _COLLISION_H_