//============================================================================
// 
// �Q�O�Q�T�N�S���Q�W���F�u���b�N�N���X���쐬 [Button.cpp]
// Author : �Ŗ����z
// 
//============================================================================

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "Button.h"
#include "bounding_box.h"

// ���͗p
#include "manager.h"
#include "renderer.h"

// �e�N�X�`���擾�p
#include "texture_manager.h"

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
CButton::CButton() :
	m_nAnimCnt{0},
	m_nMaxAnim{0},
	CObject_2D{ LAYER::UI }
{
	SetType(CObject::TYPE::BUTTON); // �^�C�v���u�{�^���v�ɐݒ�
}

//============================================================================
// �f�X�g���N�^
//============================================================================
CButton::~CButton()
{

}

//============================================================================
// �����ݒ�
//============================================================================
HRESULT CButton::Init()
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
void CButton::Uninit()
{
	// �I�u�W�F�N�g2D�N���X�̏I������
	CObject_2D::Uninit();
}

//============================================================================
// �X�V����
//============================================================================
void CButton::Update()
{
	// �T�t���[�����ƂɃA�j���[�V������i�߂�
	if (m_nAnimCnt % 5 == 0)
	{
		int nNowPatternU = GetNowPatternU(); // �e�N�X�`�����WU���擾
		nNowPatternU++;
		// �ő�ɒB�������ɖ߂�
		if (nNowPatternU == m_nMaxAnim)
		{
			nNowPatternU = 0;
		}

		// �e�N�X�`�����WU��ݒ�
		SetNowPatternU(nNowPatternU);
	}

	// �I�u�W�F�N�g2D�N���X�̍X�V
	CObject_2D::Update();

	m_nAnimCnt++;
}

//============================================================================
// �`�揈��
//============================================================================
void CButton::Draw()
{
	LPDIRECT3DDEVICE9 pDev = CRenderer::GetDeviece(); // �f�o�C�X�擾

	pDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	pDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);

	// �I�u�W�F�N�g2D�N���X�̕`�揈��
	CObject_2D::Draw();

	pDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	pDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

}

//============================================================================
// ����
//============================================================================
CButton* CButton::Create(D3DXVECTOR3 InitPos)
{
	// �C���X�^���X�𐶐�
	CButton* pButton = DBG_NEW CButton();

	// �������s
	if (pButton == nullptr)
	{
		assert(false);
	}


	pButton->Init(); // ����������
	pButton->SetPos(InitPos);// �����ʒu�ݒ�
	
	// === �e�N�X�`�����ݒ� ===

	pButton->BindTex(CTexture_Manager::GetInstance()->RefRegistry().BindAtKey("UIButton_000"));
	pButton->m_nMaxAnim = 5;
	pButton->SetTexWidth(0.2f);  // �����T���̂P
	pButton->SetTexHeight(1.0f); // �����P
	pButton->SetNowPatternU(0);  // �O�̈ʒu�ɂ���
	pButton->SetNowPatternV(0);  // �O�̈ʒu�ɂ���

	return pButton;
}