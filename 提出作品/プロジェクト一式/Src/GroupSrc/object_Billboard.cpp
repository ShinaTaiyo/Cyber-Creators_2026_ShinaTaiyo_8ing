//============================================================================
// 
// �r���{�[�h�I�u�W�F�N�g [object_Billboard.cpp]
// Author : ���c����
// 
//============================================================================

/* �폜�\�� */
int op = D3DBLENDOP_ADD, src = D3DBLEND_BOTHSRCALPHA, dest = D3DBLEND_INVSRCALPHA;

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "object_billboard.h"

// �f�o�C�X�擾�p
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
CObject_Billboard::CObject_Billboard(LAYER Priority) :
	CObject{ Priority },
	m_pVtxBuff{ nullptr },
	m_pTex{ nullptr },
	m_Size{ VEC3_INIT },
	m_Rot{ VEC3_INIT },
	m_Pos{ VEC3_INIT },
	m_Col{ XCOL_INIT },
	m_fLength{ 0.0f },
	m_fAngle{ 0.0f },
	m_fTexWidth{ 1.0f },
	m_fTexHeight{ 1.0f },
	m_nNowPatternU{ 0 },
	m_nNowPatternV{ 0 }
{
	D3DXMatrixIdentity(&m_MtxWorld);	// ���[���h�s��
}

//============================================================================
// �f�X�g���N�^
//============================================================================
CObject_Billboard::~CObject_Billboard()
{

}

//============================================================================
// �����ݒ�
//============================================================================
HRESULT CObject_Billboard::Init()
{
	// ���_�o�b�t�@�̐���
	if (FAILED(CreateVtxBuff()))
	{
		return E_FAIL;
	}

	return S_OK;
}

//============================================================================
// �I������
//============================================================================
void CObject_Billboard::Uninit()
{
	// ���_�o�b�t�@�̔j��
	if (m_pVtxBuff != nullptr)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}
}

//============================================================================
// �X�V����
//============================================================================
void CObject_Billboard::Update()
{
	if (m_pVtxBuff == nullptr)
	{ // ���_�o�b�t�@������
		assert(false);
	}

	// �K�v�Ȑ��l���v�Z
	m_fAngle = atan2f(m_Size.x, m_Size.y);
	m_fLength = sqrtf(m_Size.x * m_Size.x + m_Size.y * m_Size.y);

	// ���_���ւ̃|�C���^
	VERTEX_3D* pVtx = nullptr;

	// ���_�o�b�t�@�����b�N
 	m_pVtxBuff->Lock(0, 0, reinterpret_cast<void**>(&pVtx), 0);

	// ���_���W�̐ݒ�
	pVtx[0].pos = {
		sinf(m_Rot.z - (D3DX_PI - m_fAngle)) * m_fLength,
		-cosf(m_Rot.z - (D3DX_PI - m_fAngle)) * m_fLength,
		0.0f
	};

	pVtx[1].pos = {
		sinf(m_Rot.z + (D3DX_PI - m_fAngle)) * m_fLength,
		-cosf(m_Rot.z + (D3DX_PI - m_fAngle)) * m_fLength,
		0.0f
	};

	pVtx[2].pos = {
		sinf(m_Rot.z - m_fAngle) * m_fLength,
		-cosf(m_Rot.z - m_fAngle) * m_fLength,
		0.0f
	};

	pVtx[3].pos = {
		sinf(m_Rot.z + m_fAngle) * m_fLength,
		-cosf(m_Rot.z + m_fAngle) * m_fLength,
		0.0f
	};

	// ���_�F�̐ݒ�
	for (WORD wCntVtx = 0; wCntVtx < NUM_VTX; ++wCntVtx)
	{
		pVtx[wCntVtx].col = m_Col;
	}

	// �e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = { m_fTexWidth * m_nNowPatternU, m_fTexHeight * m_nNowPatternV };
	pVtx[1].tex = { m_fTexWidth * (m_nNowPatternU + 1), m_fTexHeight * m_nNowPatternV };
	pVtx[2].tex = { m_fTexWidth * (m_nNowPatternU), m_fTexHeight * (m_nNowPatternV + 1) };
	pVtx[3].tex = { m_fTexWidth * (m_nNowPatternU + 1), m_fTexHeight * (m_nNowPatternV + 1) };

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();

	// ���[���h�s��̐ݒ�
	SetMtxWorld();

#if 0	/* �폜�\�� */
	ImGui::SetNextWindowSize({ -1, -1 });
	ImGui::SetNextWindowPos({ 0, 0 }, ImGuiCond_FirstUseEver);
	if (ImGui::Begin("Alpha Blend")) {
		ImGui::InputInt("op", &op);
		ImGui::InputInt("src", &src);
		ImGui::InputInt("dest", &dest);
		ImGui::End();
	}
#endif
}

