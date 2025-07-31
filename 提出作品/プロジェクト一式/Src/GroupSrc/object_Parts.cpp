//============================================================================
// 
// �p�[�c�I�u�W�F�N�g [object_Parts.cpp]
// Author : ���c����
// 
//============================================================================

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "object_parts.h"
#include "renderer.h"

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
CObject_Parts::CObject_Parts(LAYER Priority) :
	CObject{ Priority },
	m_PosSync{ VEC3_INIT },
	m_RotSync{ VEC3_INIT },
	m_pParent{ nullptr },
	m_pModel{ nullptr },
	m_ScaleOffset{ VEC3_INIT },
	m_Scale{ 1.0f, 1.0f, 1.0f },
	m_RotOffset{ VEC3_INIT },
	m_Rot{ VEC3_INIT },
	m_PosOffset{ VEC3_INIT },
	m_Pos{ VEC3_INIT },
	m_Col{ XCOL_INIT },
	m_bUseCol{ false }
{
	// ���[���h�s���������
	D3DXMatrixIdentity(&m_MtxWorld);
}

//============================================================================
// �f�X�g���N�^
//============================================================================
CObject_Parts::~CObject_Parts()
{

}

//============================================================================
// �����ݒ�
//============================================================================
HRESULT CObject_Parts::Init()
{
	return S_OK;
}

//============================================================================
// �I������
//============================================================================
void CObject_Parts::Uninit()
{

}

//============================================================================
// �X�V����
//============================================================================
void CObject_Parts::Update()
{
	// �����𒲐�����
	AdjustRotAngle();

	// ���[���h�s��̐ݒ�
	SetMtxWorld();
}

//============================================================================
// �`�揈��
//============================================================================
void CObject_Parts::Draw()
{
	// ���f�����ݒ肳��Ă��Ȃ�
	if (m_pModel == nullptr)
	{
#ifdef _DEBUG	// �x���\��

		CRenderer::SetDebugString(static_cast<std::string>("�y") + typeid(*this).name() + static_cast<std::string>("�Ƀ��f����񂪂���܂���I�z"));

#endif	// _DEBUG

		return;
	}

	// �f�o�C�X���擾
	LPDIRECT3DDEVICE9 pDev = CRenderer::GetDeviece();

	// ���_�@���̎������K����L����
	pDev->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);

	// ���݂̃}�e���A���ۑ��p
	D3DMATERIAL9 matDef;

	// �}�e���A�����ւ̃|�C���^
	D3DXMATERIAL* pMat = nullptr;

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDev->SetTransform(D3DTS_WORLD, &m_MtxWorld);

	// ���݂̃}�e���A�����擾
	pDev->GetMaterial(&matDef);

	// �}�e���A���f�[�^�ւ̃|�C���^���擾
	pMat = (D3DXMATERIAL*)m_pModel->pBuffMat->GetBufferPointer();

	for (WORD wCntMat = 0; wCntMat < static_cast<WORD>(m_pModel->dwNumMat); wCntMat++)
	{
		// �}�e���A���F�̐ݒ�
		m_bUseCol ? pMat[wCntMat].MatD3D.Diffuse = m_Col : pMat[wCntMat].MatD3D.Diffuse = m_pModel->apColMat[wCntMat];

		// �}�e���A���̐ݒ�
		pDev->SetMaterial(&pMat[wCntMat].MatD3D);

		// �e�N�X�`���̐ݒ�
		pDev->SetTexture(0, m_pModel->apTex[wCntMat]);

		// �I�u�W�F�N�g�p�[�c�̕`��
		m_pModel->pMesh->DrawSubset(wCntMat);
	}

	// �ۑ����Ă����}�e���A����߂�
	pDev->SetMaterial(&matDef);

	// ���_�@���̎������K���𖳌���
	pDev->SetRenderState(D3DRS_NORMALIZENORMALS, FALSE);
}

//============================================================================
// �V���N�������̐ݒ�
//============================================================================
void CObject_Parts::SetSyncRot(D3DXVECTOR3 Rot)
{
	m_RotSync = Rot;
}

