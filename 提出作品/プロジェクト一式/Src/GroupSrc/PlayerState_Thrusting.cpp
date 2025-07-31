//============================================================================
// 
// �Q�O�Q�T�N�V���P�O���F�����֌W�̃o�O�̉��P�ƃv���C���[����������X�e�[�g�p�^�[���ɂ��� [PlayerState_Thrusting.h]
// Author : �Ŗ����z
// 
//============================================================================

//****************************************************
// �C���N���[�h
//****************************************************

// === �I�u�W�F�N�g ===

#include "GamePlayer.h"
#include "target.h"
#include "object.h"
#include "zone.h"
#include "obstacle.h"
#include "HoleObstacle.h"
#include "hole.h"
#include "particle.h"

// === �V�[�� ===

#include "game.h" // �Q�[��

// === ��� ===

#include "PlayerState_Thrusting.h"
#include "summon_manager_state.h" // �����}�l�[�W���[���
#include "camera_state_targetzoom.h"
#include "PlayerState_Returning.h" // �v���C���[��ԁQ�߂�
#include "summon_manager_state_start.h" // �����}�l�[�W���[�����J�n���
#include "camera_state_normal.h" // �J�����ʏ���

// === ��� ===

#include "OBB.h" // OBB����
#include "bounding_box.h" // ������
#include "move_info.h"    // �ړ�
#include "horming_info.h" // �z�[�~���O

// �V�X�e��
#include "renderer.h"
#include "logger.h"
#include "calculation.h"
#include "sound.h"

//****************************************************
// using�f�B���N�e�B�u
//****************************************************
using namespace useful;

//============================================================================
// 
// public�����o
// 
//============================================================================

//============================================================================
// �R���X�g���N�^
//============================================================================
CPlayerState_Thrusting::CPlayerState_Thrusting(CPlayer* pPlayer, D3DXVECTOR3 GoalPos) :
	CPlayerState(pPlayer),
	m_GoalPos{ GoalPos },
	m_bGoalPosArrived{ false },
	m_bAvoidObstacle{ false },
	m_bCollisionObstacle{false},
	m_bNowCollision{false},
	m_pOBB{DBG_NEW COBB()},
	m_pCollisionInfo_Box{DBG_NEW CCollisionInfo_BoundingBox()}
{
	// �����Ȃ�
}

//============================================================================
// �f�X�g���N�^
//============================================================================
CPlayerState_Thrusting::~CPlayerState_Thrusting()
{
	// ��������I��
	if (m_pCollisionInfo_Box)
	{
		delete m_pCollisionInfo_Box;
		m_pCollisionInfo_Box = nullptr;
	}

	// OBB������I��
	if (m_pOBB)
	{
		delete m_pOBB;
		m_pOBB = nullptr;
	}
}

//============================================================================
// ����������
//============================================================================
void CPlayerState_Thrusting::Init(CPlayer* pPlayer)
{
	const D3DXVECTOR3&
		PlayerPos = pPlayer->GetPos(), // �v���C���[�ʒu
		PlayerRot = pPlayer->GetRot(), // �v���C���[����
		PlayerSize = pPlayer->GetSize(), // �v���C���[�T�C�Y
		PlayerVtxMax = pPlayer->GetVtxMax(), // �v���C���[�ő咸�_
		PlayerVtxMin = pPlayer->GetVtxMin(); // �v���C���[�ŏ����_

	// ���������ݒ�
	m_pCollisionInfo_Box->SetUpdateInfo(PlayerPos, VEC3_INIT, VEC3_INIT, PlayerSize, PlayerVtxMax, PlayerVtxMin, false, false, false, false);

	// OBB����ݒ�
	m_pOBB->SetPos(PlayerPos);
	m_pOBB->SetVtxMax(PlayerVtxMax);
	m_pOBB->SetVtxMin(PlayerVtxMin);
	m_pOBB->SetRot(PlayerRot);

	// �v���C���[��Ԃ����O�ŕ\��
	CLogger::GetInstance()->AddDuration(u8"�v���C���[���u�˂���ԁv�ɂȂ�܂����I", 1);
}

//============================================================================
// �I������
//============================================================================
void CPlayerState_Thrusting::Uninit(CPlayer* pPlayer)
{
	// �����Ȃ�
}

