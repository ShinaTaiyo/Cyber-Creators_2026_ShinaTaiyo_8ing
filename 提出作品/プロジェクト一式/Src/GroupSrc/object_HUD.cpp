//============================================================================
// 
// HUD�I�u�W�F�N�g [object_HUD.cpp]
// Author : ���c����
// 
//============================================================================

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "object_HUD.h"
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
CObject_HUD::CObject_HUD(LAYER Priority) :
	CObject_2D{ Priority },
	m_fCorrectionCoef{ 0.0f },
	m_PosTarget{ VEC3_INIT },
	m_RotTarget{ VEC3_INIT },
	m_SizeTarget{ VEC3_INIT },
	m_ColTarget{ XCOL_INIT }
{

}

//============================================================================
// �f�X�g���N�^
//============================================================================
CObject_HUD::~CObject_HUD()
{

}

//============================================================================
// �����ݒ�
//============================================================================
HRESULT CObject_HUD::Init()
{
	// 2D�I�u�W�F�N�g�̏����ݒ�
	if (FAILED(CObject_2D::Init()))
	{
		return E_FAIL;
	}

	return S_OK;
}

//============================================================================
// �I������
//============================================================================
void CObject_HUD::Uninit()
{
	// 2D�I�u�W�F�N�g�̏I������
	CObject_2D::Uninit();
}

//============================================================================
// �X�V����
//============================================================================
void CObject_HUD::Update()
{
	// �ڕW�l�ւ̕␳
	CorrectToTarget();

	// 2D�I�u�W�F�N�g�̍X�V����
	CObject_2D::Update();
}

//============================================================================
// �`�揈��
//============================================================================
void CObject_HUD::Draw()
{
	// 2D�I�u�W�F�N�g�̕`�揈��
	CObject_2D::Draw();
}

//============================================================================
// �U����^����
//============================================================================
void CObject_HUD::SetVibration()
{
	Vec3 NewPos = GetPos();
	NewPos.x += hws::GetRandomValue<float>() * 0.05f;
	NewPos.y += hws::GetRandomValue<float>() * 0.05f;
	SetPos(NewPos);
}

//============================================================================
// �g�ł���^����
//============================================================================
void CObject_HUD::SetWaving()
{
	Vec3 NewSize = GetSize();
	NewSize.x *= 1.2f;
	NewSize.y *= 0.8f;
	SetSize(NewSize);
}

//============================================================================
// �␳�W���ݒ�
//============================================================================
void CObject_HUD::SetCorrectionCoef(float fCorrectioncoef)
{
	m_fCorrectionCoef = fCorrectioncoef;
}

//============================================================================
// �ڕW���W�擾
//============================================================================
const D3DXVECTOR3& CObject_HUD::GetPosTarget() const
{
	return m_PosTarget;
}

//============================================================================
// �ڕW���W�ݒ�
//============================================================================
void CObject_HUD::SetPosTarget(D3DXVECTOR3 PosTarget)
{
	m_PosTarget = PosTarget;
}

//============================================================================
// �ڕW�����擾
//============================================================================
const D3DXVECTOR3& CObject_HUD::GetRotTarget() const
{
	return m_RotTarget;
}

//============================================================================
// �ڕW�����ݒ�
//============================================================================
void CObject_HUD::SetRotTarget(D3DXVECTOR3 RotTarget)
{
	m_RotTarget = RotTarget;
}

//============================================================================
// �ڕW�T�C�Y�擾
//============================================================================
const D3DXVECTOR3& CObject_HUD::GetSizeTarget() const
{
	return m_SizeTarget;
}

//============================================================================
// �ڕW�T�C�Y�ݒ�
//============================================================================
void CObject_HUD::SetSizeTarget(D3DXVECTOR3 SizeTarget)
{
	m_SizeTarget = SizeTarget;
}

//============================================================================
// �ڕW�F�擾
//============================================================================
const D3DXCOLOR& CObject_HUD::GetColTarget() const
{
	return m_ColTarget;
}

//============================================================================
// �ڕW�F�ݒ�
//============================================================================
void CObject_HUD::SetColTarget(D3DXCOLOR ColTarget)
{
	m_ColTarget = ColTarget;
}

//============================================================================
// ����
//============================================================================
CObject_HUD* CObject_HUD::Create(JSON Json)
{
	// �C���X�^���X�𐶐�
	CObject_HUD* pNewInstance = DBG_NEW CObject_HUD();

	// �����o���Ă����珉���ݒ�
	if (pNewInstance == nullptr)
	{
		assert(false && "HUD�I�u�W�F�N�g�̐����Ɏ��s");
	}

	// �����ݒ�
	pNewInstance->Init();

	{ // �p�����[�^��ݒ�

		// �W�F�C�\���f�[�^�̃G�C���A�X
		JSON
			ColData = Json["Col"],
			ColTargetData = Json["ColTarget"];

		// �f�[�^���L���X�g
		float
			fCorrectCoef = static_cast<float>(Json["CorrectionCoef"]);
		Vec3
			Size = hws::JsonConvertToVec3(Json["Size"]),
			SizeTarget = hws::JsonConvertToVec3(Json["SizeTarget"]),
			Rot = hws::JsonConvertToVec3(Json["Rot"]),
			RotTarget = hws::JsonConvertToVec3(Json["RotTarget"]),
			Pos = hws::JsonConvertToVec3(Json["Pos"]),
			PosTarget = hws::JsonConvertToVec3(Json["PosTarget"]);
		XCol
			Col = hws::JsonConvertToXCol(Json["Col"]),
			ColTarget = hws::JsonConvertToXCol(Json["ColTarget"]);

		// �f�[�^���Z�b�g
		pNewInstance->SetCorrectionCoef(fCorrectCoef);	// ��ԋ��x
		pNewInstance->SetSize(Size);					// �T�C�Y
		pNewInstance->SetSizeTarget(SizeTarget);		// �ڕW�T�C�Y
		pNewInstance->SetRot(Rot);						// ����
		pNewInstance->SetRotTarget(RotTarget);			// �ڕW����
		pNewInstance->SetPos(Pos);						// ���W
		pNewInstance->SetPosTarget(PosTarget);			// �ڕW���W
		pNewInstance->SetCol(Col);						// �F
		pNewInstance->SetColTarget(ColTarget);			// �ڕW�F
	}

	return pNewInstance;
}

//============================================================================
// 
// private�����o
// 
//============================================================================

//============================================================================
// �ڕW�l�ւ̕␳
//============================================================================
void CObject_HUD::CorrectToTarget()
{
	// �ڕW���W�ֈړ�
	Vec3 NowPos = GetPos();
	NowPos += (m_PosTarget - NowPos) * m_fCorrectionCoef;
	SetPos(NowPos);

	// �ڕW�����֕␳
	Vec3 NowRot = GetRot();
	hws::WrapAngleToRange(m_RotTarget.y, NowRot.y);	// �����͈̔͂̕␳
	NowRot += (m_RotTarget - NowRot) * m_fCorrectionCoef;
	SetRot(NowRot);

	// �ڕW�T�C�Y�֕␳
	Vec3 NowSize = GetSize();
	NowSize += (m_SizeTarget - NowSize) * m_fCorrectionCoef;
	SetSize(NowSize);

	// �ڕW�F�␳
	XCol NowCol = GetCol();
	NowCol += (m_ColTarget - NowCol) * m_fCorrectionCoef;
	SetCol(NowCol);
}