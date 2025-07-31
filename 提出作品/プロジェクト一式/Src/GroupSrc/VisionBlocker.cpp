//============================================================================
// 
// �Q�O�Q�T�N�U���P�Q���F����W�Q�������쐬 [VisionBlocker.cpp]
// Author : �Ŗ����z
// 
//============================================================================

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************

// �I�u�W�F�N�g
#include "VIsionBlocker.h"
#include "StencilMask.h"

// ���
#include "move_info.h"

// ���͗p
#include "manager.h"
#include "renderer.h"
#include "input_keyboard.h"

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
// �ÓI�����o�錾
//============================================================================

// ��Q���̂��ꂼ��̃��f���^�C�v��錾
const std::string CVisionBlocker::s_aVisionBlockerModelType[static_cast<int>(CVisionBlocker::TYPE::MAX)] =
{
	"Leaf_000",
	"Leaf_001",
	"Leaf_002",
};

//============================================================================
// �R���X�g���N�^
//============================================================================
CVisionBlocker::CVisionBlocker() :
	CObject_X{ LAYER::DEFAULT },
	m_Type{TYPE::LEAF00},
    m_fLeafSwayAngle{0.0f},
	m_fLeafSwayAddAngle{0.0f},
	m_fLeafSwaySpeed{0.0f},
	m_AddRot{VEC3_INIT},
	m_nLife{1},
	m_BlowMove{VEC3_INIT},
	m_fBlowInertia{0.1f},
	m_pMove_Info{DBG_NEW CMove_Info()}
{
	SetType(TYPE::LEAF00); // �^�C�v���u�t�O�v�ɐݒ�
}

//============================================================================
// �f�X�g���N�^
//============================================================================
CVisionBlocker::~CVisionBlocker()
{
	// �����Ȃ�
}

//============================================================================
// �����ݒ�
//============================================================================
HRESULT CVisionBlocker::Init()
{
	// X�I�u�W�F�N�g�N���X�̏����ݒ�
	if (FAILED(CObject_X::Init()))
	{
		assert(false && "�v���C���[�̏����ݒ�Ɏ��s");
	}
	return S_OK;
}

//============================================================================
// �I������
//============================================================================
void CVisionBlocker::Uninit()
{
	// �ړ����I��
	if (m_pMove_Info != nullptr)
	{
		delete m_pMove_Info;
		m_pMove_Info = nullptr;
	}

	// X�I�u�W�F�N�g�N���X�̏I������
	CObject_X::Uninit();
}

//============================================================================
// �X�V����
//============================================================================
void CVisionBlocker::Update()
{
	D3DXVECTOR3 Pos = GetPos(); // �ʒu
	const D3DXVECTOR3& CurrentPos = GetPos(); // �Q�ƈʒu

	// �ړ����̍X�V���s��
	if (m_pMove_Info)
	{
		D3DXVECTOR3 LeafMove = { cosf(m_fLeafSwayAngle) * m_fLeafSwaySpeed,0.0f,0.0f }; // �t���ς̈ړ��ʂɗh���ړ��ʂ����Z
		m_fLeafSwayAngle += m_fLeafSwayAddAngle; // �t���ς�h�炷�p�x���Z
		m_pMove_Info->Update(Pos, this); //	�ړ����X�V
		SetPos(Pos); // �ʒu�ݒ�
		SetPos(CurrentPos + LeafMove);   // �h�炷�ړ��ʂňʒu�����Z
	}

	// ������΂�����
	Blow();

	// ���������Z����
	SetRot(GetRot() + m_AddRot);

	// �̗͂��Ȃ��Ȃ�����j������
	if (m_nLife <= 0)
	{
		SetRelease();
	}

	// X�I�u�W�F�N�g�N���X�̍X�V
	CObject_X::Update();

	// �̗͂����炷
	m_nLife--;
}

//============================================================================
// �`�揈��
//============================================================================
void CVisionBlocker::Draw()
{
	// �X�e���V���o�b�t�@�̕`��ݒ�
	CStencilMask::SetOtherStencilState();

	// X�I�u�W�F�N�g�N���X�̕`�揈��
	CObject_X::Draw();

	// �X�e���V���o�b�t�@�̕`��ݒ�����ɖ߂�
	CStencilMask::SetOtherResetStencilState();
}

//============================================================================
// ��ސݒ�
//============================================================================
void CVisionBlocker::SetType(TYPE Type)
{
	m_Type = Type;
}

//============================================================================
// �t���ς�h�炷�p�x�擾
//============================================================================
const float& CVisionBlocker::GetLeafSwayAngle() const
{
	return m_fLeafSwayAngle;
}

