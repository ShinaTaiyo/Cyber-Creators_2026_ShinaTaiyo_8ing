//============================================================================
// 
// X�I�u�W�F�N�g [object_X.cpp]
// Author : ���c����
// 
//============================================================================

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "object_X.h"
#include "renderer.h"
#include "manager.h"
#include "StencilMask.h" // �X�e���V���}�X�N
#include "scene.h"   // ���݂̃V�[�������ʂ��邽��
#include "game.h"    // ���̃V�[�����u�Q�[���v��������̔��ʂ̂���

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
CObject_X::CObject_X(LAYER Priority) :
	CObject{ Priority },
	m_pModel{ nullptr },
	m_Pos{ VEC3_INIT },
	m_Rot{ VEC3_INIT },
	m_Scale{ 1.0f, 1.0f, 1.0f },
	m_Col{ XCOL_INIT },
	m_bUseCol{ false },
	m_Size{VEC3_INIT},
	m_VtxMax{VEC3_INIT},
	m_VtxMin{VEC3_INIT},
	m_BaseVtxMax{VEC3_INIT},
	m_BaseVtxMin{VEC3_INIT}
{
	// ���[���h�s���������
	D3DXMatrixIdentity(&m_MtxWorld);

	SetObjType(CObject::OBJTYPE::X);//�I�u�W�F�N�g���ރ^�C�v��X�ɐݒ�
}

//============================================================================
// �f�X�g���N�^
//============================================================================
CObject_X::~CObject_X()
{

}

//============================================================================
// �����ݒ�
//============================================================================
HRESULT CObject_X::Init()
{
	return S_OK;
}

//============================================================================
// �I������
//============================================================================
void CObject_X::Uninit()
{

}

//============================================================================
// �X�V����
//============================================================================
void CObject_X::Update()
{
	// Vector�n�̐��l��ԏ���
	if (m_upVectorInterpolator)
	{
		// �ۗL���Ă���ϐ���n��
		CVectorInterpolator::VectorRefWrapper rWrap(m_Scale, m_Rot, m_Pos);
		m_upVectorInterpolator->Interpolate(rWrap);
	}

	//���_�𒲐�����
	AdjustVtx();

	// �����𒲐�����
	AdjustRotAngle();

	// ���[���h�s��̐ݒ�
	UpdateMtxWorld();
}

//============================================================================
// �`�揈��
//============================================================================
void CObject_X::Draw()
{
	// ���f�����ݒ肳��Ă��Ȃ�
	if (m_pModel == nullptr)
	{
#ifdef _DEBUG
		CRenderer::SetDebugString(static_cast<std::string>("�y") + typeid(*this).name() + static_cast<std::string>("�Ƀ��f����񂪂���܂���I�z"));
#endif	// _DEBUG

		return;
	}

	// �}�e���A���̃o�b�t�@���ُ�
	if (m_pModel->pBuffMat == nullptr)
	{
#ifdef _DEBUG
		CRenderer::SetDebugString(static_cast<std::string>("�y") + typeid(*this).name() + static_cast<std::string>("�Ƀ}�e���A����񂪂���܂���I�z"));
#endif	// _DEBUG

		return;
	}

	// �f�o�C�X���擾
	LPDIRECT3DDEVICE9 pDev = CRenderer::GetDeviece();

	// ���_�@���̎������K����L����
	pDev->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);

//#ifdef _DEBUG	// ����\���n�̃��f���͐[�x�o�b�t�@�ɏ������܂Ȃ�
//	bool bDetectRenderCollision = false;
//	CX_Manager::MODEL* pModel = this->GetModel();
//	if (pModel == CX_Manager::GetInstance()->GetModel(CX_Manager::TYPE::RENDER_BOX) ||
//		pModel == CX_Manager::GetInstance()->GetModel(CX_Manager::TYPE::RENDER_CYLINDER) ||
//		pModel == CX_Manager::GetInstance()->GetModel(CX_Manager::TYPE::RENDER_SPHERE))
//	{
//		bDetectRenderCollision = true;	// ����ȗ��p
//		pDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
//	}
//#endif // _DEBUG

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDev->SetTransform(D3DTS_WORLD, &m_MtxWorld);

	// ���݂̃}�e���A���ۑ��p
	D3DMATERIAL9 matDef;

	// ���݂̃}�e���A�����擾
	pDev->GetMaterial(&matDef);

	// �}�e���A���f�[�^�ւ̃|�C���^���擾
	D3DXMATERIAL* pMat = (D3DXMATERIAL*)m_pModel->pBuffMat->GetBufferPointer();

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

	//�A���t�@�e�X�g�����ɖ߂�
	pDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	//Z�o�b�t�@�ɏ�������
	pDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	pDev->SetRenderState(D3DRS_ZENABLE, TRUE);