//============================================================================
// �X�V����
//============================================================================
void CPlayerState_Thrusting::Update(CPlayer* pPlayer)
{
	// === �}�C�t���[������������ϐ�

	m_bNowCollision = false; // ���ݓ����������ǂ�����������
	m_bCollisionObstacle = false; // ��Q���ɓ����������ǂ�����������

	// === �X�V�����J�n ===

	const D3DXVECTOR3& SelfPos = pPlayer->GetPos();  // ���g�̈ʒu
	D3DXVECTOR3 VecSelfToGoal = m_GoalPos - SelfPos; // ���g����ړI�n�ւ̃x�N�g��

	// �Q�[���ւ̃J�����̃|�C���^���擾����
	CCamera* pCamera = CCamera_Manager::GetInstance()->RefRegistry().BindAtKey("Game");

	// �J�����ʏ��Ԃւ̃|�C���^���擾����
	CCamera_State_Normal* pCameraState_Normal = dynamic_cast<CCamera_State_Normal*>(pCamera->GetState());

	// �w�������ŖړI�n�ֈړ�����
	pPlayer->SetPos(SelfPos + VecSelfToGoal * s_fPlayerMoveAdjust);

	// ��������X�V
	UpdateBoxInfo(pPlayer);

	// �����蔻��
	Collision(pPlayer);

	// �ړI�n�ւ̋��������ȉ��Ȃ�v���C���[�̌������̈ʒu�ɖ߂�
	if (D3DXVec3Length(&VecSelfToGoal) < s_fPlayerGoalToleranceDistance && pCameraState_Normal)
	{
		pPlayer->SetPos(m_GoalPos);
		pPlayer->SetState(DBG_NEW CPlayerState_Returning(pPlayer));
	}

	// �f�o�b�O�\��
	CRenderer::SetDebugString("�v���C���[�̏�ԁF�˂�");
}

//============================================================================
// �ړI�n�ɓ��B�������ǂ����擾
//============================================================================
const bool& CPlayerState_Thrusting::GetGoalPosArrived() const
{
	return m_bGoalPosArrived;
}

//============================================================================
// ���ݓ����������ǂ���
//============================================================================
const bool& CPlayerState_Thrusting::GetNowCollision() const
{
	return m_bNowCollision;
}

//============================================================================
// ��Q���ɓ����������ǂ���
//============================================================================
const bool& CPlayerState_Thrusting::GetCollisionObstacle() const
{
	return m_bCollisionObstacle;
}

//============================================================================
// �����蔻��
//============================================================================
void CPlayerState_Thrusting::Collision(CPlayer* pPlayer)
{
	// ���̔�����͏�ɍX�V����
	m_pCollisionInfo_Box->SetLanding(false); // �n�ʂɂ��邩�ǂ���������
	m_pCollisionInfo_Box->SetPushX(false);	 // X���ɉ����o���ꂽ���ǂ���������
	m_pCollisionInfo_Box->SetPushXOld(m_pCollisionInfo_Box->GetPushX()); // 1f�O��X���ɉ����o����Ă������ǂ����ݒ�

	// === �I�u�W�F�N�g���X�g�擪�擾�Ɣ�����̏����� ===

	CObject* pObj = CObject::GetTopObject(CObject::LAYER::DEFAULT);  // �f�t�H���g���C���[�̐擪���X�g���擾

	// === ���菈���J�n ===

	CollisionHole(pPlayer); // ��Ɍ��Ƃ̔�����J�n���A��Q����������邩�ǂ������߂�

	// �v���C���[�Ɣ��肷��I�u�W�F�N�g�̌���
	while (pObj != nullptr)
	{
		CObject* pNext = pObj->GetNext();// ���̃I�u�W�F�N�g���擾

		switch (pObj->GetType())
		{
		case CObject::TYPE::OBSTACLE:
			if (!m_bAvoidObstacle)
			{
				CollisionObstacle(pPlayer,pObj); // ��Q���Ƃ̔���J�n
			}
			break;
		case CObject::TYPE::ZONE:    // �]�[���Ƃ̔���J�n
			CollisionZone(pPlayer, pObj);
			break;
		case CObject::TYPE::HOLEOBSTACLE:
			if (!m_bAvoidObstacle)
			{
				CollisionHoleObstacle(pPlayer, pObj); // ��������Q���Ƃ̓����蔻��
			}
			break;
		default:
			break;
		}

		pObj = pNext; // ���X�g�����ɐi�߂�
	}
}