//============================================================================
// �V���N�����W�̐ݒ�
//============================================================================
void CObject_Parts::SetSyncPos(D3DXVECTOR3 Pos)
{
	m_PosSync = Pos;
}

//============================================================================
// �e�p�[�c�̃|�C���^�擾
//============================================================================
const CObject_Parts* const CObject_Parts::GetParent()const
{
	return m_pParent;
}

//============================================================================
// �e�p�[�c�̃|�C���^�ݒ�
//============================================================================
void CObject_Parts::SetParent(CObject_Parts* pParent)
{
	m_pParent = pParent;
}

//============================================================================
// ���f���擾
//============================================================================
CX_Manager::MODEL* CObject_Parts::GetModel()
{
	return m_pModel;
}

//============================================================================
// ���f������
//============================================================================
void CObject_Parts::BindModel(CX_Manager::MODEL* pModel)
{
	m_pModel = pModel;
}

//============================================================================
// �k�ڃI�t�Z�b�g���擾
//============================================================================
const D3DXVECTOR3& CObject_Parts::GetScaleOffset() const
{
	return m_ScaleOffset;
}

//============================================================================
// �k�ڃI�t�Z�b�g��ݒ�
//============================================================================
void CObject_Parts::SetScaleOffset(D3DXVECTOR3 Scale)
{
	m_ScaleOffset = Scale;
}

//============================================================================
// �k�ڂ��擾
//============================================================================
const D3DXVECTOR3& CObject_Parts::GetScale() const
{
	return m_Scale;
}

//============================================================================
// �k�ڂ�ݒ�
//============================================================================
void CObject_Parts::SetScale(D3DXVECTOR3 Scale)
{
	m_Scale = Scale;
}

//============================================================================
// �����I�t�Z�b�g�擾
//============================================================================
const D3DXVECTOR3& CObject_Parts::GetRotOffset() const
{
	return m_RotOffset;
}

//============================================================================
// �����I�t�Z�b�g�ݒ�
//============================================================================
void CObject_Parts::SetRotOffset(D3DXVECTOR3 Rot)
{
	m_RotOffset = Rot;
}

//============================================================================
// �����擾
//============================================================================
const D3DXVECTOR3& CObject_Parts::GetRot()const
{
	return m_Rot;
}

//============================================================================
// �����ݒ�
//============================================================================
void CObject_Parts::SetRot(D3DXVECTOR3 Rot)
{
	m_Rot = Rot;
}

//============================================================================
// ���W�I�t�Z�b�g�擾
//============================================================================
const D3DXVECTOR3& CObject_Parts::GetPosOffset() const
{
	return m_PosOffset;
}

//============================================================================
// ���W�I�t�Z�b�g�ݒ�
//============================================================================
void CObject_Parts::SetPosOffset(D3DXVECTOR3 Pos)
{
	m_PosOffset = Pos;
}

//============================================================================
// ���W�擾
//============================================================================
const D3DXVECTOR3& CObject_Parts::GetPos() const
{
	return m_Pos;
}

//============================================================================
// ���W�ݒ�
//============================================================================
void CObject_Parts::SetPos(D3DXVECTOR3 Pos)
{
	m_Pos = Pos;
}

//============================================================================
// �F���擾
//============================================================================
const D3DXCOLOR& CObject_Parts::GetCol() const
{
	return m_Col;
}

//============================================================================
// �F��ݒ�
//============================================================================
void CObject_Parts::SetCol(D3DXCOLOR Col)
{
	m_Col = Col;
}

//============================================================================
// �F���f���擾
//============================================================================
const bool& CObject_Parts::GetUseCol() const
{
	return m_bUseCol;
}

//============================================================================
// �F���f��ݒ�
//============================================================================
void CObject_Parts::SetUseCol(bool bUse)
{
	m_bUseCol = bUse;
}

//============================================================================
// ���[���h�s����擾
//============================================================================
const D3DXMATRIX& CObject_Parts::GetMtxWorld() const
{
	return m_MtxWorld;
}

