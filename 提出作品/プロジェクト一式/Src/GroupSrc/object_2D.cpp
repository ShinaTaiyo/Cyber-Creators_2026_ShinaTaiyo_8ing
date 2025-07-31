//============================================================================
// 
// 2D�I�u�W�F�N�g [object_2D.cpp]
// Author : ���c����
// 
//============================================================================

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "object_2D.h"
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
// �`��D��x�w��R���X�g���N�^
//============================================================================
CObject_2D::CObject_2D(LAYER Priority) :
	CObject{ Priority },
	m_pVtxBuff{ nullptr },
	m_pTex{ nullptr },
	m_Size{ VEC3_INIT },
	m_Rot{ VEC3_INIT },
	m_Pos{ VEC3_INIT },
	m_Col{ XCOL_INIT },
	m_fLength{ 0.0f },
	m_fAngle{ 0.0f },
	m_TexSize{ VEC2_INIT },
	m_fTexWidth{ 1.0f },
	m_fTexHeight{ 1.0f },
	m_nNowPatternU{ 0 },
	m_nNowPatternV{ 0 }
{

}

//============================================================================
// �f�X�g���N�^
//============================================================================
CObject_2D::~CObject_2D()
{

}

//============================================================================
// �����ݒ�
//============================================================================
HRESULT CObject_2D::Init()
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
void CObject_2D::Uninit()
{
	// ���_�o�b�t�@�̔j��
	if (m_pVtxBuff != nullptr)
	{
		m_pVtxBuff->Release();	// ���_�o�b�t�@�̉��
		m_pVtxBuff = nullptr;	// �|�C���^�̏�����
	}
}

//============================================================================
// �X�V����
//============================================================================
void CObject_2D::Update()
{
	if (m_pVtxBuff == nullptr)
	{ // ���_�o�b�t�@������
		assert(false && "2D�I�u�W�F�N�g�̒��_�o�b�t�@������");
	}

	// �K�v�Ȑ��l���Z�o
	m_fLength = sqrtf(m_Size.x * m_Size.x + m_Size.y * m_Size.y);
	m_fAngle = atan2f(m_Size.x, m_Size.y);

	// ���_���ւ̃|�C���^
	VERTEX_2D* pVtx = nullptr;

	// ���_�o�b�t�@�����b�N
	m_pVtxBuff->Lock(0, 0, reinterpret_cast<void**>(&pVtx), 0);

	// ���_���W�̐ݒ�
	pVtx[0].pos = {
		m_Pos.x + sinf(m_Rot.z - (D3DX_PI - m_fAngle)) * m_fLength,
		m_Pos.y + cosf(m_Rot.z - (D3DX_PI - m_fAngle)) * m_fLength,
		0.0f
	};

	pVtx[1].pos = {
		m_Pos.x + sinf(m_Rot.z + (D3DX_PI - m_fAngle)) * m_fLength,
		m_Pos.y + cosf(m_Rot.z + (D3DX_PI - m_fAngle)) * m_fLength,
		0.0f
	};

	pVtx[2].pos = {
		m_Pos.x + sinf(m_Rot.z - m_fAngle) * m_fLength,
		m_Pos.y + cosf(m_Rot.z - m_fAngle) * m_fLength,
		0.0f
	};

	pVtx[3].pos = {
		m_Pos.x + sinf(m_Rot.z + m_fAngle) * m_fLength,
		m_Pos.y + cosf(m_Rot.z + m_fAngle) * m_fLength,
		0.0f
	};

#if 0
#ifdef _DEBUG

	for (int i = 0; i < 4; ++i)
	{
		CRenderer::SetDebugString("���_���W : " + std::to_string(pVtx[i].pos.x) + " :  " + std::to_string(pVtx[i].pos.y) + " : " + std::to_string(pVtx[i].pos.z));
	}

#endif	// _DEBUG
#endif

	// ���_�F�̐ݒ�
	pVtx[0].col = m_Col;
	pVtx[1].col = m_Col;
	pVtx[2].col = m_Col;
	pVtx[3].col = m_Col;

	// �e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = { m_fTexWidth * m_nNowPatternU, m_fTexHeight * m_nNowPatternV };
	pVtx[1].tex = { m_fTexWidth * (m_nNowPatternU + 1) - m_TexSize.x, m_fTexHeight * m_nNowPatternV };
	pVtx[2].tex = { m_fTexWidth * (m_nNowPatternU), m_fTexHeight * (m_nNowPatternV + 1) };
	pVtx[3].tex = { m_fTexWidth * (m_nNowPatternU + 1) - m_TexSize.x, m_fTexHeight * (m_nNowPatternV + 1) };

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}

//============================================================================
// �`�揈��
//============================================================================
void CObject_2D::Draw()
{
	// �f�o�C�X���擾
	LPDIRECT3DDEVICE9 pDev = CRenderer::GetDeviece();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDev->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDev->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDev->SetTexture(0, m_pTex);

	// �|���S���̕`��
	pDev->DrawPrimitive(D3DPT_TRIANGLESTRIP,	// �v���~�e�B�u�̎��
		0,										// ���_���̐擪�A�h���X
		NUM_PRIM);								// �v���~�e�B�u��
}

//============================================================================
// �e�N�X�`������
//============================================================================
void CObject_2D::BindTex(LPDIRECT3DTEXTURE9 pTex)
{
	m_pTex = pTex;
}

