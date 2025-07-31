//============================================================================
//
// �񎟃x�W�F�Ȑ� [quadratic_bezier.cpp]
// Author : ���c����
//
//============================================================================

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "quadratic_bezier.h"

// �`��p
#include "manager.h"
#include "renderer.h"

// �I�u�W�F�N�g�����p
#include "object_X.h"

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
// �f�t�H���g�R���X�g���N�^
//============================================================================
CQuadratic_Bezier::CQuadratic_Bezier() :
	m_pLineVtx(nullptr),
	m_fRate(0.0f),
	m_fSpeed(DEFAULT_SPEED),
	m_bUseReverse(false),
	m_bIsReverse(false),
	m_bIsEndFirstTime(false),
	m_bDispModel(false)
{
	// ���[���h�s���������
	D3DXMatrixIdentity(&m_MtxWorld);

	// ����_�̏�����
	for (WORD wIdx = 0; wIdx < NUM_CONTROL; ++wIdx)
	{
		m_ControlPoint[wIdx] = VEC3_INIT;
		m_pControlPointModel[wIdx] = nullptr;
	}

	// �ړ��_�̏�����
	for (WORD wIdx = 0; wIdx < NUM_TRACE; ++wIdx)
	{
		m_TracePoint[wIdx] = VEC3_INIT;
		m_pTracePointModel[wIdx] = nullptr;
	}

	// �񎟋Ȑ��O���̏�����
	m_vTrajectoryModel.clear();
}

//============================================================================
// �f�X�g���N�^
//============================================================================
CQuadratic_Bezier::~CQuadratic_Bezier()
{
	// �O�̂��߃N���[���A�b�v
	Finalize();
}

//============================================================================
// ����������
//============================================================================
bool CQuadratic_Bezier::Initialize()
{
	// ���f���\�����s��Ȃ��ꍇ������������̂�����
	if (!m_bDispModel)
	{
		return true;
	}

	// ���_�o�b�t�@�̐���
	if (FAILED(CreateVtxBuff()))
	{
		return false;
	}

	// ����_�����p��JSON�t�@�C����W�J
	const JSON& Json_Control = hws::SafeOpenJsonFile("Data\\JSON\\OBJECT\\X\\ControlPoint.json");

	// �@ - ����_�̐���
	for (WORD wIdx = 0; wIdx < NUM_CONTROL; ++wIdx)
	{
		m_pControlPointModel[wIdx] = CObject_X::Create(Json_Control);
	}

	// �ړ��_�����p��JSON�t�@�C����W�J
	const JSON& Json_Trace = hws::SafeOpenJsonFile("Data\\JSON\\OBJECT\\X\\TracePoint.json");

	// �A - �ړ��_�̐���
	for (WORD wIdx = 0; wIdx < NUM_TRACE; ++wIdx)
	{
		m_pTracePointModel[wIdx] = CObject_X::Create(Json_Trace);
	}

	return true;
}

//============================================================================
// �I������
//============================================================================
void CQuadratic_Bezier::Finalize()
{
	// ���_�o�b�t�@�̔j��
	if (m_pLineVtx != nullptr)
	{
		m_pLineVtx->Release();
		m_pLineVtx = nullptr;
	}

	// ����_�̉���\��
	for (WORD wIdx = 0; wIdx < NUM_CONTROL; ++wIdx)
	{
		if (!m_pControlPointModel[wIdx])
		{
			continue;
		}

		m_pControlPointModel[wIdx]->SetRelease();
	}

	// �ړ��_�̉���\��
	for (WORD wIdx = 0; wIdx < NUM_TRACE; ++wIdx)
	{
		if (!m_pTracePointModel[wIdx])
		{
			continue;
		}

		m_pTracePointModel[wIdx]->SetRelease();
	}

	// �񎟋O�՗p�̑S�Ẵ��f���ɔj���\��
	SetReleaseAllTrajectory();
}

//============================================================================
// �X�V����
//============================================================================
bool CQuadratic_Bezier::Update()
{
	// �i�s�x�̍X�V
	if (UpdateRate())
	{
		return true;
	}

	return false;
}

