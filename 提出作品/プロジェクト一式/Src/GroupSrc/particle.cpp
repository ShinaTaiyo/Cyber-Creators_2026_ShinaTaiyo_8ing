//============================================================================
// 
// �Q�O�Q�T�N�T���Q�Q���F�p�[�e�B�N�����쐬 [particle.cpp]
// Author : �Ŗ����z
// 
//============================================================================

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "particle.h" // �p�[�e�B�N��

// �f�o�C�X�擾�p
#include "renderer.h"

// �e�N�X�`���擾�p
#include "texture_manager.h"

// ���
#include "move_info.h"    // �ړ�
#include "horming_info.h" // �z�[�~���O���

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
CParticle::CParticle() : 
	m_nLife{ 1 },
	m_nMaxLife{ 1 },
	m_MaxSize{ VEC3_INIT },
	m_pMove_Info{ DBG_NEW CMove_Info() },
	m_pHorming_Info{DBG_NEW CHorming_Info()},
	m_nCntFrame{0},
	CObject_Billboard()
{
	SetType(CObject::TYPE::PARTICLE); // �^�C�v���u�p�[�e�B�N���v�ɐݒ�
}

//============================================================================
// �f�X�g���N�^
//============================================================================
CParticle::~CParticle()
{

}

//============================================================================
// �����ݒ�
//============================================================================
HRESULT CParticle::Init()
{
	CObject_Billboard::Init(); // �r���{�[�h����������
	return S_OK;
}

//============================================================================
// �I������
//============================================================================
void CParticle::Uninit()
{
	// �ړ����I������
	if (m_pMove_Info != nullptr)
	{
		delete m_pMove_Info;    // �j��
		m_pMove_Info = nullptr; // �|�C���^������
	}

	// �z�[�~���O���I������
	if (m_pHorming_Info != nullptr)
	{
		delete m_pHorming_Info;    // �j��
		m_pHorming_Info = nullptr; // �|�C���^������
	}

	CObject_Billboard::Uninit(); // �r���{�[�h�I������
}

//============================================================================
// �X�V����
//============================================================================
void CParticle::Update()
{
	float fRatioLife = static_cast<float>(m_nLife) / static_cast<float>(m_nMaxLife); // �̗͂̊���
	D3DXCOLOR Col = GetCol();   // �F����
	D3DXVECTOR3 Pos = GetPos(); // �ʒu
	Col.a = fRatioLife;         // �A���t�@�l��̗͊����ɐݒ�

	m_nCntFrame++; // �t���[�������J�E���g�A�b�v
	m_nLife -= 1;  // �̗͂����炷
	SetCol(Col);   // �̗͊����𔽉f�������F�����ɐݒ�
	SetSize(m_MaxSize * fRatioLife); // ���݂̃T�C�Y��̗͂̊����Őݒ�

	// �ړ���񂪂���Ȃ炻�̍X�V���s��
	if (m_pMove_Info != nullptr)
	{
		m_pMove_Info->Update(Pos, this);
		SetPos(Pos);
	}

	// �z�[�~���O��񂪂���Ȃ炻�̍X�V���s��
	if (m_pHorming_Info != nullptr)
	{
		SetPos(Pos + m_pHorming_Info->Update(Pos,this));
	}

	// �̗͂��Ȃ��Ȃ�����j��
	if (m_nLife <= 0)
	{
		SetRelease();
	}
	
	CObject_Billboard::Update(); // �r���{�[�h�X�V����
}

