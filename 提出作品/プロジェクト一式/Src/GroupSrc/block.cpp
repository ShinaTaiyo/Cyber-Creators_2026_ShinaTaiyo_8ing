//============================================================================
// 
// �Q�O�Q�T�N�S���Q�W���F�u���b�N�N���X���쐬 [block.cpp]
// Author : �Ŗ����z
// 
//============================================================================

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "block.h"        // ���w�b�_
#include "bounding_box.h" // ��������
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
CBlock::CBlock() :
	CObject_X{ LAYER::DEFAULT },
	m_pCollisionInfo_Box{DBG_NEW CCollisionInfo_BoundingBox()}
{
	SetType(CObject::TYPE::BLOCK); // �^�C�v���u�u���b�N�v�ɐݒ�

	// �������񏉊���
	m_pCollisionInfo_Box->SetUpdateInfo(GetPos(),VEC3_INIT, VEC3_INIT, GetSize(),GetVtxMax(),GetVtxMin(),false,false,false,false);
}

//============================================================================
// �f�X�g���N�^
//============================================================================
CBlock::~CBlock()
{

}

//============================================================================
// �����ݒ�
//============================================================================
HRESULT CBlock::Init()
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
void CBlock::Uninit()
{
	if (m_pCollisionInfo_Box != nullptr)
	{
		delete m_pCollisionInfo_Box;    // ���
		m_pCollisionInfo_Box = nullptr; // �|�C���^������
	}

	// X�I�u�W�F�N�g�N���X�̏I������
	CObject_X::Uninit();
}

//============================================================================
// �X�V����
//============================================================================
void CBlock::Update()
{
	// X�I�u�W�F�N�g�N���X�̍X�V
	CObject_X::Update();

	// ��������X�V
	m_pCollisionInfo_Box->SetUpdateInfo(GetPos(),VEC3_INIT,VEC3_INIT, GetSize(), GetVtxMax(), GetVtxMin(), false, false, false, false);
}

//============================================================================
// �`�揈��
//============================================================================
void CBlock::Draw()
{
	// X�I�u�W�F�N�g�N���X�̕`�揈��
	CObject_X::Draw();
}

//============================================================================
// ������擾
//============================================================================
CCollisionInfo* CBlock::GetCollisionInfo() const
{
	return m_pCollisionInfo_Box;
}

//============================================================================
// ����
//============================================================================
CBlock* CBlock::Create(D3DXVECTOR3 InitPos)
{
	// �C���X�^���X�𐶐�
	CBlock* pBlock = DBG_NEW CBlock;

	// �������s
	if (pBlock == nullptr)
	{ 
		assert(false);
	}

	// �^�C�v��ݒ�
	pBlock->SetType(TYPE::BLOCK);

	// �����ݒ�
	pBlock->Init();

	// �����ʒu�̐ݒ�
	pBlock->SetPos(InitPos);

	// ���f����ݒ�
	pBlock->BindModel(CX_Manager::GetInstance()->RefRegistry().BindAtKey("Block"));

	// �T�C�Y��ݒ�
	pBlock->SetSize(pBlock->GetModel()->Size);

	// �ő咸�_�ݒ�
	pBlock->SetBaseVtxMax(pBlock->GetModel()->VtxMax);

	// �ŏ����_�ݒ�
	pBlock->SetBaseVtxMin(pBlock->GetModel()->VtxMin);

	//������ݒ�
	pBlock->m_pCollisionInfo_Box->SetUpdateInfo(pBlock->GetPos(),VEC3_INIT,VEC3_INIT, pBlock->GetSize(), pBlock->GetVtxMax(), pBlock->GetVtxMin(), false, false, false, false);

	return pBlock;
}