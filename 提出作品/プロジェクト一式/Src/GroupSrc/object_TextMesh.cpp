//============================================================================
// 
// �e�L�X�g���b�V���I�u�W�F�N�g [object_TextMesh.cpp]
// Author : ���c����
// 
//============================================================================

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "object_TextMesh.h"
#include "renderer.h"
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
CObject_TextMesh::CObject_TextMesh(LAYER Priority) :
	CObject{ Priority },
	m_pVtxBuff{ nullptr },
	m_pTex{ nullptr },
	m_pSurface{ nullptr },
	m_Text{},
	m_TextTarget{},
	m_nCntDisp{ 0 },
	m_nTextSpeed{ 0 },
	m_nTextDelay{ 0 },
	m_nTimer{ -1 },
	m_TextSize{ VEC2_INIT },
	m_MeshSize{ VEC3_INIT },
	m_Rot{ VEC3_INIT },
	m_Pos{ VEC3_INIT },
	m_Col{ XCOL_INIT },
	m_fLength{ 0.0f },
	m_fAngle{ 0.0f }
{
	// ���[���h�s��̏�����
	D3DXMatrixIdentity(&m_MtxWorld);
}

//============================================================================
// �f�X�g���N�^
//============================================================================
CObject_TextMesh::~CObject_TextMesh()
{

}

//============================================================================
// �����ݒ�
//============================================================================
HRESULT CObject_TextMesh::Init()
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

	// �e�L�X�g���b�V���^�C�v�ɐݒ�
	SetType(TYPE::TEXTMESH);

	return S_OK;
}

//============================================================================
// �I������
//============================================================================
void CObject_TextMesh::Uninit()
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
void CObject_TextMesh::Update()
{
	// ���Ԍo�߂ŏ���
	if (m_nTimer != -1)
	{
		DisapperByTimer();
	}

	// �e�L�X�g����
	TextAnimation();

	if (m_pVtxBuff == nullptr)
	{ // ���_�o�b�t�@������
		assert(false);
	}

	// �K�v�Ȑ��l���v�Z
	m_fAngle = atan2f(m_MeshSize.x, m_MeshSize.y);
	m_fLength = sqrtf(m_MeshSize.x * m_MeshSize.x + m_MeshSize.y * m_MeshSize.y);

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

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();

	// ���[���h�s��̐ݒ�
	SetMtxWorld();
}

//============================================================================
// �`�揈��
//============================================================================
void CObject_TextMesh::Draw()
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
	pDev->SetTexture(0, m_pTex);

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
// �T�[�t�F�C�X���擾
//============================================================================
LPDIRECT3DSURFACE9 CObject_TextMesh::GetSurface()
{
	return m_pSurface;
}

//============================================================================
// �e�L�X�g���擾
//============================================================================
const std::string& CObject_TextMesh::GetText() const
{
	return m_Text;
}

//============================================================================
// �e�L�X�g��ݒ�
//============================================================================
void CObject_TextMesh::SetText(std::string Text)
{
	m_Text = Text;
}

//============================================================================
// �ڕW�e�L�X�g���擾
//============================================================================
const std::string& CObject_TextMesh::GetTextTarget() const
{
	return m_TextTarget;
}

//============================================================================
// �ڕW�e�L�X�g��ݒ�
//============================================================================
void CObject_TextMesh::SetTextTarget(std::string Text)
{
	m_TextTarget = Text;
}

//============================================================================
// �^�C�}�[��ݒ�
//============================================================================
void CObject_TextMesh::SetTimer(int nTimer)
{
	m_nTimer = nTimer;
}

//============================================================================
// �e�L�X�g�T�C�Y�擾
//============================================================================
const D3DXVECTOR2& CObject_TextMesh::GetTextSize() const
{
	return m_TextSize;
}

//============================================================================
// �e�L�X�g�T�C�Y�ݒ�
//============================================================================
void CObject_TextMesh::SetTextSize(D3DXVECTOR2 Size)
{
	m_TextSize = Size;
}

//============================================================================
// ���b�V���T�C�Y�擾
//============================================================================
const D3DXVECTOR3& CObject_TextMesh::GetMeshSize() const
{
	return m_MeshSize;
}

//============================================================================
// ���b�V���T�C�Y�ݒ�
//============================================================================
void CObject_TextMesh::SetMeshSize(D3DXVECTOR3 Size)
{
	m_MeshSize = Size;
}

//============================================================================
// �����擾
//============================================================================
const D3DXVECTOR3& CObject_TextMesh::GetRot()const
{
	return m_Rot;
}

//============================================================================
// �����ݒ�
//============================================================================
void CObject_TextMesh::SetRot(D3DXVECTOR3 Rot)
{
	m_Rot = Rot;
}

//============================================================================
// ���W�擾
//============================================================================
const D3DXVECTOR3& CObject_TextMesh::GetPos() const
{
	return m_Pos;
}

//============================================================================
// ���W�ݒ�
//============================================================================
void CObject_TextMesh::SetPos(D3DXVECTOR3 Pos)
{
	m_Pos = Pos;
}

//============================================================================
// �F���擾
//============================================================================
const D3DXCOLOR& CObject_TextMesh::GetCol() const
{
	return m_Col;
}

//============================================================================
// �F��ݒ�
//============================================================================
void CObject_TextMesh::SetCol(D3DXCOLOR Col)
{
	m_Col = Col;
}

//============================================================================
// �A���t�@�l���擾
//============================================================================
const float& CObject_TextMesh::GetAlpha() const
{
	return m_Col.a;
}

//============================================================================
// �A���t�@�l��ݒ�
//============================================================================
void CObject_TextMesh::SetAlpha(float fAlpha)
{
	m_Col.a = fAlpha;
}