//============================================================================
// ��Q���Ƃ̓����蔻��
//============================================================================
void CPlayerState_Thrusting::CollisionObstacle(CPlayer* pPlayer, CObject* pOther)
{
	CObstacle* pObstacle = dynamic_cast<CObstacle*>(pOther); // ��Q���Ƀ_�E���L���X�g

    // ��Q���ւ̃|�C���^�����݂��Ȃ��̂ŏ������I��
	if (pObstacle == nullptr)
	{
		return;
	}

	// ��Q���̔���𔠂̔���Ƀ_�E���L���X�g
	//CCollisionInfo_BoundingBox* pOtherBox = dynamic_cast<CCollisionInfo_BoundingBox*>(pObstacle->GetCollisionInfo());
	COBB* pOtherOBB = pObstacle->GetOBB(); // ��Q���̔����OBB����Ƀ_�E���L���X�g

	// �ǂ��炩��OBB�����񂪑��݂��Ă��Ȃ������珈�����I��
	if (m_pOBB == nullptr || pOtherOBB == nullptr)
	{
		return;
	}

	// OBB������s���A����������_���[�W��Ԃɂ���
	if (collision::HitOBBToOBB(m_pOBB, pOtherOBB))
	{
		// ��eSE
		CSound::GetInstance()->Play(CSound::LABEL::HIT_0);
		CSound::GetInstance()->Play(CSound::LABEL::GUKI);

		pPlayer->SetDamage(60);
		pOther->SetRelease(); // ���������I�u�W�F�N�g�͔j�����܂�
		m_bNowCollision = true; // ���ݓ��������Ƃ����t���O���I���ɂ���
		m_bCollisionObstacle = true; // ��Q���ɓ��������Ƃ����t���O���I���ɂ���
	}

}

//============================================================================
// �]�[���Ƃ̓����蔻��
//============================================================================
void CPlayerState_Thrusting::CollisionZone(CPlayer* pPlayer, CObject* pOther)
{
	CZone* pZone = dynamic_cast<CZone*>(pOther); // ��Q���Ƀ_�E���L���X�g
	const D3DXVECTOR3& PlayerPos = pPlayer->GetPos(); // �ʒu

	// �]�[�����Ȃ����_�ŏ������I��
	if (pZone == nullptr)
	{
		return;
	}

	// �]�[���̔���𔠂̔���Ƀ_�E���L���X�g
	CCollisionInfo_BoundingBox* pOtherBox = dynamic_cast<CCollisionInfo_BoundingBox*>(pZone->GetCollisionInfo());

	// �ǂ��炩�̔������񂪑��݂��Ă��Ȃ������珈�����I��
	if (m_pCollisionInfo_Box == nullptr || pOtherBox == nullptr)
	{
		return;
	}

	// ��Vs�������o��������s��
	if (collision::HitBoxToBox(m_pCollisionInfo_Box, pOtherBox))
	{
		// �`��������
		CSound::GetInstance()->Play(CSound::LABEL::CHARIN);

		pOther->SetRelease(); // �Z�[�t�]�[����j��
		pPlayer->SetAirPower(pPlayer->GetAirPower() + s_nHitSafeZoneToAddPower); // �C�𑝂₷
		m_bNowCollision = true; // ���ݓ��������Ƃ����t���O���I���ɂ���

		// ����Ƀp�[�e�B�N�����΂�
		for (int nCnt = 0; nCnt < s_nZoneParticleSummonNum; nCnt++)
		{
			// �P�ʋ���̃����_�������𐶐���rand()��������RANDMAX�܂ł̗������Ԃ����̂ŁA���̒l��RANDMAX�Ŋ��萳�K�������̒l�ɉ~�����|����
			float theta = static_cast<float>(rand()) / RAND_MAX * D3DX_PI * 2.0f; // Yaw�i0�`2�΁j
			float z = static_cast<float>(rand()) / RAND_MAX * 2.0f - 1.0f;        // cos(Pitch)�Ɠ����i-1�`1�j
			float r = sqrtf(1.0f - z * z);  // �R�����x�N�g���̎�����iX2��{Y�Q��{Z�Q��j�@���@�P
											// XZ���ʂ̔��a��r�Ƃ���ƁAr2�� + z�Q�� = 1�̂Q�� = 1
											// �Ȃ̂� r = ��(1 - z�Q��)

			// �����x�N�g��
			D3DXVECTOR3 dir(
				r * cosf(theta),
				z,
				r * sinf(theta)
			);

			// �C�ӂ̃X�P�[���ő��x�𒲐�
			dir *= s_fZoneParticleSpeed;

			// �p�[�e�B�N������
			CParticle* pParticle = CParticle::Create(pZone->GetPos(), D3DXVECTOR3(5.0f, 5.0f, 0.0f), D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f), 600, 600);
			CMove_Info* pMove_Info = pParticle->GetMoveInfo(); // �ړ����ւ̃|�C���^
			CHorming_Info* pHorming_Info = pParticle->GetHormingInfo(); // �z�[�~���O���ւ̃|�C���^

			// �ړ����ݒ�
			pMove_Info->SetMove(dir);        // �ړ��ʐݒ�
			pMove_Info->SetInertia(0.001f);    // ����������
			pMove_Info->SetUseInertia(true); // �������g�p����

			// �z�[�~���O���ݒ�
			pHorming_Info->SetYaw(Calculation::Rot::Yaw(PlayerPos, PlayerPos + dir));     // ���݂̈ʒu��1f��̈ʒu����Yaw�����߂�
			pHorming_Info->SetPitch(Calculation::Rot::Pitch(PlayerPos, PlayerPos + dir)); // ���݂̈ʒu��1f��̈ʒu����Pitch�����߂�
			pHorming_Info->SetUseHorming(true);  // �z�[�~���O���g�p����
			pHorming_Info->SetAdjust(0.5f);      // �␳�l
			pHorming_Info->SetSpeed(1.0f);       // ���x
			pHorming_Info->SetGoalPos(PlayerPos); // �ڕW�ʒu
			pHorming_Info->SetAddSpeed(0.2f);    // ���Z���x�ݒ�
			pHorming_Info->SetAddAdjust(0.01f); // ���Z�␳�l�ݒ�
			pHorming_Info->SetLockOnType(CHorming_Info::LOCKONTYPE::PLAYER); // �v���C���[��_��

			// ��Q���̊g�U
			SpreadObstacle(pPlayer);
		}
		CRenderer::SetDebugString("�Z�[�t�I");   // �Z�[�t�]�[���ʉߎ��̌��ʂ��f�o�b�O�\��
	}
}

