//============================================================================
// 
// �e�N�X�`���I�u�W�F�N�g [object_Texture.cpp]
// Author : ���c����
// 
//============================================================================

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "object_Texture.h"

// �f�o�C�X�擾�p
#include "renderer.h"

// �T�E���h�擾�p
#include "sound.h"

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
CObject_Texture::CObject_Texture(LAYER Priority) :
	CObject(Priority),
	m_pVtxBuff(nullptr),
	m_pTexture(nullptr),
	m_pSurface(nullptr),
	m_TextureSize(VEC2_INIT),
	m_TextureMove(VEC2_INIT),
	m_MeshSize(VEC2_INIT),
	m_MeshSizeTarget(VEC2_INIT),
	m_Rot(VEC3_INIT),
	m_RotTarget(VEC3_INIT),
	m_Pos(VEC3_INIT),
	m_PosTarget(VEC3_INIT),
	m_Col(XCOL_INIT),
	m_ColTarget(XCOL_INIT),
	m_fLength(0.0f),
	m_fAngle(0.0f)
{
	// ���[���h�s��̏�����
	D3DXMatrixIdentity(&m_MtxWorld);
}

//============================================================================
// �f�X�g���N�^
//============================================================================
CObject_Texture::~CObject_Texture()
{
	// �O�̂��߃N���[���A�b�v
	Uninit();
}

//============================================================================
// �����ݒ�
//============================================================================
HRESULT CObject_Texture::Init()
{
	// ���_�o�b�t�@�̐���
	if (FAILED(CreateVtxBuff()))
	{
		return E_FAIL;
	}

	// �e�N�X�`���̐���
	if (FAILED(CreateTex()))
	{
		return E_FAIL;
	}

	// �e�N�X�`���^�C�v�ɐݒ�
	SetType(TYPE::TEXTURE);

	return S_OK;
}

//============================================================================
// �I������
//============================================================================
void CObject_Texture::Uninit()
{
	// ���_�o�b�t�@�̔j��
	if (m_pVtxBuff != nullptr)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}

	// �e�N�X�`���̔j��
	if (m_pTexture)
	{
		m_pTexture->Release();
		m_pTexture = nullptr;
	}
}

//============================================================================
// �X�V����
//============================================================================
void CObject_Texture::Update()
{
	// ���_�o�b�t�@������
	if (!m_pVtxBuff)
	{ 
		throw std::runtime_error("'CObject_Texture::Update()' was Failed");
	}

	// �ڕW�l�ւ̕��
	CorrectToTarget();

	// �K�v�Ȑ��l���v�Z
	m_fAngle = atan2f(m_MeshSize.x, m_MeshSize.y);
	m_fLength = sqrtf(m_MeshSize.x * m_MeshSize.x + m_MeshSize.y * m_MeshSize.y);

	// ���_���ւ̃|�C���^
	VERTEX_3D* pVtx = nullptr;

	// ���_�o�b�t�@�����b�N
	m_pVtxBuff->Lock(0, 0, reinterpret_cast<void**>(&pVtx), 0);

	{ // ���_���W�̐ݒ�

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
	}

	// ���_�F�̐ݒ�
	for (WORD wCntVtx = 0; wCntVtx < NUM_VTX; ++wCntVtx)
	{
		pVtx[wCntVtx].col = m_Col;
	}

	// �e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = { 0.0f + m_TextureMove.x, 0.0f + m_TextureMove.y };
	pVtx[1].tex = { 1.0f + m_TextureMove.x, 0.0f + m_TextureMove.y };
	pVtx[2].tex = { 0.0f + m_TextureMove.x, 1.0f + m_TextureMove.y };
	pVtx[3].tex = { 1.0f + m_TextureMove.x, 1.0f + m_TextureMove.y };

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();

	// ���[���h�s��̐ݒ�
	UpdateMtxWorld(m_MtxWorld, VEC3_INIT, m_Rot, m_Pos);
}