//============================================================================
// �`�揈��
//============================================================================
void CParticle::Draw()
{
	// �f�o�C�X���擾
	LPDIRECT3DDEVICE9 pDev = CRenderer::GetDeviece();

	// ���C�g�𖳌��ɂ���
	pDev->SetRenderState(D3DRS_LIGHTING, FALSE);

	// Z�o�b�t�@�ɏ������܂Ȃ�
	pDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	// �A���t�@�e�X�g��L��(�A���t�@�l���O���傫���ꍇ�ɕ`�悷��j
	pDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDev->SetRenderState(D3DRS_ALPHAREF, 0); // ��l���O�Ƃ���
	pDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER); // ��l���傫���ꍇ�ɕ`�悷��

	// a�u�����f�B���O�����Z�����ɐݒ�i�ŏI�F = �\�[�X�F �~ �\�[�X�̃A���t�@ + �w�i�F �~ 1�j
	// �����̐ݒ�̓A���t�@�e�X�g��ON�ɂ��Ȃ��ƓK�p����܂���B
	pDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE); // �A���t�@�u�����h���g�p����
	pDev->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD); // �u�����h�̌v�Z���@�����Z�����ɂ���
	pDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA); // �`�悷��e�N�X�`����F���A���t�@�l�ɔ�Ⴓ����
	pDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE); // �w�i�̉e���x���P�O�O����

	CObject_Billboard::Draw(); // �r���{�[�h�̕`�揈��

	//���C�g��L���ɖ߂�
	pDev->SetRenderState(D3DRS_LIGHTING, TRUE);

	//Z�o�b�t�@�ɏ�������
	pDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	//�A���t�@�e�X�g�����ɖ߂�
	pDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	//a�u�����f�B���O�����ɖ߂�
	pDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE); // �A���t�@�u�����h���g�p���Ȃ�
	pDev->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

}

//============================================================================
// �ړ������擾����
//============================================================================
CMove_Info* CParticle::GetMoveInfo() const
{
	return m_pMove_Info;
}

//============================================================================
// �z�[�~���O�����擾����
//============================================================================
CHorming_Info* CParticle::GetHormingInfo() const
{
	return m_pHorming_Info;
}

//============================================================================
// �ő�T�C�Y�擾
//============================================================================
const D3DXVECTOR3& CParticle::GetMaxSize() const
{
	return m_MaxSize;
}

//============================================================================
// �ő�T�C�Y�ݒ�
//============================================================================
void CParticle::SetMaxSize(D3DXVECTOR3 MaxSize)
{
	m_MaxSize = MaxSize;
}

//============================================================================
// �̗͎擾
//============================================================================
const int& CParticle::GetLife() const
{
	return m_nLife;
}

//============================================================================
// �̗͐ݒ�
//============================================================================
void CParticle::SetLife(int nLife)
{
	m_nLife = nLife;
}

//============================================================================
// �ő�̗͎擾
//============================================================================
const int& CParticle::GetMaxLife() const
{
	return m_nMaxLife;
}

//============================================================================
// �ő�̗͐ݒ�
//============================================================================
void CParticle::SetMaxLife(int nMaxLife)
{
	m_nMaxLife = nMaxLife;
}

//============================================================================
// �t���[�������擾����
//============================================================================
const int& CParticle::GetCntFrame() const
{
	return m_nCntFrame;
}

//============================================================================
// ��������
//============================================================================
CParticle* CParticle::Create(D3DXVECTOR3 InitPos, D3DXVECTOR3 InitSize, D3DXCOLOR Col, int nLife, int nMaxLife)
{
	CParticle * pParticle = DBG_NEW CParticle(); // �C���X�^���X�𐶐�

	// �������s
	if (pParticle == nullptr)
	{ 
		assert(false);
	}

	// �^�C�v��ݒ�
	pParticle->SetType(TYPE::PARTICLE);

	// �F������ݒ�
	pParticle->SetCol(Col);

	// �e�N�X�`���ݒ�
	pParticle->BindTex(CTexture_Manager::GetInstance()->RefRegistry().BindAtKey("Shadow_000"));

	// ����������
	pParticle->Init();

	// �ʒu�ݒ�
	pParticle->SetPos(InitPos);

	// �T�C�Y�ݒ�
	pParticle->SetSize(InitSize);
	pParticle->SetMaxSize(InitSize);

	// �̗͐ݒ�
	pParticle->SetLife(nLife);
	pParticle->SetMaxLife(nMaxLife);

	return pParticle;
}