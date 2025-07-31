//============================================================================
// 
// �Q�O�Q�T�N�V���P�O���F�����֌W�̃o�O�̉��P�ƃv���C���[����������X�e�[�g�p�^�[���ɂ��� [PlayerState_HandSwordPractice.h]
// Author : �Ŗ����z
// 
//============================================================================

//****************************************************
// �Q�d�C���N���[�h�h�~
//****************************************************
#ifndef _PLAYERSTATE_HandSwordPractice_H_
#define _PLAYERSTATE_HandSwordPractice_H_	

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "PlayerState_BeforeThrust.h"
#include "Vec3LerpInfo.h"

//****************************************************
// �O���錾
//****************************************************
class CButton;

//****************************************************
// �v���C���[�˂��O�K�C�h�N���X
//****************************************************
class CPlayerState_HandSwordPractice : public CPlayerState_BeforeThrust
{
public:
	// === ����֐� ===

	CPlayerState_HandSwordPractice(
		CPlayer* pPlayer,       // �v���C���[�ւ̃|�C���^
		int nAnticipationFrame // ���؂�t���[����
	); // �R���X�g���N�^
	~CPlayerState_HandSwordPractice() override; // �f�X�g���N�^

	// === ���C�t�T�C�N�������o�֐� ===

	void Init(CPlayer* pPlayer) override;   // ����������
	void Uninit(CPlayer* pPlayer) override; // �I������
	void Update(CPlayer* pPlayer) override; // �X�V����

	// === �����o�֐� ===
	static bool GetEndPractice(); // ���K���I��������ǂ����擾

private:
	// === �ÓI�����o�ϐ� ===
	
	static bool s_bEndPractice; // ���K���I���������ǂ���

	// === �����o�֐� ===

	bool ControlDecisionGoal() override; // �ړI�n�����߂鑀��
};
#endif // _COLLISION_H_