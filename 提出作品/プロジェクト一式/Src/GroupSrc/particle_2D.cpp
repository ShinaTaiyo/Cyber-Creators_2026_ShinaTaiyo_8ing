//============================================================================
// 
// �Q�O�Q�T�N�T���P�Q���F�^�C�}�[�ɂ��J�ڂ��쐬 [particle_2D.cpp]
// Author : �Ŗ����z
// 
//============================================================================

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "particle_2D.h"
#include "bounding_box.h"
#include "renderer.h"

// �e�N�X�`���擾�p
#include "texture_manager.h"

// ���
#include "move_info.h" // �ړ����

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
CParticle_2D::CParticle_2D() :
	CObject_2D{ LAYER::FRONT },
	m_nLife{1},
	m_nMaxLife{1},
	m_InitSize{VEC3_INIT},
	m_pMove_Info{DBG_NEW CMove_Info()}
{
	SetType(CObject::TYPE::PARTICLE_2D); // �^�C�v���u�p�[�e�B�N��2D�v�ɐݒ�
}

//============================================================================
// �f�X�g���N�^
//============================================================================
CParticle_2D::~CParticle_2D()
{

}

//============================================================================
// �����ݒ�
//============================================================================
HRESULT CParticle_2D::Init()
{
	// �I�u�W�F�N�g2D�N���X�̏����ݒ�
	if (FAILED(CObject_2D::Init()))
	{
		assert(false && "�����̏����ݒ�Ɏ��s");
	}

	return S_OK;
}

//============================================================================
// �I������
//============================================================================
void CParticle_2D::Uninit()
{
	// �ړ����I������
	if (m_pMove_Info != nullptr)
	{
		delete m_pMove_Info;    // �j��
		m_pMove_Info = nullptr; // �|�C���^������
	}

	// �I�u�W�F�N�g2D�N���X�̏I������
	CObject_2D::Uninit();
}

//============================================================================
// �X�V����
//============================================================================
void CParticle_2D::Update()
{
	// === �ϐ� ===

	float fRatioLife = static_cast<float>(m_nLife) / static_cast<int>(m_nMaxLife); // �̗͊���
	D3DXCOLOR Col = GetCol(); //�F����
	D3DXVECTOR3 Pos = GetPos(); // �ʒu

	// === ���� ===

	SetSize(m_InitSize * fRatioLife);  // �����T�C�Y�ɑ̗͊������|���ďk������
	m_nLife -= 1; // �̗͂����炷

	if (m_nLife <= 0)
	{// �̗͂��Ȃ��Ȃ�����
		SetRelease(); //�@�j������
	}

	SetCol(D3DXCOLOR(Col.r, Col.g, Col.b, fRatioLife)); // �A���t�@�l��̗͊����ɂ���

	// �ړ���񂪑��݂���Ȃ�A����̍X�V
	if (m_pMove_Info != nullptr)
	{
		m_pMove_Info->Update(Pos, this);
	}

	// �ʒu�̐ݒ�
	SetPos(Pos);

	// �I�u�W�F�N�g2D�N���X�̍X�V
	CObject_2D::Update();
}

//============================================================================
// �`�揈��
//============================================================================
void CParticle_2D::Draw()
{
	// �f�o�C�X���擾
	LPDIRECT3DDEVICE9 pDev = CRenderer::GetDeviece();

	//a�u�����f�B���O�����Z�����ɐݒ�
	pDev->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	CObject_2D::Draw();//�I�u�W�F�N�g2D�̕`�揈��

	//a�u�����f�B���O�����ɖ߂�
	pDev->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//============================================================================
// �̗͎擾
//============================================================================
const int& CParticle_2D::GetLife() const
{
	return m_nLife;
}

//============================================================================
// �̗͐ݒ�
//============================================================================
void CParticle_2D::SetLife(int nLife)
{
	m_nLife = nLife;
}

//============================================================================
// �ő�̗͎擾
//============================================================================
const int& CParticle_2D::GetMaxLife() const
{
	return m_nMaxLife;
}

//============================================================================
// �ő�̗͐ݒ�
//============================================================================
void CParticle_2D::SetMaxLife(int nMaxLife)
{
	m_nMaxLife = nMaxLife;
}

//============================================================================
// �ŏ��̃T�C�Y�擾
//============================================================================
const D3DXVECTOR3& CParticle_2D::GetInitSize() const
{
	return m_InitSize;
}

//============================================================================
// �ŏ��̃T�C�Y�ݒ�
//============================================================================
void CParticle_2D::SetInitSize(D3DXVECTOR3 InitSize)
{
	m_InitSize = InitSize;
}

//============================================================================
// �ړ������擾����
//============================================================================
CMove_Info* CParticle_2D::GetMoveInfo() const
{
	return m_pMove_Info;
}

//============================================================================
// ����
//============================================================================
CParticle_2D* CParticle_2D::Create(D3DXCOLOR Col, D3DXVECTOR3 InitPos)
{
	// �C���X�^���X�𐶐�
	CParticle_2D* pParticle_2D = DBG_NEW CParticle_2D();

	if (pParticle_2D == nullptr)
	{ // �������s
		assert(false);
	}

	// �^�C�v��ݒ�
	pParticle_2D->SetType(TYPE::PARTICLE_2D);

	// �F������ݒ�
	pParticle_2D->SetCol(Col);

	// �e�N�X�`���ݒ�
	pParticle_2D->BindTex(CTexture_Manager::GetInstance()->RefRegistry().BindAtKey("Shadow_000"));

	// CParticle_2D
	pParticle_2D->Init();

	// �����ʒu�̐ݒ�
	pParticle_2D->SetPos(InitPos);

	return pParticle_2D;
}