//============================================================================
// �`�揈��
//============================================================================
void CObject_Billboard::Draw()
{
	// �f�o�C�X���擾
	LPDIRECT3DDEVICE9 pDev = CRenderer::GetDeviece();

	//// �A���t�@�e�X�g��L���ɂ���
	//pDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);

	//// �A���t�@�u�����h�̐ݒ��ύX
	//pDev->SetRenderState(D3DRS_BLENDOPALPHA, op);
	//pDev->SetRenderState(D3DRS_SRCBLEND, src);
	//pDev->SetRenderState(D3DRS_DESTBLEND, dest);

	//// ���C�g���f�𖳌��ɂ���
	//pDev->SetRenderState(D3DRS_LIGHTING, FALSE);

	// ���[���h�s��̐ݒ�
	pDev->SetTransform(D3DTS_WORLD, &m_MtxWorld);

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDev->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDev->SetFVF(FVF_VERTEX_3D);

	// �e�N�X�`���̐ݒ�
	pDev->SetTexture(0, m_pTex);

	// �|���S���̕`��
	pDev->DrawPrimitive(D3DPT_TRIANGLESTRIP,	// �v���~�e�B�u�̎��
		0,										// ���_���̐擪�A�h���X
		NUM_PRIM);								// �v���~�e�B�u��

	//// ���C�g���f��L���ɂ���
	//pDev->SetRenderState(D3DRS_LIGHTING, TRUE);

	//// �A���t�@�u�����h�̐ݒ����{�ɖ߂�
	//pDev->SetRenderState(D3DRS_BLENDOPALPHA, D3DBLENDOP_ADD);
	//pDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	//pDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//// �A���t�@�e�X�g�𖳌��ɂ���
	//pDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

//============================================================================
// �e�N�X�`������
//============================================================================
void CObject_Billboard::BindTex(LPDIRECT3DTEXTURE9 pTex)
{
	m_pTex = pTex;
}

//============================================================================
// �T�C�Y�擾
//============================================================================
const D3DXVECTOR3& CObject_Billboard::GetSize() const
{
	return m_Size;
}

//============================================================================
// �T�C�Y�ݒ�
//============================================================================
void CObject_Billboard::SetSize(D3DXVECTOR3 Size)
{
	m_Size = Size;
}

//============================================================================
// �����擾
//============================================================================
const D3DXVECTOR3& CObject_Billboard::GetRot()const
{
	return m_Rot;
}

//============================================================================
// �����ݒ�
//============================================================================
void CObject_Billboard::SetRot(D3DXVECTOR3 Rot)
{
	m_Rot = Rot;
}

//============================================================================
// ���W�擾
//============================================================================
const D3DXVECTOR3& CObject_Billboard::GetPos() const
{
	return m_Pos;
}

//============================================================================
// ���W�ݒ�
//============================================================================
void CObject_Billboard::SetPos(D3DXVECTOR3 Pos)
{
	m_Pos = Pos;
}

//============================================================================
// �F���擾
//============================================================================
const D3DXCOLOR& CObject_Billboard::GetCol() const
{
	return m_Col;
}

//============================================================================
// �F��ݒ�
//============================================================================
void CObject_Billboard::SetCol(D3DXCOLOR Col)
{
	m_Col = Col;
}

//============================================================================
// �A���t�@�l���擾
//============================================================================
const float& CObject_Billboard::GetAlpha() const
{
	return m_Col.a;
}

//============================================================================
// �A���t�@�l��ݒ�
//============================================================================
void CObject_Billboard::SetAlpha(float fAlpha)
{
	m_Col.a = fAlpha;
}

//============================================================================
// �W�J�p�Ίp���擾
//============================================================================
const float& CObject_Billboard::GetLength() const
{
	return m_fLength;
}

//============================================================================
// ���e�N�X�`���������ݒ�
//============================================================================
void CObject_Billboard::SetTexWidth(float fWidth)
{
	m_fTexWidth = fWidth;
}

//============================================================================
// �c�e�N�X�`���������ݒ�
//============================================================================
void CObject_Billboard::SetTexHeight(float fHeight)
{
	m_fTexHeight = fHeight;
}

//============================================================================
// ���݂̃e�N�X�`�����������擾
//============================================================================
const int& CObject_Billboard::GetNowPatternU() const
{
	return m_nNowPatternU;
}

//============================================================================
// ���݂̃e�N�X�`�����������ݒ�
//============================================================================
void CObject_Billboard::SetNowPatternU(int nNowPatternU)
{
	m_nNowPatternU = nNowPatternU;
}

//============================================================================
// ���݂̃e�N�X�`���c�������擾
//============================================================================
const int& CObject_Billboard::GetNowPatternV() const
{
	return m_nNowPatternV;
}

//============================================================================
// ���݂̃e�N�X�`���c�������ݒ�
//============================================================================
void CObject_Billboard::SetNowPatternV(int nNowPatternV)
{
	m_nNowPatternV = nNowPatternV;
}

//============================================================================
// ����
//============================================================================
CObject_Billboard* CObject_Billboard::Create()
{
	CObject_Billboard* pObject3D = DBG_NEW CObject_Billboard{};

	// �����o���Ă����珉���ݒ�
	if (pObject3D != nullptr)
	{
		pObject3D->Init();
	}

	return pObject3D;
}

//============================================================================
// 
// private�����o
// 
//============================================================================

//============================================================================
// ���_�o�b�t�@�̐���
//=========================================================================
HRESULT CObject_Billboard::CreateVtxBuff()
{
	// �f�o�C�X���擾
	LPDIRECT3DDEVICE9 pDev = CRenderer::GetDeviece();

	// ���_�o�b�t�@�̐���
	pDev->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VTX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		nullptr);

	if (m_pVtxBuff == nullptr)
	{ // �������s
		return E_FAIL;
	}

	// ���_���ւ̃|�C���^
	VERTEX_3D* pVtx = nullptr;

	// ���_�o�b�t�@�����b�N
	m_pVtxBuff->Lock(0, 0, reinterpret_cast<void**>(&pVtx), 0);

	for (WORD wNumVtx = 0; wNumVtx < NUM_VTX; ++wNumVtx)
	{
		// ���_���W�̐ݒ�
		pVtx[wNumVtx].pos = { VEC3_INIT };

		// �@���x�N�g���̐ݒ�
		pVtx[wNumVtx].nor = { VEC3_INIT };

		// ���_�F�̐ݒ�
		pVtx[wNumVtx].col = XCOL_INIT;
	}

	// �e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = { 0.0f, 0.0f };
	pVtx[1].tex = { 1.0f, 0.0f };
	pVtx[2].tex = { 0.0f, 1.0f };
	pVtx[3].tex = { 1.0f, 1.0f };

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();

	return S_OK;
}

//============================================================================
// ���[���h�s��ݒ�
//============================================================================
void CObject_Billboard::SetMtxWorld()
{
	LPDIRECT3DDEVICE9 pDev = CRenderer::GetDeviece();

	// �v�Z�p�s��
	D3DXMATRIX mtxRot{}, mtxTrans{}, mtxView{};

	// ���[���h�s���������
	D3DXMatrixIdentity(&m_MtxWorld);

	// �r���[�s��擾
	pDev->GetTransform(D3DTS_VIEW, &mtxView);

	// �|���S�����J�����̐��ʂɌ���������
	D3DXMatrixInverse(&m_MtxWorld, nullptr, &mtxView);

	// �t�s������߂�
	m_MtxWorld._41 = 0.0f;
	m_MtxWorld._42 = 0.0f;
	m_MtxWorld._43 = 0.0f;

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