//============================================================================
// �`�揈��
//============================================================================
void CQuadratic_Bezier::Draw()
{
	// ���f���\�����s��Ȃ��ꍇ�`�揈�����s���Ă͂����Ȃ�
	if (!m_bDispModel)
	{
		return;
	}

	// ���_���W�̍X�V
	UpdateVtxPos();

	// ���[���h�s��̍X�V
	UpdateMtxWorld(m_MtxWorld, SCALE_INIT, VEC3_INIT, VEC3_INIT);

	// �f�o�C�X���擾
	LPDIRECT3DDEVICE9 pDev = CRenderer::GetDeviece();

	// ���C�g���I�t
	pDev->SetRenderState(D3DRS_LIGHTING, FALSE);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDev->SetTransform(D3DTS_WORLD, &m_MtxWorld);

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDev->SetStreamSource(0, m_pLineVtx, 0, sizeof(VERTEX_3D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDev->SetFVF(FVF_VERTEX_3D);

	// �e�N�X�`���̐ݒ�
	pDev->SetTexture(0, nullptr);

	// ���̕`��
	pDev->DrawPrimitive(
		D3DPT_LINESTRIP,	// �v���~�e�B�u�̎��
		0,					// ���_���̐擪�A�h���X
		NUM_CONTROL - 1);	// �v���~�e�B�u��

	// ���C�g���I��
	pDev->SetRenderState(D3DRS_LIGHTING, TRUE);

	// ����ړ����Ɍ���A�O�Ղ�`�����߂̃��f���𐶐�����
	if (!m_bIsEndFirstTime)
	{
		// ���f���ɕK�v�Ȓǉ��p�����[�^�̓W�J
		const JSON& Json_QiadraticTrajectory = hws::SafeOpenJsonFile("Data\\JSON\\OBJECT\\X\\TracePoint.json");

		// ���f���C���X�^���X�𐶐�
		CObject_X* p = CObject_X::Create(Json_QiadraticTrajectory);

		// �������s
		if (!p)
		{
			throw std::runtime_error("Failed to Create QiadraticTrajectory");
		}

		// ���̎��_�ł̈ړ��_�̍��W���A�O�Ղ̍��W�Ƃ��Đݒ肷��
		p->SetPos(m_TracePoint[2]);

		// �O�̂��߁A�z��Ƀ|�C���^���i�[���Ă���
		m_vTrajectoryModel.push_back(p);
	}
}

//============================================================================
// 
// private�����o
// 
//============================================================================

//============================================================================
// ���_�o�b�t�@�𐶐�
//============================================================================
HRESULT CQuadratic_Bezier::CreateVtxBuff()
{
	// �f�o�C�X���擾
	LPDIRECT3DDEVICE9 pDev = CRenderer::GetDeviece();

	// ���_�o�b�t�@�̐���
	pDev->CreateVertexBuffer(
		sizeof(VERTEX_3D) * NUM_CONTROL,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pLineVtx,
		nullptr);

	// �������s
	if (m_pLineVtx == nullptr)
	{
		return E_FAIL;
	}

	// ���_���ւ̃|�C���^
	VERTEX_3D* pVtx = nullptr;

	// ���_�o�b�t�@�����b�N
	m_pLineVtx->Lock(0, 0, reinterpret_cast<void**>(&pVtx), 0);

	for (WORD wIdx = 0; wIdx < NUM_CONTROL; ++wIdx)
	{
		// ���_���W��ݒ�
		pVtx[wIdx].pos = VEC3_INIT;

		// �@���x�N�g���̐ݒ�
		pVtx[wIdx].nor = VEC3_INIT;

		// ���_�F�̐ݒ�
		pVtx[wIdx].col = D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f);

		// �e�N�X�`�����W�̐ݒ�
		pVtx[wIdx].tex = VEC2_INIT;
	}

	// ���_�o�b�t�@���A�����b�N����
	m_pLineVtx->Unlock();

	return S_OK;
}

//============================================================================
// �񎟋O�՗p�̑S�Ẵ��f���ɔj���\��
//============================================================================
void CQuadratic_Bezier::SetReleaseAllTrajectory()
{
	for (const auto& it : m_vTrajectoryModel)
	{
		if (!it)
		{
			continue;
		}

		it->SetRelease();
	}

	// �R���e�i���̂�������
	m_vTrajectoryModel.clear();
}

//============================================================================
// �i�s�x�̍X�V
//============================================================================
bool CQuadratic_Bezier::UpdateRate()
{
	// �I�[�ɒB�����t���O
	bool bIsReachEnd = false;

	// �i�s�x�̕ϓ�
	if (!m_bIsReverse)
	{
		if (m_fRate < 1.0f)
		{
			m_fRate += m_fSpeed;
		}
	}
	else
	{
		if (m_fRate > 0.0f)
		{
			m_fRate += -m_fSpeed;
		}
	}

	/*
		���߂��Ă��܂����ۂ̐��l�������G�ɐ������Ă��܂����c
		�ړ����x���ӂ݂āA�����̒����ƁA���߂��Ă��܂��\��̋�������A
		�ł��Y��ɐi�s���Ă��������ɕ␳���������ǁA�떂�������܂܂ł��ǂ��c
	*/
	if (m_fRate > 1.0f)
	{
		m_fRate = 1.0f;

		if (!m_bUseReverse)
		{
			// �I�[�ɓ��B�����t���O�𗧂Ă�
			bIsReachEnd = true;
		}
		else
		{
			// ���]�t���O�𔽓]����c
			m_bIsReverse = !m_bIsReverse;
		}

		// ����ړ��I���t���O�𗧂Ă�
		if (!m_bIsEndFirstTime)
		{
			m_bIsEndFirstTime = true;
		}
	}
	else if (m_fRate < 0.0f)
	{
		m_fRate = 0.0f;

		// ���]�t���O�𔽓]����c
		m_bIsReverse = !m_bIsReverse;
	}

	// �@ - �i�s�x�ɍ��킹�āA����_�ԏ�̈ړ��_�̍��W���X�V
	for (WORD wIdx = 0; wIdx < NUM_CONTROL - 1; ++wIdx)
	{
		const Vec3& posLinear = Lerp(m_ControlPoint[wIdx], m_ControlPoint[wIdx + 1], m_fRate);
		m_TracePoint[wIdx] = posLinear;
	}

	// �A - �ړ��_���m�����Ԑ���ɁA�����i�s�x�ł���Ȃ�ړ��_��ݒ�
	const Vec3& posQuadratic = Lerp(m_TracePoint[0], m_TracePoint[1], m_fRate);
	m_TracePoint[2] = posQuadratic;

	/* ����_�ƈړ��_�̍��W���o�͂��� */
#if 0
	for (WORD i = 0; i < NUM_CONTROL; ++i)
	{
		CRenderer::SetDebugString(to_string(m_pTracePointModel[i]->GetPos().x) + " : " + to_string(m_pTracePointModel[i]->GetPos().y) + " : " + to_string(m_pTracePointModel[i]->GetPos().z));
	}

	for (WORD i = 0; i < NUM_CONTROL; ++i)
	{
		CRenderer::SetDebugString(to_string(m_ControlPoint[i].x) + " : " + to_string(m_ControlPoint[i].y) + " : " + to_string(m_ControlPoint[i].z));
	}
#endif

	return bIsReachEnd;
}

//============================================================================
// ���_���W�̍X�V
//============================================================================
void CQuadratic_Bezier::UpdateVtxPos()
{
	// ���_���ւ̃|�C���^
	VERTEX_3D* pVtx = nullptr;

	// ���_�o�b�t�@�����b�N
	m_pLineVtx->Lock(0, 0, reinterpret_cast<void**>(&pVtx), 0);

	// ���_���W�𐧌�_�̍��W�ɓ���
	for (WORD wIdx = 0; wIdx < NUM_CONTROL; ++wIdx)
	{
		pVtx[wIdx].pos = m_ControlPoint[wIdx];
	}

	// ���_�o�b�t�@���A�����b�N����
	m_pLineVtx->Unlock();
}