//============================================================================
// �`�揈��
//============================================================================
void CObject_Texture::Draw()
{
	// �f�o�C�X���擾
	LPDIRECT3DDEVICE9 pDev = CRenderer::GetDeviece();

	// �A���t�@�e�X�g��L���ɂ���
	pDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDev->SetRenderState(D3DRS_ALPHAREF, 0);
	pDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	// ���C�g���f�𖳌��ɂ���
	pDev->SetRenderState(D3DRS_LIGHTING, FALSE);

	// ���[���h�s��̐ݒ�
	pDev->SetTransform(D3DTS_WORLD, &m_MtxWorld);

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDev->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDev->SetFVF(FVF_VERTEX_3D);

	// �e�N�X�`���̐ݒ�
	pDev->SetTexture(0, m_pTexture);

	// �|���S���̕`��
	pDev->DrawPrimitive(D3DPT_TRIANGLESTRIP,	// �v���~�e�B�u�̎��
		0,										// ���_���̐擪�A�h���X
		NUM_PRIM);								// �v���~�e�B�u��

	// ���C�g���f��L���ɂ���
	pDev->SetRenderState(D3DRS_LIGHTING, TRUE);

	// �A���t�@�e�X�g�𖳌��ɂ���
	pDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

//============================================================================
// ����
//============================================================================
CObject_Texture* CObject_Texture::Create(const JSON& json)
{
	// �C���X�^���X�𐶐�
	CObject_Texture* pNewInstance = DBG_NEW CObject_Texture();

	// �������s
	if (!pNewInstance)
	{
		throw std::runtime_error("'CObject_Texture::Create(const JSON& json)' was Failed");
	}

	{ // �p�����[�^�̐ݒ�

		// �f�[�^���L���X�g
		float
			DecayRate = static_cast<float>(json["DecayRate"]);
		Vec2
			TextureSize = hws::JsonConvertToVec2(json["TextureSize"]),
			TextureMove = hws::JsonConvertToVec2(json["TextureMove"]),
			MeshSize = hws::JsonConvertToVec2(json["MeshSize"]),
			MeshSizeTarget = hws::JsonConvertToVec2(json["MeshSizeTarget"]);
		Vec3
			Rot = hws::JsonConvertToVec3(json["Rot"]),
			RotTarget = hws::JsonConvertToVec3(json["RotTarget"]),
			Pos = hws::JsonConvertToVec3(json["Pos"]),
			PosTarget = hws::JsonConvertToVec3(json["PosTarget"]);
		XCol
			Col = hws::JsonConvertToXCol(json["Col"]),
			ColTarget = hws::JsonConvertToXCol(json["ColTarget"]);

		// �f�[�^���Z�b�g
		pNewInstance->m_fDecayRate = DecayRate;				// ��������
		pNewInstance->m_TextureSize = TextureSize;			// �e�N�X�`���T�C�Y
		pNewInstance->m_TextureMove = TextureMove;			// �e�N�X�`�����[�u
		pNewInstance->m_MeshSize = MeshSize;				// ���b�V���T�C�Y
		pNewInstance->m_MeshSizeTarget = MeshSizeTarget;	// �ڕW���b�V���T�C�Y
		pNewInstance->m_Rot = Rot;							// ����
		pNewInstance->m_RotTarget = RotTarget;				// �ڕW����
		pNewInstance->m_Pos = Pos;							// ���W
		pNewInstance->m_PosTarget = PosTarget;				// �ڕW���W
		pNewInstance->m_Col = Col;							// �F
		pNewInstance->m_ColTarget = ColTarget;				// �ڕW�F
	}

	// �e�L�X�g���b�V���̏���������
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
HRESULT CObject_Texture::CreateVtxBuff()
{
	// �f�o�C�X���擾
	LPDIRECT3DDEVICE9 pDev = CRenderer::GetDeviece();

	// ���_�o�b�t�@�̐���
	pDev->CreateVertexBuffer(
		sizeof(VERTEX_3D) * NUM_VTX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		nullptr);
	
	// �������s
	if (!m_pVtxBuff)
	{
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

	// �e�N�X�`�����W�̏�����
	InitializeTextureMappingInVertex(pVtx);

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();

	return S_OK;
}

//============================================================================
// �e�N�X�`���̐���
//============================================================================
HRESULT CObject_Texture::CreateTex()
{
	// �f�o�C�X���擾
	auto pDev = CRenderer::GetDeviece();

	// �e�N�X�`�����쐬
	HRESULT hr = pDev->CreateTexture(
		static_cast<UINT>(m_TextureSize.x),	// U
		static_cast<UINT>(m_TextureSize.y),	// V
		0,									// �~�b�v�}�b�v���x��
		D3DUSAGE_RENDERTARGET,				// �e�N�X�`���̐���
		D3DFMT_A8R8G8B8,					// �s�N�Z���t�H�[�}�b�g
		D3DPOOL_DEFAULT,					// �������Ǘ��t���O
		&m_pTexture,						// �e�N�X�`���ۑ���
		nullptr);							// �n���h��

	// �������s
	if (FAILED(hr))
	{
		return E_FAIL;
	}

	// �e�N�X�`���̃T�[�t�F�C�X����ێ�
	m_pTexture->GetSurfaceLevel(0, &m_pSurface);

	return S_OK;
}

//============================================================================
// �ڕW�l�ւ̕��
//============================================================================
void CObject_Texture::CorrectToTarget()
{
	// ��Ԃ̋��x
	const float fCoef = 0.05f;

	// ���b�V���T�C�Y
	ExponentialDecay(m_MeshSize, m_MeshSizeTarget, fCoef);

	// ����
	ExponentialDecay(m_Rot, m_RotTarget, fCoef);

	// ���W
	ExponentialDecay(m_Pos, m_PosTarget, fCoef);

	// �F
	ExponentialDecay(m_Col, m_ColTarget, fCoef);
}