//============================================================================
// ���[���h�s���ݒ�
//============================================================================
void CObject_Parts::SetMtxWorld(D3DXMATRIX Mtx)
{
	m_MtxWorld = Mtx;
}

//============================================================================
// �T�C�Y���擾
//============================================================================
D3DXVECTOR3 CObject_Parts::GetSize() const
{
	return VEC3_INIT;
}

//============================================================================
// ���a���擾
//============================================================================
float CObject_Parts::GetRadius() const
{
	return 0.0f;
}

//============================================================================
// �������擾
//============================================================================
float CObject_Parts::GetHeight() const
{
	return 0.0f;
}

//============================================================================
// ����
//============================================================================
CObject_Parts* CObject_Parts::Create(CObject::LAYER Layer, const std::string& key, CObject_Parts* pParent)
{
	// �C���X�^���X�𐶐�
	CObject_Parts* pNewInstance = DBG_NEW CObject_Parts(Layer);

	// �������s
	if (pNewInstance == nullptr)
	{
		assert(false && "�p�[�c�I�u�W�F�N�g�̐����Ɏ��s���܂���");
	}

	// �e�p�[�c�̃|�C���^��ݒ�
	pNewInstance->SetParent(pParent);

	// ���f����ݒ�
	pNewInstance->BindModel(CX_Manager::GetInstance()->RefRegistry().BindAtKey(key));

	// �p�[�c�I�u�W�F�N�g�̏����ݒ�
	pNewInstance->Init();

	return pNewInstance;
}

//============================================================================
// 
// private�����o
// 
//============================================================================

//============================================================================
// �����𒲐�����
//============================================================================
void CObject_Parts::AdjustRotAngle()
{
	while (fabsf(m_Rot.y) > D3DX_PI)
	{
		hws::WrapAngleToRange(m_Rot.y);
	}
}

//============================================================================
// ���[���h�s��ݒ�
//============================================================================
void CObject_Parts::SetMtxWorld()
{
	// �v�Z�p�s��
	D3DXMATRIX mtxScale, mtxRot, mtxTrans;

	// ���[���h�s���������
	D3DXMatrixIdentity(&m_MtxWorld);

	// �g�k�s��쐬
	D3DXMatrixScaling(&mtxScale,
		m_ScaleOffset.x + m_Scale.x,
		m_ScaleOffset.y + m_Scale.y,
		m_ScaleOffset.z + m_Scale.z);

	// �g�k�s��Ƃ̊|���Z
	D3DXMatrixMultiply(&m_MtxWorld,
		&m_MtxWorld,
		&mtxScale);

	// ��]�s��쐬
	D3DXMatrixRotationYawPitchRoll(&mtxRot,
		m_RotOffset.x + m_Rot.x + m_RotSync.y,
		m_RotOffset.y + m_Rot.y + m_RotSync.x,
		m_RotOffset.z + m_Rot.z + m_RotSync.z);

	// ��]�s��Ƃ̊|���Z
	D3DXMatrixMultiply(&m_MtxWorld,
		&m_MtxWorld,
		&mtxRot);

	// ���s�ړ��s��쐬
	D3DXMatrixTranslation(&mtxTrans,
		m_PosOffset.x + m_Pos.x + m_PosSync.x,
		m_PosOffset.y + m_Pos.y + m_PosSync.y,
		m_PosOffset.z + m_Pos.z + m_PosSync.z);

	// ���s�ړ��s��Ƃ̊|���Z
	D3DXMatrixMultiply(&m_MtxWorld,
		&m_MtxWorld,
		&mtxTrans);

	// �e�p�[�c��������΂����ŏI��
	if (m_pParent == nullptr)
	{
		return;
	}

	// �e�p�[�c�̃��[���h�s����擾
	D3DXMATRIX mtxParent = m_pParent->GetMtxWorld();

	// �e�p�[�c�̃��[���h�s��Ƃ������킹��
	D3DXMatrixMultiply(&m_MtxWorld,
		&m_MtxWorld,
		&mtxParent);
}