//============================================================================
// ���Ƃ̓����蔻��
//============================================================================
void CPlayerState_Thrusting::CollisionHole(CPlayer* pPlayer)
{
	CObject* pObj = CObject::GetTopObject(CObject::LAYER::DEFAULT);  // �f�t�H���g���C���[�̐擪���X�g���擾

    // �v���C���[�Ɣ��肷��I�u�W�F�N�g�̌���
	while (pObj != nullptr)
	{
		CObject* pNext = pObj->GetNext(); // ���X�g�̎��̃|�C���^�擾

		// �^�C�v���u�������I�u�W�F�N�g�v�ȊO�Ȃ烊�X�g�����ɐi�߂�
		if (pObj->GetType() != CObject::TYPE::HOLEOBSTACLE)
		{
			pObj = pNext;
			continue;
		}

		CHoleObstacle* pHoleObstacle = dynamic_cast<CHoleObstacle*>(pObj); // ��������Q���Ƀ_�E���L���X�g

		// ��������Q���ւ̃|�C���^�����݂��Ȃ��̂ŏ������I��
		if (pHoleObstacle == nullptr)
		{
			return;
		}

		// ��������Q���̌��̉�]�~�����ւ̃|�C���^
		CRotatingCylinder* pOtherRotCylinder = pHoleObstacle->GetHole()->GetRotatingCylinder();

		// �擾���������񂪑��݂��Ȃ������珈�����I��
		if (pOtherRotCylinder == nullptr)
		{
			return;
		}

		// ���[���h��Ԃ��l��������]�~���Ƃ̓����蔻����s���A
		// ��x���ɓ������炻�̃t�F�[�Y�͂����Ə�Q���ɓ�����Ȃ����Ƃɂ���
		if (!m_bAvoidObstacle)
		{
			m_bAvoidObstacle = collision::HitAABBToIsWorldRotatingCylinder_Sweat(m_pCollisionInfo_Box, pOtherRotCylinder,
				pHoleObstacle->GetPos(), pHoleObstacle->GetRot());
		}
		if (m_bAvoidObstacle)
		{
			CRenderer::SetDebugString("���̒��Ȃ̂œ������Ă��Ȃ��I");
			return;
		}

		// ���X�g�����ɐi�߂�
		pObj = pNext;
	}
}

