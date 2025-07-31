//============================================================================
// 
// �Q�O�Q�T�N�S���Q�W���F�u���b�N�N���X���쐬 [Target.cpp]
// Author : �Ŗ����z
// 
//============================================================================

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "target.h"           // ���w�b�_
#include "transparent_info.h" // �����x���

// ���
#include "calculation.h" // �v�Z

// ���͗p
#include "manager.h" // �}�l�[�W���[

// �J�����擾�p
#include "camera_manager.h"

// �f�o�b�O�\���p
#include "renderer.h"

// �e�N�X�`���擾�p
#include "texture_manager.h"

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
CTarget::CTarget() :
	CObject_3D{ LAYER::UI },
	m_bControl{ true },
	m_Move{ useful::VEC3_INIT },
	m_pTransParent_Info{ DBG_NEW CTransparent_Info(60,60,false,1) },
	m_PosTarget{GetPos()},
	m_PosTargetOld{GetPos()},
	m_bUseMove{true},
	m_bUseAdjustCameraPos{true}
{
	SetType(CObject::TYPE::TARGET); // �^�C�v���u�^�[�Q�b�g�v�ɐݒ�
}

//============================================================================
// �f�X�g���N�^
//============================================================================
CTarget::~CTarget()
{
	// �����Ȃ�
}

//============================================================================
// �����ݒ�
//============================================================================
HRESULT CTarget::Init()
{
	// �I�u�W�F�N�g3D�̏����ݒ�
	if (FAILED(CObject_3D::Init()))
	{
		assert(false && "�v���C���[�̏����ݒ�Ɏ��s");
	}
     
	return S_OK;
}

//============================================================================
// �I������
//============================================================================
void CTarget::Uninit()
{
	// �����x���I��
	if (m_pTransParent_Info != nullptr)
	{
		delete m_pTransParent_Info;    // �j������
		m_pTransParent_Info = nullptr; // �|�C���^������
	}

	// �I�u�W�F�N�g3D�̏I������
	CObject_3D::Uninit();
}

//============================================================================
// �X�V����
//============================================================================
void CTarget::Update()
{
	D3DXCOLOR Col = GetCol(); // �F����

	// ������
	Control(); 

	// �I�u�W�F�N�g3D�̍X�V
	CObject_3D::Update();

	// �����x��񂪑��݂���Ȃ瓧���x�𑀍�
	if (m_pTransParent_Info != nullptr)
	{
		m_pTransParent_Info->Update(Col);
	}

	AdjustCameraPos(); // �J�����̈ʒu�𒲐�����

	SetCol(Col); // �F�����ݒ�
}

//============================================================================
// �`�揈��
//============================================================================
void CTarget::Draw()
{
	// �f�o�C�X���擾
	LPDIRECT3DDEVICE9 pDev = CRenderer::GetDeviece();

	//Z�o�b�t�@�ɏ������܂Ȃ�
	pDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	// �[�x�𖳎����ĕ`�揇�ɂ��܂�
	pDev->SetRenderState(D3DRS_ZENABLE, FALSE);

	// �I�u�W�F�N�g3D�̕`�揈��
	CObject_3D::Draw();

	//Z�o�b�t�@�ɏ�������
	pDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	// �[�x�𖳎����ĕ`�揇�ɂ��܂�
	pDev->SetRenderState(D3DRS_ZENABLE, TRUE);

}

//============================================================================
// ���삷�邩�ǂ����擾
//============================================================================
const bool& CTarget::GetUseControl() const
{
	return m_bControl;
}

//============================================================================
// ���삷�邩�ǂ����ݒ�
//============================================================================
void CTarget::SetUseControl(bool bControl)
{
	m_bControl = bControl;
}

//============================================================================
// ����
//============================================================================
CTarget* CTarget::Create(D3DXVECTOR3 InitPos)
{
	// �C���X�^���X�𐶐�
	CTarget* pTarget = DBG_NEW CTarget;

	// �������s
	if (pTarget == nullptr)
	{
		assert(false);
	}

	// �^�C�v��ݒ�
	pTarget->SetType(TYPE::TARGET);

	// �e�N�X�`���ݒ�
	pTarget->BindTex(CTexture_Manager::GetInstance()->RefRegistry().BindAtKey("Target_000"));

	// �����ݒ�
	pTarget->Init();

	// �����ʒu�̐ݒ�
	pTarget->SetPos(InitPos);

	return pTarget;
}

