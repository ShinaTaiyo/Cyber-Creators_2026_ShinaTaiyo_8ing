//============================================================================
// 
// �Q�O�Q�T�N�U���Q���F�Q�[���p�̃v���C���[������ [GameGamePlayer.cpp]
// Author : �Ŗ����z
// 
//============================================================================

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************

// === ���̑� ===

#include "renderer.h" // �f�o�b�O�p
#include "game.h"     // �Q�[��
#include "manager.h"

// �J�����擾�p
#include "camera_manager.h"

// === �I�u�W�F�N�g ===

#include "GamePlayer.h"   // �v���C���[�i���w�b�_�j
#include "block.h"        // �u���b�N
#include "bgmodel.h"      // �w�i���f��
#include "object_X.h"     // �I�u�W�F�N�gX
#include "obstacle.h"     // ��Q��
#include "HoleObstacle.h" // ��������Q��
#include "hole.h"         // ��
#include "target.h"       // �^�[�Q�b�g
#include "particle_2D.h"  // �p�[�e�B�N��2D�i�蓁�Ȃǁj
#include "particle.h"     // �p�[�e�B�N���i�Z�[�t�]�[���ʉߎ��Ȃǁj
#include "zone.h"         // �]�[���i�ړ���\���A�Z�[�t�]�[���Ȃǁj
#include "VisionBlocker.h"// ����W�Q�I�u�W�F�N�g
#include "logger.h"       // ���O
#include "PlayerState.h"  // �v���C���[���
#include "PlayerState_BeforeThrust.h" // �v���C���[���_�˂�������O
#include "PlayerState_Returning.h" // �v���C���[��ԁQ�߂�
// === ��� ===

#include "calculation.h"       // �v�Z
#include "OBB.h"               // OBB���
#include "RotatingCylinder.h"  // ��]�~�����
#include "collision.h"         // �����蔻��X�g���e�W�[�N���X�̃R���|�[�l���g������
#include "horming_info.h"      // �z�[�~���O���i�p�[�e�B�N���Ȃǁj
#include "bounding_box.h"      // Box������
#include "transparent_info.h"  // �����x���i�^�[�Q�b�g�����x�Ȃǁj
#include "SummonGroupSelector.h" // �����O���[�v�I��
#include "bounding_cylinder.h" // Cylinder������
#include "summon_manager_state_start.h" // �����}�l�[�W���[�̏����J�n���
#include "camera_state_targetzoom.h"    // �ڕW�ʒu�ɃY�[������J�������

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
CGamePlayer::CGamePlayer() : 
	CPlayer()
{
	// �����Ȃ�
}

//============================================================================
// �f�X�g���N�^
//============================================================================
CGamePlayer::~CGamePlayer()
{
	// �����Ȃ�
}

//============================================================================
// �����ݒ�
//============================================================================
HRESULT CGamePlayer::Init()
{
	// �v���C���[�N���X�̏����ݒ�
	if (FAILED(CPlayer::Init()))
	{
		assert(false && "�v���C���[�̏����ݒ�Ɏ��s");
	}

	// �v���C���[�^�C�v���u�Q�[���v�ɐݒ�
	CPlayer::SetType(CPlayer::TYPE::GAME);

	return S_OK;
}

//============================================================================
// �I������
//============================================================================
void CGamePlayer::Uninit()
{
	// �v���C���[�N���X�̏I������
	CPlayer::Uninit();
}

//============================================================================
// �X�V����
//============================================================================
void CGamePlayer::Update()
{
	// �v���C���[�X�V
	CPlayer::Update();
}

//============================================================================
// �`�揈��
//============================================================================
void CGamePlayer::Draw()
{
	// �v���C���[�N���X�̕`�揈��
	CPlayer::Draw();
}

//============================================================================
// ����
//============================================================================
CGamePlayer* CGamePlayer::Create(D3DXVECTOR3 InitPos)
{
	// �C���X�^���X�𐶐�
	CGamePlayer* pGamePlayer = DBG_NEW CGamePlayer;

	if (pGamePlayer == nullptr)
	{ // �������s
		assert(false);
	}

	// �^�C�v��ݒ�
	pGamePlayer->SetType(TYPE::GAME);

	// �����ݒ�
	pGamePlayer->Init();

	// �ʒu�̐ݒ�
	pGamePlayer->SetPos(InitPos);

	// ���f����ݒ�
	pGamePlayer->BindModel(CX_Manager::GetInstance()->RefRegistry().BindAtKey("Player"));

	// �T�C�Y��ݒ�
	pGamePlayer->SetSize(pGamePlayer->GetModel()->Size);

	// �ő咸�_�ݒ�
	pGamePlayer->SetBaseVtxMax(pGamePlayer->GetModel()->VtxMax);

	// �ŏ����_�ݒ�
	pGamePlayer->SetBaseVtxMin(pGamePlayer->GetModel()->VtxMin);

	return pGamePlayer;
}