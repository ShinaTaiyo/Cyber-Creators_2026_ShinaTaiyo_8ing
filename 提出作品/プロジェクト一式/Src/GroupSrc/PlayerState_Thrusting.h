//============================================================================
// 
// �Q�O�Q�T�N�V���P�O���F�����֌W�̃o�O�̉��P�ƃv���C���[����������X�e�[�g�p�^�[���ɂ��� [PlayerState_Thrusting.h]
// Author : �Ŗ����z
// 
//============================================================================

//****************************************************
// �Q�d�C���N���[�h�h�~
//****************************************************
#ifndef _PLAYERSTATE_THRUSTING_H_
#define _PLAYERSTATE_THRUSTING_H_	

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "PlayerState.h"

//****************************************************
// �ÓI�����o�錾
//****************************************************
class CCollisionInfo_BoundingBox; // ��������
class COBB; // OBB������

//****************************************************
// �v���C���[��ԃN���X
//****************************************************
class CPlayerState_Thrusting : public CPlayerState
{
public:
	// === ����֐� ===

	CPlayerState_Thrusting(CPlayer* pPlayer,D3DXVECTOR3 GoalPos); // �R���X�g���N�^
	~CPlayerState_Thrusting() override; // �f�X�g���N�^

	// === ���C�t�T�C�N�������o�֐� ===

	void Init(CPlayer* pPlayer) override;   // ����������
	void Uninit(CPlayer* pPlayer) override; // �I������
	void Update(CPlayer* pPlayer) override; // �X�V����

	// === �����o�֐� ===

	// �ړI�n�ɓ��B�������ǂ���
	const bool& GetGoalPosArrived() const; // �擾

	// ���݂������������ǂ���
	const bool& GetNowCollision() const; // �擾

	// ��Q���ɓ����������ǂ���
	const bool& GetCollisionObstacle() const; // �擾

private:
	// === �ÓI�����o�ϐ� ===

	static constexpr float
		s_fCameraZoom_PosAdjust = 0.15f,       // �J�����̃Y�[�����J�n���鎞�̈ʒu�␳�l
		s_fCameraZoom_DistanceAdjust = 0.05f,  // �J�����̃Y�[�����J�n���鎞�̋����␳�l
		s_fPlayerMoveAdjust = 0.18f,           // �v���C���[�ړ��ʕ␳�l
		s_fZoneParticleSpeed = 5.0f, // �]�[����ʉ߂������̃p�[�e�B�N�����x
		s_fPlayerGoalToleranceDistance = 0.5f; // �v���C���[���ړI�n�ɓ��B�����Ƃ݂Ȃ����e����
	static constexpr int 
	    s_nHitSafeZoneToAddPower = 5,  // �Z�[�t�]�[���ʉߎ��̋C�𑝂₷��
		s_nZoneParticleSummonNum = 50; // �]�[���ʉߎ��̃p�[�e�B�N��������

	// === �����o�ϐ� ===

	D3DXVECTOR3 m_GoalPos;  // �ڕW�ʒu
	bool m_bGoalPosArrived; // �ڕW�ʒu�ɓ��B�������ǂ���
	bool m_bAvoidObstacle;  // ��Q����������邩�ǂ���
	bool m_bNowCollision; // ���ݓ����������ǂ���
	bool m_bCollisionObstacle; // ��Q���ɓ����������ǂ���

	CCollisionInfo_BoundingBox* m_pCollisionInfo_Box; // ��������
	COBB* m_pOBB; // OBB���

	// === �����o�֐� ===
	void Collision(CPlayer * pPlayer); // �����蔻��
	void CollisionObstacle(CPlayer* pPlayer,CObject* pOther);      // ��Q���Ƃ̓����蔻��
	void CollisionZone(CPlayer* pPlayer, CObject* pOther);         // �]�[���Ƃ̓����蔻�� 
	void CollisionHole(CPlayer* pPlayer);         // ���Ƃ̓����蔻��
	void CollisionHoleObstacle(CPlayer* pPlayer, CObject* pOther); // ��������Q���Ƃ̓����蔻��
	void UpdateBoxInfo(CPlayer* pPlayer); // �{�b�N�X������X�V
	void SpreadObstacle(CPlayer * pPlayer);	// ��Q���𐁂���΂�

};
#endif // _COLLISION_H_