//#ifdef _DEBUG	// ����\���n�̃��f���͐[�x�o�b�t�@�ɏ������܂Ȃ�
//	if (bDetectRenderCollision)
//		pDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
//#endif // _DEBUG
}

//============================================================================
// ���f���擾
//============================================================================
CX_Manager::MODEL* CObject_X::GetModel()
{
	return m_pModel;
}

//============================================================================
// ���f������
//============================================================================
void CObject_X::BindModel(CX_Manager::MODEL* pModel)
{
	m_pModel = pModel;
}
//============================================================================
// �����擾
//============================================================================
const D3DXVECTOR3& CObject_X::GetRot()const
{
	return m_Rot;
}

//============================================================================
// �����ݒ�
//============================================================================
void CObject_X::SetRot(D3DXVECTOR3 Rot)
{
	m_Rot = Rot;
}

//============================================================================
// ���W�擾
//============================================================================
const D3DXVECTOR3& CObject_X::GetPos() const
{
	return m_Pos;
}


//============================================================================
// ���W�ݒ�
//============================================================================
void CObject_X::SetPos(D3DXVECTOR3 Pos)
{
	m_Pos = Pos;
}

//============================================================================
// �k�ڂ��擾
//============================================================================
const D3DXVECTOR3& CObject_X::GetScale() const
{
	return m_Scale;
}


//============================================================================
// �k�ڂ�ݒ�
//============================================================================
void CObject_X::SetScale(D3DXVECTOR3 Scale)
{
	m_Scale = Scale;
}

//============================================================================
// �ő咸�_�擾
//============================================================================
const D3DXVECTOR3& CObject_X::GetVtxMax() const
{
	return m_VtxMax;
}

//============================================================================
// �ő咸�_�ݒ�
//===========================================================================
void CObject_X::SetVtxMax(D3DXVECTOR3 VtxMax)
{
	m_VtxMax = VtxMax;
}

//============================================================================
// �ŏ����_�擾
//===========================================================================
const D3DXVECTOR3& CObject_X::GetVtxMin() const
{
	return m_VtxMin;
}

//============================================================================
// �ŏ����_�ݒ�
//===========================================================================
void CObject_X::SetVtxMin(D3DXVECTOR3 VtxMin)
{
	m_VtxMin = VtxMin;
}

//============================================================================
// ���̍ő咸�_�擾
//============================================================================
const D3DXVECTOR3& CObject_X::GetBaseVtxMax() const
{
	return m_BaseVtxMax;
}

//============================================================================
// ���̍ő咸�_�ݒ�
//============================================================================
void CObject_X::SetBaseVtxMax(D3DXVECTOR3 BaseVtxMax)
{
	m_BaseVtxMax = BaseVtxMax;
}

//============================================================================
// ���̍ŏ����_�擾
//============================================================================
const D3DXVECTOR3& CObject_X::GetBaseVtxMin() const
{
	return m_BaseVtxMin;
}

//============================================================================
// ���̍ŏ����_�ݒ�
//============================================================================
void CObject_X::SetBaseVtxMin(D3DXVECTOR3 BaseVtxMin)
{
	m_BaseVtxMin = BaseVtxMin;
}

//============================================================================
// �F���擾
//============================================================================
const D3DXCOLOR& CObject_X::GetCol() const
{
	return m_Col;
}

//============================================================================
// �F��ݒ�
//============================================================================
void CObject_X::SetCol(D3DXCOLOR Col)
{
	m_Col = Col;
}

//============================================================================
// �T�C�Y�ݒ�
//============================================================================
void CObject_X::SetSize(D3DXVECTOR3 Size)
{
	m_Size = Size;
}

//============================================================================
// �F���f���擾
//============================================================================
const bool& CObject_X::GetUseCol() const
{
	return m_bUseCol;
}

//============================================================================
// �F���f��ݒ�
//============================================================================
void CObject_X::SetUseCol(bool bUse)
{
	m_bUseCol = bUse;
}

//============================================================================
// �T�C�Y���擾
//============================================================================
D3DXVECTOR3 CObject_X::GetSize() const
{
	return m_Size;
}

//============================================================================
// ���a���擾
//============================================================================
float CObject_X::GetRadius() const
{
	return 0.0f;
}