//============================================================================
// �T�C�Y�擾
//============================================================================
const D3DXVECTOR3& CObject_2D::GetSize() const
{
	return m_Size;
}

//============================================================================
// �T�C�Y�ݒ�
//============================================================================
void CObject_2D::SetSize(D3DXVECTOR3 Size)
{
	m_Size = Size;
}

//============================================================================
// �����擾
//============================================================================
const D3DXVECTOR3& CObject_2D::GetRot()const
{
	return m_Rot;
}

//============================================================================
// �����ݒ�
//============================================================================
void CObject_2D::SetRot(D3DXVECTOR3 Rot)
{
	m_Rot = Rot;
}

//============================================================================
// ���W�擾
//============================================================================
const D3DXVECTOR3& CObject_2D::GetPos() const
{
	return m_Pos;
}

//============================================================================
// ���W�ݒ�
//============================================================================
void CObject_2D::SetPos(D3DXVECTOR3 Pos)
{
	m_Pos = Pos;
}

//============================================================================
// �F���擾
//============================================================================
const D3DXCOLOR& CObject_2D::GetCol() const
{
	return m_Col;
}

//============================================================================
// �F��ݒ�
//============================================================================
void CObject_2D::SetCol(D3DXCOLOR Col)
{
	m_Col = Col;
}

//============================================================================
// �A���t�@�l���擾
//============================================================================
const float& CObject_2D::GetAlpha() const
{
	return m_Col.a;
}

//============================================================================
// �A���t�@�l��ݒ�
//============================================================================
void CObject_2D::SetAlpha(float fAlpha)
{
	m_Col.a = fAlpha;
}

//============================================================================
// �W�J�p�Ίp���擾
//============================================================================
const float& CObject_2D::GetLength() const
{
	return m_fLength;
}

//============================================================================
// �e�N�X�`���T�C�Y���擾
//============================================================================
const D3DXVECTOR2& CObject_2D::GetTexSize() const
{
	return m_TexSize;
}

//============================================================================
// �e�N�X�`���T�C�Y��ݒ�
//============================================================================
void CObject_2D::SetTexSize(D3DXVECTOR2 Size)
{
	m_TexSize = Size;
}

//============================================================================
// ���e�N�X�`���������ݒ�
//============================================================================
void CObject_2D::SetTexWidth(float fWidth)
{
	m_fTexWidth = fWidth;
}

//============================================================================
// �c�e�N�X�`���������ݒ�
//============================================================================
void CObject_2D::SetTexHeight(float fHeight)
{
	m_fTexHeight = fHeight;
}

//============================================================================
// ���݂̃e�N�X�`�����������擾
//============================================================================
const int& CObject_2D::GetNowPatternU() const
{
	return m_nNowPatternU;
}

//============================================================================
// ���݂̃e�N�X�`�����������ݒ�
//============================================================================
void CObject_2D::SetNowPatternU(int nNowPatternU)
{
	m_nNowPatternU = nNowPatternU;
}

//============================================================================
// ���݂̃e�N�X�`���c�������擾
//============================================================================
const int& CObject_2D::GetNowPatternV() const
{
	return m_nNowPatternV;
}

//============================================================================
// ���݂̃e�N�X�`���c�������ݒ�
//============================================================================
void CObject_2D::SetNowPatternV(int nNowPatternV)
{
	m_nNowPatternV = nNowPatternV;
}

//============================================================================
// ����
//============================================================================
CObject_2D* CObject_2D::Create()
{
	// �C���X�^���X�𐶐�
	CObject_2D* pNewInstance = DBG_NEW CObject_2D();

	// �����o���Ă����珉���ݒ�
	if (pNewInstance == nullptr)
	{
		assert(false && "2D�I�u�W�F�N�g�̐����Ɏ��s");
	}

	// 2D�I�u�W�F�N�g�̏����ݒ�
	pNewInstance->Init();

	return pNewInstance;
}

//============================================================================
// 
// private�����o
// 
//============================================================================

//============================================================================
// ���_�o�b�t�@�̐���
//============================================================================
HRESULT CObject_2D::CreateVtxBuff()
{
	// �f�o�C�X���擾
	LPDIRECT3DDEVICE9 pDev = CRenderer::GetDeviece();

	// ���_�o�b�t�@�̐���
	pDev->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VTX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		nullptr);

	if (m_pVtxBuff == nullptr)
	{ // �������s
		return E_FAIL;
	}

	// ���_���ւ̃|�C���^
	VERTEX_2D* pVtx = nullptr;

	// ���_�o�b�t�@�����b�N
	m_pVtxBuff->Lock(0, 0, reinterpret_cast<void**>(&pVtx), 0);

	for (WORD wNumVtx = 0; wNumVtx < NUM_VTX; ++wNumVtx)
	{
		// ���_���W�̐ݒ�
		pVtx[wNumVtx].pos = VEC3_INIT;

		// ���Z���̐ݒ�
		pVtx[wNumVtx].rhw = 1.0f;

		// ���_�F�̐ݒ�
		pVtx[wNumVtx].col = XCOL_INIT;

		// �e�N�X�`�����W�̐ݒ�
		pVtx[wNumVtx].tex = VEC2_INIT;
	}

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();

	return S_OK;
}