//============================================================================
// �t���ς�h�炷�p�x�ݒ�
//============================================================================
void CVisionBlocker::SetLeafSwayAngle(float fAngle)
{
	m_fLeafSwayAngle = fAngle;
}

//============================================================================
// �t���ς�h�炷�p�x���Z�ʎ擾
//============================================================================
const float& CVisionBlocker::GetLeafSwayAddAngle() const
{
	return m_fLeafSwayAddAngle;
}

//============================================================================
// �t���ς�h�炷�p�x���Z�ʐݒ�
//============================================================================
void CVisionBlocker::SetLeafSwayAddAngle(float fAddAngle)
{
	m_fLeafSwayAddAngle = fAddAngle;
}

//============================================================================
// �t���ς�h�炷���x�擾
//============================================================================
const float& CVisionBlocker::GetLeafSwaySpeed() const
{
	return m_fLeafSwaySpeed;
}

//============================================================================
// �t���ς�h�炷���x�ݒ�
//============================================================================
void CVisionBlocker::SetLeafSwaySpeed(float fSwaySpeed)
{
	m_fLeafSwaySpeed = fSwaySpeed;
}

//============================================================================
// �p�x���Z�ʎ擾
//============================================================================
const D3DXVECTOR3& CVisionBlocker::GetAddRot() const
{
	return m_AddRot;
}

//============================================================================
// �p�x���Z�ʐݒ�
//============================================================================
void CVisionBlocker::SetAddRot(D3DXVECTOR3 AddRot)
{
	m_AddRot = AddRot;
}

//============================================================================
// ������΂��ړ��ʎ擾
//============================================================================
const D3DXVECTOR3& CVisionBlocker::GetBlowMove() const
{
	return m_BlowMove;
}

//============================================================================
// ������΂��ړ��ʐݒ�
//============================================================================
void CVisionBlocker::SetBlowMove(D3DXVECTOR3 Move)
{
	m_BlowMove = Move;
}

//============================================================================
// ������΂������擾
//============================================================================
const float& CVisionBlocker::GetBlowInertia() const
{
	return m_fBlowInertia;
}

//============================================================================
// ������΂������ݒ�
//============================================================================
void CVisionBlocker::SetBlowInertia(float fInertia)
{
	m_fBlowInertia = fInertia;
}

//============================================================================
// ��ގ擾
//============================================================================
const CVisionBlocker::TYPE& CVisionBlocker::GetType() const
{
	return m_Type;
}

//============================================================================
// �ړ����擾
//============================================================================
CMove_Info* CVisionBlocker::GetMoveInfo() const
{
	return m_pMove_Info;
}

//============================================================================
// �̗͎擾
//============================================================================
const int& CVisionBlocker::GetLife() const
{
	return m_nLife;
}

//============================================================================
// �̗͐ݒ�
//============================================================================
void CVisionBlocker::SetLife(int nLife)
{
	m_nLife = nLife;
}

//============================================================================
// ����
//============================================================================
CVisionBlocker* CVisionBlocker::Create(TYPE Type, D3DXVECTOR3 InitPos)
{
	// �C���X�^���X�𐶐�
	CVisionBlocker* pVisionBlocker = DBG_NEW CVisionBlocker();

	// �������s
	if (pVisionBlocker == nullptr)
	{
		assert(false);
	}

	// �^�C�v��ݒ�
	pVisionBlocker->CObject::SetType(CObject::TYPE::VISIONBLOCKER);

	// ��Q���^�C�v�ݒ�
	pVisionBlocker->SetType(Type);

	// �����ݒ�
	pVisionBlocker->Init();

	// �����ʒu�̐ݒ�
	pVisionBlocker->SetPos(InitPos);

	// ���f����ݒ�
	pVisionBlocker->BindModel(CX_Manager::GetInstance()->RefRegistry().BindAtKey(s_aVisionBlockerModelType[static_cast<int>(Type)]));

	// �T�C�Y��ݒ�
	pVisionBlocker->SetSize(pVisionBlocker->GetModel()->Size);

	// �ő咸�_�ݒ�
	pVisionBlocker->SetBaseVtxMax(pVisionBlocker->GetModel()->VtxMax);

	// �ŏ����_�ݒ�
	pVisionBlocker->SetBaseVtxMin(pVisionBlocker->GetModel()->VtxMin);

	return pVisionBlocker;
}

//============================================================================
// ������΂�����
//============================================================================
void CVisionBlocker::Blow()
{
	const D3DXVECTOR3& CurrentPos = GetPos(); // �Q�ƈʒu

	// ������΂��ړ��ʂ�ݒ�
	SetPos(CurrentPos + m_BlowMove);

	// ������΂��ړ��ʌ���
	m_BlowMove += (VEC3_INIT - m_BlowMove) * m_fBlowInertia;
}