//============================================================================
// �������擾
//============================================================================
float CObject_X::GetHeight() const
{
	return 0.0f;
}

//============================================================================
// ���[���h�}�g���b�N�X���擾
//============================================================================
const D3DXMATRIX CObject_X::GetMtxWorld() const
{
	return m_MtxWorld;
}

//============================================================================
// ���[���h�}�g���b�N�X��ݒ�
//============================================================================
void CObject_X::SetMtxWorld(D3DXMATRIX Mtx)
{
	m_MtxWorld = Mtx;
}

//============================================================================
// ����
//============================================================================
CObject_X* CObject_X::Create(const std::string& key)
{
	// �C���X�^���X�𐶐�
	CObject_X* pObjectX = DBG_NEW CObject_X();

	// �������s
	if (pObjectX == nullptr)
	{
		assert(false && "X�I�u�W�F�N�g�̐����Ɏ��s���܂���");
	}

	// ���f����ݒ�
	pObjectX->BindModel(CX_Manager::GetInstance()->RefRegistry().BindAtKey(key));

	// X�I�u�W�F�N�g�̏����ݒ�
	pObjectX->Init();

	return pObjectX;
}

//============================================================================
// ����
//============================================================================
CObject_X* CObject_X::Create(LAYER Priority, const std::string& key)
{
	// �C���X�^���X�𐶐�
	CObject_X* pObjectX = DBG_NEW CObject_X(Priority);

	// �������s
	if (pObjectX == nullptr)
	{
		assert(false && "X�I�u�W�F�N�g�̐����Ɏ��s���܂���");
	}

	// ���f����ݒ�
	pObjectX->BindModel(CX_Manager::GetInstance()->RefRegistry().BindAtKey(key));

	// X�I�u�W�F�N�g�̏����ݒ�
	pObjectX->Init();

	return pObjectX;
}

//============================================================================
// ����
//============================================================================
CObject_X* CObject_X::Create(const JSON& Json)
{
	// �p�����[�^���R�s�[���Ă���
	const auto& Priority = static_cast<int>(Json["Priority"]);
	const auto& ModelType = static_cast<std::string>(Json["ModelType"]);
	const auto& Rot = hws::JsonConvertToVec3(Json["Rot"]);
	const auto& Pos = hws::JsonConvertToVec3(Json["Pos"]);
	const auto& Scale = hws::JsonConvertToVec3(Json["Scale"]);

	// �C���X�^���X�𐶐�
	CObject_X* pObjectX = DBG_NEW CObject_X(static_cast<CObject::LAYER>(Priority));

	// �������s
	if (pObjectX == nullptr)
	{
		assert(false && "X�I�u�W�F�N�g�̐����Ɏ��s���܂���");
	}

	// ���f����ݒ�
	pObjectX->BindModel(CX_Manager::GetInstance()->RefRegistry().BindAtKey(ModelType));

	// X�I�u�W�F�N�g�̏����ݒ�
	pObjectX->Init();

	// �e��p�����[�^��ݒ�
	pObjectX->SetRot(Rot);
	pObjectX->SetPos(Pos);
	pObjectX->SetScale(Scale);

	if (Json.contains("VectorInterpolator"))
	{
		if (static_cast<bool>(Json["VectorInterpolator"]))
		{
			CVectorInterpolator::VectorCopyWrapper wrapper =
			{
				hws::JsonConvertToVec3(Json["ScaleTarget"]),
				hws::JsonConvertToVec3(Json["RotTarget"]),
				hws::JsonConvertToVec3(Json["PosTarget"])
			};

			float aRate[3] = { 0.0f, 0.0f, 0.0f };
			unsigned char aFlags[3] = { 0, 0, 0 };

			for (unsigned char ucIdx = 0; ucIdx < 3; ++ucIdx)
			{
				aRate[ucIdx] = static_cast<float>(Json["Rate"][ucIdx]);
#if SWITCH_STARTUP_MODE
				aFlags[ucIdx] = static_cast<unsigned char>(static_cast<VectorInterpolationFlags>(Json["Flags"][ucIdx]) & ~VectorInterpolationFlags::UseInterpolation);
#else				
				aFlags[ucIdx] = static_cast<unsigned char>(Json["Flags"][ucIdx]);
#endif
			}

			Compose(pObjectX->m_upVectorInterpolator,
				[wrapper, aRate, aFlags]() -> decltype(auto)
				{
					up<CVectorInterpolator> uPtr = nullptr;
					uPtr = make_unique<CVectorInterpolator>();
					uPtr->SetScaleTarget(wrapper.Scale);
					uPtr->SetRotTarget(wrapper.Rot);
					uPtr->SetPosTarget(wrapper.Pos);
					for (unsigned char ucCntLoop = 0; ucCntLoop < 3; ++ucCntLoop)
					{
						uPtr->SetRate(ucCntLoop, aRate[ucCntLoop]);
						uPtr->SetFlags(ucCntLoop, static_cast<VectorInterpolationFlags>(aFlags[ucCntLoop]));
					}
					return uPtr;
				});
		}
	}

	return pObjectX;
}

