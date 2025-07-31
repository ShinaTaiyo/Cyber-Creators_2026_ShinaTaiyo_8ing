//============================================================================
// 
// �Q�O�Q�T�N�U���P�P���F��]�~���Ƃ̓����蔻����s�� [Hole.cpp]
// Author : �Ŗ����z
// 
//============================================================================

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "Hole.h"
#include "RotatingCylinder.h" // ��]�~�����
#include "move_info.h" // �ړ����
#include "StencilMask.h" // �X�e���V���}�X�N
#include "renderer.h"
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
CHole::CHole() :
	CObject_X{ LAYER::DEFAULT },
	m_pRotating_Cylinder{ DBG_NEW CRotatingCylinder() },
	m_ParentPos{ VEC3_INIT },
	m_ParentRot{ VEC3_INIT }
{
	SetType(CObject::TYPE::HOLE);     // �^�C�v���u���v�ɐݒ�

//#ifndef  _DEBUG
	SetUseCol(true); // Release�������F�������g�p����
	SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)); // �F�����𓧖��ɂ���
//#endif // ! _DEBUG

}

//============================================================================
// �f�X�g���N�^
//============================================================================
CHole::~CHole()
{
	// �����Ȃ�
}

//============================================================================
// �����ݒ�
//============================================================================
HRESULT CHole::Init()
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
void CHole::Uninit()
{
	// ��]�~�����I��
	if (m_pRotating_Cylinder != nullptr)
	{
		delete m_pRotating_Cylinder;
		m_pRotating_Cylinder = nullptr;
	}

	// X�I�u�W�F�N�g�N���X�̏I������
	CObject_X::Uninit();
}

//============================================================================
// �X�V�����i�I�u�W�F�N�g�N���X�ŌĂт����Ȃ��j
//============================================================================
void CHole::ExtraUpdate()
{
	// X�I�u�W�F�N�g�N���X�̍X�V
	CObject_X::Update();

	const D3DXVECTOR3&
		VtxMax = GetVtxMax(), // �ő咸�_
		VtxMin = GetVtxMin(), // �ŏ����_
		Rot = GetRot();       // ����
	

	// ��]�~�����X�V
	m_pRotating_Cylinder->SetPos(GetPos());
	m_pRotating_Cylinder->SetVtxMax(VtxMax);
	m_pRotating_Cylinder->SetVtxMin(VtxMin);
	m_pRotating_Cylinder->SetRot(Rot);
}

//============================================================================
// �`�揈��
//============================================================================
void CHole::Draw()
{
	// �f�o�C�X�擾
	LPDIRECT3DDEVICE9 pDev = CRenderer::GetRenderer()->GetDeviece();
#ifdef _DEBUG
	// �X�e���V���}�X�N�̉e�����󂯂����Ȃ��ꏊ�ŌĂ�
	CStencilMask::SetOtherStencilAvoid();

	CObject_X::Draw();

	// �X�e���V���ݒ�����ɖ߂�
	CStencilMask::SetOtherResetStencilState();

#endif // _DEBUG
}

//============================================================================
// ��]�~�����擾
//============================================================================
CRotatingCylinder* CHole::GetRotatingCylinder() const
{
	return m_pRotating_Cylinder;
}

//============================================================================
// �e�̈ʒu��ݒ�
//============================================================================
void CHole::SetParentPos(D3DXVECTOR3 ParentPos)
{
	m_ParentPos = ParentPos;
}

//============================================================================
// �e�̌�����ݒ�
//============================================================================
void CHole::SetParentRot(D3DXVECTOR3 ParentRot)
{
	m_ParentRot = ParentRot;
}