//============================================================================
// �W�J�p�Ίp���擾
//============================================================================
const float& CObject_TextMesh::GetLength() const
{
	return m_fLength;
}

//============================================================================
// ����
//============================================================================
CObject_TextMesh* CObject_TextMesh::Create(JSON Json)
{
	// �C���X�^���X�𐶐�
	CObject_TextMesh* pNewInstance = DBG_NEW CObject_TextMesh();

	// �������s
	if (pNewInstance == nullptr)
	{
		assert(false && "�e�L�X�g���b�V���I�u�W�F�N�g�̐����Ɏ��s");
	}

	{ // �p�����[�^�̐ݒ�

		// �f�[�^���L���X�g
		std::string
			Text = hws::JsonConvertToSJIS(Json["Text"]);
		int
			nTextSpeed = static_cast<int>(Json["TextSpeed"]),
			nTextDelay = static_cast<int>(Json["TextDelay"]),
			nInitTextDelay = static_cast<int>(Json["InitTextDelay"]),
			nTimer = static_cast<int>(Json["Timer"]);
		Vec2
			TextSize = hws::JsonConvertToVec2(Json["TextSize"]);
		Vec3
			MeshSize = hws::JsonConvertToVec3(Json["MeshSize"]),
			Rot = hws::JsonConvertToVec3(Json["Rot"]),
			Pos = hws::JsonConvertToVec3(Json["Pos"]);
		XCol
			Col = hws::JsonConvertToXCol(Json["Col"]);

		// �f�[�^���Z�b�g
		pNewInstance->m_TextTarget = Text;			// �ڕW�e�L�X�g
		pNewInstance->m_nTextSpeed = nTextSpeed;	// �e�L�X�g�X�s�[�h
		pNewInstance->m_nTextDelay = nTextDelay;	// �e�L�X�g�f�B���C
		pNewInstance->m_nCntDisp = nInitTextDelay;	// ����e�L�X�g�f�B���C
		pNewInstance->m_nTimer = nTimer;			// �^�C�}�[
		pNewInstance->m_TextSize = TextSize;		// �e�L�X�g�T�C�Y
		pNewInstance->m_MeshSize = MeshSize;		// ���b�V���T�C�Y
		pNewInstance->m_Rot = Rot;					// ����
		pNewInstance->m_Pos = Pos;					// ���W
		pNewInstance->m_Col = Col;					// �F
	}
	
	// �e�L�X�g���b�V���̏����ݒ�
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
HRESULT CObject_TextMesh::CreateVtxBuff()
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
// �e�N�X�`���̐���
//============================================================================
HRESULT CObject_TextMesh::CreateTex()
{
	// �f�o�C�X���擾
	auto pDev = CRenderer::GetDeviece();

	// �e�N�X�`�����쐬
	HRESULT hr = pDev->CreateTexture(
		static_cast<UINT>(m_TextSize.x),	// U
		static_cast<UINT>(m_TextSize.y),	// V
		0,									// �~�b�v�}�b�v���x��
		D3DUSAGE_RENDERTARGET,				// �e�N�X�`���̐���
		D3DFMT_A8R8G8B8,					// �s�N�Z���t�H�[�}�b�g
		D3DPOOL_DEFAULT,					// �������Ǘ��t���O
		&m_pTex,							// �e�N�X�`���ۑ���
		nullptr);							// �n���h��

	if (FAILED(hr))
	{ // �������s
		return E_FAIL;
	}

	// �e�N�X�`���̃T�[�t�F�C�X����ێ�
	m_pTex->GetSurfaceLevel(0, &m_pSurface);

	return S_OK;
}

//============================================================================
// ���Ԍo�߂ɂ�����
//============================================================================
void CObject_TextMesh::DisapperByTimer()
{
	// �^�C�}�[���f�N�������g
	--m_nTimer;

	// �^�C�}�[���[����
	if (m_nTimer <= 0)
	{
		// �e�L�X�g��j��
		SetRelease();
	}
}

//============================================================================
// �e�L�X�g����
//============================================================================
void CObject_TextMesh::TextAnimation()
{
	// �e�L�X�g���S�ĕ\������Ă����珈�������Ȃ�
	if (m_Text == m_TextTarget)
		return;
	
	// �e�L�X�g����J�E���g���C���N�������g
	++m_nCntDisp;

	// �e�L�X�g����J�E���g���K��l�ɒB������
	if (m_nCntDisp > m_nTextSpeed)
	{
		// �e�L�X�g����J�E���g�����Z�b�g
		m_nCntDisp = 0;

		// �ڕW�e�L�X�g����1�����؂蕪����
		std::string SjisChar = m_TextTarget.substr(m_Text.size(), 2);

		// ���s�V���{���ŕ����u������
		if (SjisChar == "\n#")
		{
			// ���s�V���{���̏ꏊ���R�s�[
			WORD wReturnPos = static_cast<WORD>(m_TextTarget.find("\n#", 0));

			// ���K�̉��s�����ɒu��
			m_TextTarget.replace(wReturnPos, 2, "\n");

			// �؂蕪�����������u��
			SjisChar = "\n";

			// �e�L�X�g����J�E���g�Ƀf�B���C
			m_nCntDisp = m_nTextDelay;
		}

		// �e�L�X�g��1�����ǉ�
		m_Text += SjisChar;

		// �e�L�X�gSE��炷
		CSound::GetInstance()->Play(CSound::LABEL::TEXT);
	}
}

//============================================================================
// ���[���h�s��ݒ�
//============================================================================
void CObject_TextMesh::SetMtxWorld()
{
	// �v�Z�p�s��
	D3DXMATRIX mtxRot, mtxTrans;

	// ���[���h�s���������
	D3DXMatrixIdentity(&m_MtxWorld);

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