//============================================================================
// �����x�����擾
//============================================================================
CTransparent_Info* CTarget::GetTransparent_Info() const
{
	return m_pTransParent_Info;
}

//============================================================================
// �ړI�n��ݒ�
//============================================================================
void CTarget::SetPosTarget(D3DXVECTOR3 Pos)
{
	m_PosTarget = Pos;
}

//============================================================================
// �ړ������邩�ǂ����擾
//============================================================================
const bool& CTarget::GetUseMove() const
{
	return m_bUseMove;
}

//============================================================================
// �ړ������邩�ǂ����ݒ�
//============================================================================
void CTarget::SetUseMove(bool bUse)
{
	m_bUseMove = bUse;
}

//============================================================================
// �J�����̈ʒu�̒������g�p���邩�ǂ����擾
//============================================================================
const bool& CTarget::GetUseAdjustCameraPos() const
{
	return m_bUseAdjustCameraPos;
}

//============================================================================
// �J�����̈ʒu�̒������g�p���邩�ǂ����ݒ�
//============================================================================
void CTarget::SetUseAdjustCameraPos(bool bUse)
{
	m_bUseAdjustCameraPos = bUse;
}

//============================================================================
// 
// private�����o
// 
//============================================================================

//============================================================================
// ����
//============================================================================
void CTarget::Control()
{
	// �C���v�b�g���擾
	auto* pKeyboard = CManager::GetKeyboard();
	auto* pPad = CManager::GetPad();

	// === �����Ɏg�p����ϐ��錾 ===

	float fRot = 0.0f;  // ����
	float fX = 0.0f;    // X
	float fY = 0.0f;    // Y
	const D3DXVECTOR3& Pos = GetPos(); // �ʒu

	// === �ړ������J�n ===

	// ����\�Ȃ瓮����
	if (m_bControl)
	{
		// �E
		if (pKeyboard->GetPress(DIK_D) ||
			pPad->GetPress(CInputPad::JOYKEY::RIGHT) ||
			pPad->GetJoyStickL().X > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
		{
			fX = 1.0f;
		}
		// ��
		if (pKeyboard->GetPress(DIK_A) ||
			pPad->GetPress(CInputPad::JOYKEY::LEFT) ||
			pPad->GetJoyStickL().X < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
		{
			fX = -1.0f;
		}
		// ��
		if (pKeyboard->GetPress(DIK_W) ||
			pPad->GetPress(CInputPad::JOYKEY::UP) ||
			pPad->GetJoyStickL().Y > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
		{
			fY = 1.0f;
		}
		// ��
		if (pKeyboard->GetPress(DIK_S) ||
			pPad->GetPress(CInputPad::JOYKEY::DOWN) ||
			pPad->GetJoyStickL().Y < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
		{
			fY = -1.0f;
		}
	}

	// 1f�O�̖ړI�n�̍X�V
	m_PosTargetOld = m_PosTarget;

	// �ړ��{�^���������Ă����ړI�n���X�V
	if (fX != 0.0f || fY != 0.0f)
	{
		fRot = atan2f(fX, fY);                                          
		m_Move = { sinf(fRot) * MOVESPEED,cosf(fRot) * MOVESPEED,0.0f };
		m_PosTarget += m_Move; // �ړI�n���X�V
	}

	D3DXVECTOR3 Dir = m_PosTarget - Pos; // �ړI�n�ƌ��݈ʒu�̃x�N�g��
	m_PosOld = Pos; // 1f�O�̈ʒu���X�V

	// �ړ������鎞�����ړ�
	if (m_bUseMove)
	{
		SetPos(Pos + Dir * s_fMoveAdjust); // �ړI�n�Ɍ������ď��X�Ɉʒu���X�V
	}
	// �ʒu���f�o�b�O�\��
	CRenderer::SetDebugString("�^�[�Q�b�g�ʒu�FX = " + to_string(Pos.x) + "Y = " + to_string(Pos.y) + "Z = " + to_string(Pos.z));

	LimitRange(); // �ړ��͈͐���
}

//============================================================================
// ��ʊO�ɏo�Ȃ��悤�ɂ���
//============================================================================
void CTarget::LimitRange()
{
	// === �����Ɏg�p������錾 ===

	CCamera* pCamera = CCamera_Manager::GetInstance()->RefRegistry().BindAtKey("Game"); // �J�����ւ̃|�C���^
	D3DXVECTOR3 PosTarget = m_PosTarget,   // �ʒu
                Size = GetSize(), // �T�C�Y
	            PosVtxMax = PosTarget + Size, // �ő咸�_�̈ʒu
	            PosVtxMin = PosTarget - Size, // �ŏ����_�̈ʒu

	            // �ő咸�_�̃X�N���[�����W
                ScreenPos_VtxMax = Calculation::Pos::WorldToScreen(PosVtxMax, *pCamera->GetMtxView(), *pCamera->GetMtxProj(), FSCREEN_WIDTH, FSCREEN_HEIGHT),
	            
	            // �ŏ����_�̃X�N���[�����W
	            ScreenPos_VtxMin = Calculation::Pos::WorldToScreen(PosVtxMin, *pCamera->GetMtxView(), *pCamera->GetMtxProj(), FSCREEN_WIDTH, FSCREEN_HEIGHT),
	            
                TurnVec = m_PosTargetOld - PosTarget, // �߂点��x�N�g��
	            Move = VEC3_INIT;         // �ړ���
	bool bTurn = false; // �߂����ǂ���

	// === �ړ��͈͐��������J�n ===

	//// �ő�ŏ����_X����ʊO�ɏo�Ă�����߂�
	//if (ScreenPos_VtxMax.x < 0.0f || ScreenPos_VtxMax.x > FSCREEN_WIDTH ||
	//	ScreenPos_VtxMin.x < 0.0f || ScreenPos_VtxMin.x > FSCREEN_WIDTH)
	//{
	//	Move.x = TurnVec.x * 1.0f;
	//	bTurn = true;
	//}
	//// �ő�ŏ����_Y����ʊO�ɏo�Ă�����߂�
	//if (ScreenPos_VtxMax.y < 0.0f || ScreenPos_VtxMax.y > FSCREEN_HEIGHT ||
	//	ScreenPos_VtxMin.y < 0.0f || ScreenPos_VtxMin.y > FSCREEN_HEIGHT)
	//{
	//	Move.y = TurnVec.y * 1.0f;
	//	bTurn = true;
	//}
	//// �ő�ŏ����_Z����ʊO�ɏo�Ă�����߂�
	//if (ScreenPos_VtxMax.z < 0.0f || ScreenPos_VtxMax.z > FSCREEN_WIDTH ||
	//	ScreenPos_VtxMin.z < 0.0f || ScreenPos_VtxMin.z > FSCREEN_WIDTH)
	//{
	//	Move.z = TurnVec.z * 1.0f;
	//	bTurn = true;
	//}

	if (PosTarget.x > s_fMoveLimitMaxX || PosTarget.x < s_fMoveLimitMinX)
	{
		Move.x = TurnVec.x * 1.0f;
		bTurn = true;
	}
	if (PosTarget.y > s_fMoveLimitMaxY || PosTarget.y < s_fMoveLimitMinY)
	{
		Move.y = TurnVec.y * 1.0f;
		bTurn = true;
	}

	// 1f�O�̈ʒu�ւ̃x�N�g�����ʒu��߂�
	if (bTurn)
	{
		m_PosTarget += Move;
	}
}

//============================================================================
// �J�����̈ʒu�𒲐�����
//============================================================================
void CTarget::AdjustCameraPos()
{
	if (!m_bUseAdjustCameraPos)
	{
		return;
	}

	const D3DXVECTOR3& SelfPos = GetPos(); // ���g�̈ʒu

	// �J�����ւ̃|�C���^
	CCamera* pCamera = CCamera_Manager::GetInstance()->RefRegistry().BindAtKey("Game");

	// ���݂̈ʒu�̔����̈ʒu�ɃJ������ݒ�
	pCamera->SetPos(D3DXVECTOR3(SelfPos.x, SelfPos.y, 0.0f) / 2);
}