//============================================================================
// ��������Q���Ƃ̓����蔻��
//============================================================================
void CPlayerState_Thrusting::CollisionHoleObstacle(CPlayer* pPlayer, CObject* pOther)
{
	CHoleObstacle* pHoleObstacle = dynamic_cast<CHoleObstacle*>(pOther); // ��������Q���Ƀ_�E���L���X�g

    // ��������Q���ւ̃|�C���^�����݂��Ȃ��̂ŏ������I��
	if (pHoleObstacle == nullptr)
	{
		return;
	}

	// ��������Q����OBB������ւ̃|�C���^
	COBB* pOtherOBB = pHoleObstacle->GetOBB();

	// �擾���������񂪑��݂��Ȃ������珈�����I��
	if (pOtherOBB == nullptr)
	{
		return;
	}

	// ����̃t���[���Ő��������ꍇ�A���̌������I�u�W�F�N�g�̌��ȊO�ɂ�������Ȃ��̂Ō�������Q���Ƃ͓�����Ȃ�
	// OBB���m�̔�����s��
	if (collision::HitOBBToOBB(m_pOBB, pOtherOBB))
	{
		m_bNowCollision = true; // ���ݓ��������Ƃ����t���O���I���ɂ���
		m_bCollisionObstacle = true; // ��Q���ɓ��������Ƃ����t���O���I���ɂ���
		CRenderer::SetDebugString("�������Ă���I");
		pPlayer->SetDamage(60);
		pOther->SetRelease(); // ���������I�u�W�F�N�g�͔j�����܂�
	}
}

//============================================================================
// �{�b�N�X������X�V
//============================================================================
void CPlayerState_Thrusting::UpdateBoxInfo(CPlayer* pPlayer)
{
	if (m_pCollisionInfo_Box != nullptr)
	{
		const D3DXVECTOR3& SelfPos = pPlayer->GetPos();   // ���g�̈ʒu
		const D3DXVECTOR3& Rot = pPlayer->GetRot();       // ����
		const D3DXVECTOR3& Size = pPlayer->GetSize();     // �T�C�Y
		const D3DXVECTOR3& VtxMax = pPlayer->GetVtxMax(); // �ő咸�_
		const D3DXVECTOR3& VtxMin = pPlayer->GetVtxMin(); // �ŏ����_

		const D3DXVECTOR3& BoxPosOld = m_pCollisionInfo_Box->GetPosOld();  // �������1f�O�̈ʒu
		const bool& bIsLanding = m_pCollisionInfo_Box->GetLanding();       // ������̒n�ʂɂ��邩�ǂ���
		const bool& bIsLandingOld = m_pCollisionInfo_Box->GetLandingOld(); // 1f�O�ɒn�ʂɂ��邩�ǂ���
		const bool& bPushX = m_pCollisionInfo_Box->GetPushX();             // X���ɉ����o����Ă������ǂ���
		const bool& bPushXOld = m_pCollisionInfo_Box->GetPushXOld();       // 1f�O��X���ɉ����o����Ă������ǂ���
		m_pCollisionInfo_Box->SetPosOld(SelfPos);                          // �������1f�O�̈ʒu���X�V
		m_pCollisionInfo_Box->SetLandingOld(bIsLanding);                   // 1f�O�ɒn�ʂɂ��邩�ǂ����ݒ�

		// ��������X�V�i�Ȃ������ɏ������H���ʒu�̍X�V�������Ȃ̂�)
		m_pCollisionInfo_Box->SetUpdateInfo(SelfPos, BoxPosOld, VEC3_INIT, Size, VtxMax, VtxMin, bIsLanding, bIsLandingOld, bPushX, bPushXOld);

		// OBB������X�V
		m_pOBB->SetPos(SelfPos);
		m_pOBB->SetVtxMax(VtxMax);
		m_pOBB->SetVtxMin(VtxMin);
		m_pOBB->SetRot(Rot);
	}
}

//============================================================================
// ��Q���𐁂���΂�
//============================================================================
void CPlayerState_Thrusting::SpreadObstacle(CPlayer* pPlayer)
{
	// �f�t�H���g���C���[�̐擪���X�g���擾
	CObject* pObj = CObject::GetTopObject(CObject::LAYER::DEFAULT);

	// ���̎��_�ő��݂����Q���^�C�v�̃I�u�W�F�N�g�S�ĂɊg�U�����𐶐�
	while (pObj != nullptr)
	{
		// ��Q���^�C�v�̃I�u�W�F�N�g�Ȃ�
		if (pObj->GetType() == CObject::TYPE::OBSTACLE)
		{
			CObstacle* pObstacle = dynamic_cast<CObstacle*>(pObj);

			// �_�E���L���X�g������
			if (pObstacle)
			{
				// �i�s���x���쐬
				float fRate = 0.1f;

				// ������ѕ������쐬
				Vec3 Dir = pObstacle->GetPos() - pPlayer->GetPos();

				// �g�U����������
				pObstacle->ComposeSpreader([fRate, Dir]()
					{
						up<CObstacle::CSpreader> up = make_unique<CObstacle::CSpreader>();
						up->SetRate(fRate);
						up->SetDir(Dir);
						return up;
					}
				);
			}
		}

		pObj = pObj->GetNext();
	}
}