//============================================================================
// ����
//============================================================================
std::unique_ptr<CObject_X> CObject_X::upCreate(const JSON& Json)
{
	// �p�����[�^���R�s�[���Ă���
	const auto& Priority = static_cast<int>(Json["Priority"]);
	const auto& ModelType = static_cast<std::string>(Json["ModelType"]);
	const auto& Rot = hws::JsonConvertToVec3(Json["Rot"]);
	const auto& Pos = hws::JsonConvertToVec3(Json["Pos"]);
	const auto& Scale = hws::JsonConvertToVec3(Json["Scale"]);

	// �C���X�^���X�𐶐�
	std::unique_ptr<CObject_X> pObjectX = std::make_unique<CObject_X>(static_cast<CObject::LAYER>(Priority));

	// �������s
	if (!pObjectX)
	{
		return nullptr;
	}

	// ���f����ݒ�
	pObjectX->BindModel(CX_Manager::GetInstance()->RefRegistry().BindAtKey(ModelType));

	// X�I�u�W�F�N�g�̏����ݒ�
	pObjectX->Init();

	// �e��p�����[�^��ݒ�
	pObjectX->SetRot(Rot);
	pObjectX->SetPos(Pos);
	pObjectX->SetScale(Scale);

	return pObjectX;
}

//============================================================================
// 
// private�����o
// 
//============================================================================

//============================================================================
// ���_�𒲐�����
//============================================================================
void CObject_X::AdjustVtx()
{
	//�ő咸�_�ɃX�P�[���l���|����
	m_VtxMax.x = m_BaseVtxMax.x * m_Scale.x;
	m_VtxMax.y = m_BaseVtxMax.y * m_Scale.y;
	m_VtxMax.z = m_BaseVtxMax.z * m_Scale.z;

	//�ŏ����_�ɃX�P�[���l���|����
	m_VtxMin.x = m_BaseVtxMin.x * m_Scale.x;
	m_VtxMin.y = m_BaseVtxMin.y * m_Scale.y;
	m_VtxMin.z = m_BaseVtxMin.z * m_Scale.z;

	//�T�C�Y�v�Z
	m_Size = (m_VtxMax - m_VtxMin) * 0.5f;
}

//============================================================================
// �����𒲐�����
//============================================================================
void CObject_X::AdjustRotAngle()
{
	while (fabsf(m_Rot.y) > D3DX_PI)
	{
		hws::WrapAngleToRange(m_Rot.y);
	}
}

//============================================================================
// ���[���h�s��ݒ�
//============================================================================
void CObject_X::UpdateMtxWorld()
{
	// �v�Z�p�s��
	D3DXMATRIX mtxScale, mtxRot, mtxTrans;

	// ���[���h�s���������
	D3DXMatrixIdentity(&m_MtxWorld);
	
	// �g�k�s��쐬
	D3DXMatrixScaling(&mtxScale,
		m_Scale.x,
		m_Scale.y,
		m_Scale.z);

	// �g�k�s��Ƃ̊|���Z
	D3DXMatrixMultiply(&m_MtxWorld,
		&m_MtxWorld,
		&mtxScale);

	// ��]�s��쐬
	D3DXMatrixRotationYawPitchRoll(&mtxRot,
		m_Rot.y,
		m_Rot.x,
		m_Rot.z);

	// ��]�s��Ƃ̊|���Z
	D3DXMatrixMultiply(&m_MtxWorld,
		&m_MtxWorld,
		&mtxRot);

	// ���s�ړ��s��쐬
	D3DXMatrixTranslation(&mtxTrans,
		m_Pos.x,
		m_Pos.y,
		m_Pos.z);

	// ���s�ړ��s��Ƃ̊|���Z
	D3DXMatrixMultiply(&m_MtxWorld,
		&m_MtxWorld,
		&mtxTrans);
}