//============================================================================
// ����
//============================================================================
CHole* CHole::Create(D3DXVECTOR3 InitPos)
{
	// �C���X�^���X�𐶐�
	CHole* pHole = DBG_NEW CHole;

	// �������s
	if (pHole == nullptr)
	{
		assert(false);
	}

	// �^�C�v��ݒ�
	pHole->SetType(TYPE::HOLE);

	// �����ݒ�
	pHole->Init();

	// �����ʒu�̐ݒ�
	pHole->SetPos(InitPos);

	// ���f����ݒ�
	pHole->BindModel(CX_Manager::GetInstance()->RefRegistry().BindAtKey("Cylinder"));

	// �T�C�Y��ݒ�
	pHole->SetSize(pHole->GetModel()->Size);

	// �ő咸�_�ݒ�
	pHole->SetBaseVtxMax(pHole->GetModel()->VtxMax);

	// �ŏ����_�ݒ�
	pHole->SetBaseVtxMin(pHole->GetModel()->VtxMin);

	// ��]�~����񏉊���
	CRotatingCylinder* pRotCylinder = pHole->m_pRotating_Cylinder;
	pRotCylinder->SetPos(pHole->GetPos());
	pRotCylinder->SetVtxMax(pHole->GetBaseVtxMax());
	pRotCylinder->SetVtxMin(pHole->GetBaseVtxMin());
	pRotCylinder->SetRot(pHole->GetRot());

	return pHole;
}

//============================================================================
// ���[���h�}�g���b�N�X��ݒ肷��i�e�̊g�嗦�𔽉f���������Ȃ��̂Ŏ蓮�Őݒ�j
//============================================================================
void CHole::UpdateMtxWorld()
{
	// ���Ȃ��e�̃��[���h�s�񂩂�ʒu��������Q�Ƃ��Ȃ��H
	//�����[���h�s��̈ʒu������ɂ́A���񔽉f���������Ȃ�"�X�P�[��"�����f����Ă��܂��܂��B�X�P�[���g�������Ȃ��̂�
	//�X�P�[�������ɂ��Ă�����x�v�Z�������܂��B

	// �v�Z�p�}�g���b�N�X
	D3DXMATRIX mtxScale, mtxRot, mtxTrans;
	D3DXMATRIX mtxWorld;

	// ���[���h�}�g���b�N�X��������
	D3DXMatrixIdentity(&mtxWorld);

	const D3DXVECTOR3& Pos = GetPos();     // �ʒu�擾
	const D3DXVECTOR3& Scale = GetScale(); // �g�嗦�擾
	const D3DXVECTOR3& Rot = GetRot();     // �����擾

   // �����̃X�P�[�������[���h�s��Ɋ|�����킹��
	D3DXMatrixScaling(&mtxScale, Scale.x, Scale.y, Scale.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxScale);

	// ���g�̌��������[���h�s��Ɋ|�����킹��
	D3DXMatrixRotationYawPitchRoll(&mtxRot, Rot.y, Rot.x, Rot.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

	// ���g�̈ʒu�����[���h�s��Ɋ|�����킹��
	D3DXMatrixTranslation(&mtxTrans, Pos.x, Pos.y, Pos.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrans);

	// �e�̃X�P�[���𖳎��������ꍇ�A�e�̉�]�ƈʒu�������|����i���Ԃɒ��Ӂj
	D3DXMATRIX 
		mtxParentRot, 
		mtxParentTrans,
		mtxParentNoScale;

	// �X�P�[���𖳎�����e�̃��[���h�}�g���b�N�X��������
	D3DXMatrixIdentity(&mtxParentNoScale);

	// �e�̌�����e�̃��[���h�s��Ɋ|�����킹��
	D3DXMatrixRotationYawPitchRoll(&mtxParentRot, m_ParentRot.y, m_ParentRot.x, m_ParentRot.z);
	D3DXMatrixMultiply(&mtxParentNoScale, &mtxParentNoScale, &mtxParentRot);

	// �e�̈ʒu��e�̃��[���h�s��Ɋ|�����킹��
	D3DXMatrixTranslation(&mtxParentTrans, m_ParentPos.x, m_ParentPos.y, m_ParentPos.z);
	D3DXMatrixMultiply(&mtxParentNoScale, &mtxParentNoScale, &mtxParentTrans);

	// �e�Ǝq���|�����킹��
	D3DXMatrixMultiply(&mtxWorld,&mtxWorld,&mtxParentNoScale);

	// �Z�b�g
	SetMtxWorld(mtxWorld);
}