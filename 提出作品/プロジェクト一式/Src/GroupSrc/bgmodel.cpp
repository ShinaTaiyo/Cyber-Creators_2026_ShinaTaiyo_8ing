//============================================================================
// 
// �Q�O�Q�T�N�U���P�P���F��]�~���Ƃ̓����蔻����s�� [BgModel.cpp]
// Author : �Ŗ����z
// 
//============================================================================

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "bgmodel.h"
#include "RotatingCylinder.h" // ��]�~�����
#include "move_info.h" // �ړ����

// ���͗p
#include "manager.h"

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
CBgModel::CBgModel() :
	CObject_X{ LAYER::DEFAULT },
	m_pRotating_Cylinder{ DBG_NEW CRotatingCylinder() },
	m_AddRot{VEC3_INIT},
	m_pMoveInfo{DBG_NEW CMove_Info()}
{
	SetType(CObject::TYPE::BGMODEL);  // �^�C�v���u�u���b�N�v�ɐݒ�
}

//============================================================================
// �f�X�g���N�^
//============================================================================
CBgModel::~CBgModel()
{
	// �����Ȃ�
}

//============================================================================
// �����ݒ�
//============================================================================
HRESULT CBgModel::Init()
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
void CBgModel::Uninit()
{
	// ��]�~�����I��
	if (m_pRotating_Cylinder != nullptr)
	{
		delete m_pRotating_Cylinder;
		m_pRotating_Cylinder = nullptr;
	}

	// �ړ����I��
	if (m_pMoveInfo != nullptr)
	{
		delete m_pMoveInfo;
		m_pMoveInfo = nullptr;
	}

	// X�I�u�W�F�N�g�N���X�̏I������
	CObject_X::Uninit();
}

//============================================================================
// �X�V����
//============================================================================
void CBgModel::Update()
{
	D3DXVECTOR3 Pos = GetPos(); // �ʒu
	D3DXMATRIX MtxWorld = GetMtxWorld(); // ���[���h�}�g���b�N�X�擾

	// X�I�u�W�F�N�g�N���X�̍X�V
	CObject_X::Update();

	// ���������Z
	SetRot(GetRot() + m_AddRot);

	// �ړ����X�V
	if (m_pMoveInfo)
	{
		m_pMoveInfo->Update(Pos, this);
		SetPos(Pos);
	}

	// ��]�~�����X�V
	m_pRotating_Cylinder->SetPos(GetPos());
	m_pRotating_Cylinder->SetVtxMax(GetVtxMax());
	m_pRotating_Cylinder->SetVtxMin(GetVtxMin());
	m_pRotating_Cylinder->SetRot(GetRot());
}

//============================================================================
// �`�揈��
//============================================================================
void CBgModel::Draw()
{
	// X�I�u�W�F�N�g�N���X�̕`�揈��
	CObject_X::Draw();
}

//============================================================================
// �������Z�ʂ�ݒ�
//============================================================================
void CBgModel::SetAddRot(D3DXVECTOR3 AddRot)
{
	m_AddRot = AddRot;
}

//============================================================================
// �������Z�ʂ��擾
//============================================================================
const D3DXVECTOR3& CBgModel::GetAddRot() const
{
	return m_AddRot;
}

//============================================================================
// ������擾
//============================================================================
CCollisionInfo* CBgModel::GetCollisionInfo() const
{
	return m_pRotating_Cylinder;
}

//============================================================================
// �ړ����擾
//============================================================================
CMove_Info* CBgModel::GetMoveInfo() const
{
	return m_pMoveInfo;
}

//============================================================================
// ����
//============================================================================
CBgModel* CBgModel::Create(D3DXVECTOR3 InitPos)
{
	// �C���X�^���X�𐶐�
	CBgModel* pBgModel = DBG_NEW CBgModel;

	// �������s
	if (pBgModel == nullptr)
	{
		assert(false);
	}

	// �^�C�v��ݒ�
	pBgModel->SetType(TYPE::BGMODEL);

	// �����ݒ�
	pBgModel->Init();

	// �����ʒu�̐ݒ�
	pBgModel->SetPos(InitPos);

	// ���f����ݒ�
	pBgModel->BindModel(CX_Manager::GetInstance()->RefRegistry().BindAtKey("Cylinder"));

	// �T�C�Y��ݒ�
	pBgModel->SetSize(pBgModel->GetModel()->Size);

	// �ő咸�_�ݒ�
	pBgModel->SetBaseVtxMax(pBgModel->GetModel()->VtxMax);

	// �ŏ����_�ݒ�
	pBgModel->SetBaseVtxMin(pBgModel->GetModel()->VtxMin);

	// ��]�~����񏉊���
	CRotatingCylinder * pRotCylinder = pBgModel->m_pRotating_Cylinder;
	pRotCylinder->SetPos(pBgModel->GetPos());
	pRotCylinder->SetVtxMax(pBgModel->GetVtxMax());
	pRotCylinder->SetVtxMin(pBgModel->GetVtxMin());
	pRotCylinder->SetRot(pBgModel->GetRot());

	return pBgModel;
}