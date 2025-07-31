//============================================================================
// 
// �Q�O�Q�T�N�U���Q�U���F�X�e���V���o�b�t�@���������� [StencilMask.cpp]
// Author : �Ŗ����z
// 
//============================================================================

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "StencilMask.h"
#include "bounding_box.h"
#include "renderer.h"

// ���͗p
#include "manager.h"

// �e�N�X�`���擾�p
#include "texture_manager.h"

//****************************************************
// using�f�B���N�e�B�u
//****************************************************
using namespace useful;

//****************************************************
// �ÓI�����o�ϐ�
//****************************************************
bool CStencilMask::s_bStencil = true; // �X�e���V���o�b�t�@���g�p���邩�ǂ���

//============================================================================
// 
// public�����o
// 
//============================================================================

//============================================================================
// �R���X�g���N�^
//============================================================================
CStencilMask::CStencilMask() :
	CObject_2D{CObject::LAYER::STENCIL}
{
	SetType(CObject::TYPE::STENCILMASK); // �^�C�v���u�X�e���V���}�X�N�v�ɐݒ�
}

//============================================================================
// �f�X�g���N�^
//============================================================================
CStencilMask::~CStencilMask()
{
	// �����Ȃ�
}

//============================================================================
// �����ݒ�
//============================================================================
HRESULT CStencilMask::Init()
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
void CStencilMask::Uninit()
{
	// �I�u�W�F�N�g2D�N���X�̏I������
	CObject_2D::Uninit();
}

//============================================================================
// �X�V����
//============================================================================
void CStencilMask::Update()
{
	// �I�u�W�F�N�g2D�N���X�̍X�V
	CObject_2D::Update();
}

//============================================================================
// �`�揈��
//============================================================================
void CStencilMask::Draw()
{
	// �X�e���V���o�b�t�@�`��ݒ������
	SetStencilState();

	// �X�e���V���o�b�t�@���g�p����Ȃ�S�̂Ƀ}�X�N�p�|���S����`�悵�܂�t
	if (s_bStencil)
	{
		CObject_2D::Draw();
	}
	// �X�e���V���o�b�t�@�`��ݒ�����ɖ߂�
	ResetStencilState();
}

//============================================================================
// ����
//============================================================================
CStencilMask* CStencilMask::Create(D3DXVECTOR3 InitPos)
{
	// �C���X�^���X�𐶐�
	CStencilMask* pStencilMask = DBG_NEW CStencilMask();

	// �������s
	if (pStencilMask == nullptr)
	{
		assert(false);
	}

	// �^�C�v��ݒ�
	pStencilMask->SetType(TYPE::STENCILMASK);

	// �T�C�Y�ݒ�
	pStencilMask->SetSize(D3DXVECTOR3(FSCREEN_WIDTH, FSCREEN_HEIGHT, 0.0f));

	// �F�����ݒ�
	pStencilMask->SetCol(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));

	// === ���̑������ݒ� ===

	pStencilMask->Init(); // ����������

	pStencilMask->SetPos(InitPos);// �����ʒu�ݒ�

	return pStencilMask;
}

//============================================================================
//	���̃I�u�W�F�N�g���X�e���V���o�b�t�@�̐ݒ���s���i�X�e���V���e�X�g���I���̏ꍇ�A�S�ă}�X�N�����j
//============================================================================
void CStencilMask::SetOtherStencilState()
{
	// �X�e���V���o�b�t�@���g�p����Ȃ�ݒ肷��
	if (!s_bStencil)
	{
		return;
	}

	// �f�o�C�X���擾
	LPDIRECT3DDEVICE9 pDev = CRenderer::GetDeviece();

	// �헪�F�܂��`�敔���͑S��0x05�ŃX�e���V���l��ݒ�A���̎��ɑ傫�ȃ|���S���i0x03�Őݒ肵�A
	//���R���傫����Ε`�悳���̂ŁA���f��������Ƃ������|���S�����`�悳��܂��B
	// Z�o�b�t�@�ւ̏������݂͕��ʂɍs���܂��B�X�e���V���p�|���S�����̂��̂���Ȃ��̂ŉ��s�K�v

	// �X�e���V���e�X�g��L��
	pDev->SetRenderState(D3DRS_STENCILENABLE, TRUE);

	//���X�e���V���e�X�g��
	//�E�X�e���V���}�X�N
	//�E�X�e���V���o�b�t�@�̒l
	//�E�X�e���V���Q�ƒl
	// ���̂R�v�f��ݒ肵�A�������r�֐��Ŕ�����s���܂��B

	// �X�e���V���Q�ƒl�̐ݒ�
	pDev->SetRenderState(D3DRS_STENCILREF, 0x05); // �X�e���V���Q�ƒl��0x05�ɂ���B

	// �}�X�N�̐ݒ�

	// �X�e���V���}�X�N�̐ݒ�B�i�ǂ̃r�b�g�����������邩�����j
	// �z���̎d���F�z�����Ԃ��ꕔ�Ɍ����J���Ă���A����������r���Ȃ�

	pDev->SetRenderState(D3DRS_STENCILMASK, 0xffffffff);

	// �X�e���V���}�X�N�̐ݒ�B�i�ǂ̃r�b�g�Ŕ�r���邩�v�𐧌�
	pDev->SetRenderState(D3DRS_STENCILWRITEMASK, 0xffffffff);

	// �ǂ�ȏ����������������ɃX�e���V���o�b�t�@�ɏ������ނ̂��H�@�̐ݒ�
	pDev->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_ALWAYS); // �K���X�e���V���o�b�t�@�ɕ`�����܂���A�Ƃ����ݒ�B

	// �K���X�e���V���e�X�g�ɍ��i����̂ł��̐ݒ�͂���Ȃ�
	{
		// �������ޏꍇ�A�ǂ�Ȓl���������ނ̂��H�̐ݒ�B
		pDev->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_INCR); // �X�e���V���e�X�g�ɕs���i�̎��E�E�E�������Ȃ��B

		// �X�e���V���e�X�g�ɍ��i�A�[�x�e�X�g�ɕs���i�̎��E�E�E�������Ȃ�
		//�����f�����B�ꂽ�����͐[�x�e�X�g�s���i�ɂȂ�̂ł���ő��v�I
		pDev->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_INCR);

	}

	// �X�e���V���e�X�g�Ɛ[�x�e�X�g�ɍ��i�̎��E�E�E�l���C���N�������g���܂�
	pDev->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_INCR);
}

//============================================================================
// �}�X�N���ꂽ���Ȃ��I�u�W�F�N�g�̕`�揈���ŌĂт܂��B
//============================================================================
void CStencilMask::SetOtherStencilAvoid()
{
	// �X�e���V���o�b�t�@���g�p����Ȃ�ݒ肷��
	if (!s_bStencil)
	{
		return;
	}

	// �f�o�C�X���擾
	LPDIRECT3DDEVICE9 pDev = CRenderer::GetDeviece();

	// �X�e���V���e�X�g�𖳌��ɂ���
	pDev->SetRenderState(D3DRS_STENCILENABLE,TRUE);

	// �X�e���V���Q�ƒl�̐ݒ�
	pDev->SetRenderState(D3DRS_STENCILREF, 0x00); // �X�e���V���Q�ƒl��0x05�ɂ���B

	// �}�X�N�̐ݒ�

	// �X�e���V���}�X�N�̐ݒ�B�i�ǂ̃r�b�g�����������邩�����j
	// �z���̎d���F�z�����Ԃ��ꕔ�Ɍ����J���Ă���A����������r���Ȃ�

	pDev->SetRenderState(D3DRS_STENCILMASK, 0xffffffff);

	// �X�e���V���}�X�N�̐ݒ�B�i�ǂ̃r�b�g�Ŕ�r���邩�v�𐧌�
	pDev->SetRenderState(D3DRS_STENCILWRITEMASK, 0xffffffff);

	// �ǂ�ȏ����������������ɃX�e���V���o�b�t�@�ɏ������ނ̂��H�@�̐ݒ�
	pDev->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_ALWAYS); // �������ŃX�e���V���e�X�g�ɍ��i

	// �X�e���V���e�X�g�ɕs���i�̏ꍇ�E�E�E�X�e���V���l���O�ɂ���
	pDev->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_REPLACE);

	//�X�e���V���e�X�g�ɍ��i���AZ�e�X�g�ɂ͕s���i�Ȃ�E�E�E�X�e���V���l���O�ɂ���
	pDev->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_REPLACE);

	// �X�e���V���e�X�g�Ɛ[�x�e�X�g�ɍ��i�̎��E�E�E�X�e���V���l���O�ɂ���
	pDev->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_REPLACE);
}

//============================================================================
// ���̃I�u�W�F�N�g���X�e���V���o�b�t�@�̐ݒ���s��
//============================================================================
void CStencilMask::SetOtherResetStencilState()
{
	// �X�e���V���o�b�t�@���g�p����Ȃ�ݒ肷��
	if (!s_bStencil)
	{
		return;
	}

	// �f�o�C�X���擾
	LPDIRECT3DDEVICE9 pDev = CRenderer::GetDeviece();

	// �X�e���V���e�X�g�𖳌��ɂ���
	pDev->SetRenderState(D3DRS_STENCILENABLE, FALSE);

	// �X�e���V���Q�ƒl�̐ݒ�
	pDev->SetRenderState(D3DRS_STENCILREF, 0x03); // �X�e���V���Q�ƒl��0x03�ɂ���B

	// �X�e���V���}�X�N�ݒ�i�������Ɠ���)
	pDev->SetRenderState(D3DRS_STENCILMASK, 0xffffffff);

	// �X�e���V���}�X�N�̐ݒ�B�i�ǂ̃r�b�g�Ŕ�r���邩�v�𐧌�
	pDev->SetRenderState(D3DRS_STENCILWRITEMASK, 0xffffffff);

	// �ǂ�ȏ����������������ɃX�e���V���o�b�t�@�ɏ������ނ̂��H�@�̐ݒ�
	pDev->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_LESSEQUAL); // �K���X�e���V���o�b�t�@�ɕ`�����܂���A�Ƃ����ݒ�B
}

//============================================================================
// ���̃I�u�W�F�N�g���X�e���V���o�b�t�@�̐ݒ���s��
//============================================================================
void CStencilMask::SetUseStencil(bool bUse)
{
	s_bStencil = bUse;
}

//============================================================================
// �X�e���V���o�b�t�@�̐ݒ���s��
//============================================================================
void CStencilMask::SetStencilState()
{
	// �X�e���V���o�b�t�@���g�p����Ȃ�ݒ肷��
	if (!s_bStencil)
	{
		return;
	}

	// �f�o�C�X���擾
	LPDIRECT3DDEVICE9 pDev = CRenderer::GetDeviece();

	// ���ʂɉ��s���g���܂��B�}�X�N�̐F�����߂����̂�
	pDev->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

	// Z�o�b�t�@�ɓo�^���Ȃ��i���܂ł̐[�x���Ɣ�ׂ邪�A���g�͓o�^���Ȃ��j
	pDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	// �X�e���V���e�X�g��L��
	pDev->SetRenderState(D3DRS_STENCILENABLE, TRUE);

	//���X�e���V���e�X�g��
	//�E�X�e���V���}�X�N
	//�E�X�e���V���o�b�t�@�̒l
	//�E�X�e���V���Q�ƒl
	// ���̂R�v�f��ݒ肵�A�������r�֐��Ŕ�����s���܂��B

	// �X�e���V���Q�ƒl�̐ݒ�
	pDev->SetRenderState(D3DRS_STENCILREF, 0x0A); // �X�e���V���Q�ƒl��0x03�ɂ���B

	// �X�e���V���}�X�N�̐ݒ�B�i�ǂ̃r�b�g�����������邩�����j
	// �z���̎d���F�z�����Ԃ��ꕔ�Ɍ����J���Ă���A����������r���Ȃ�

	pDev->SetRenderState(D3DRS_STENCILMASK, 0xffffffff);

	// �X�e���V���}�X�N�̐ݒ�B�i�ǂ̃r�b�g�Ŕ�r���邩�v�𐧌�
	//���}�X�N�l��0xffffffff�ɂ���Ɣ�r�̎��̌v�Z���ȗ��������݂���
	pDev->SetRenderState(D3DRS_STENCILWRITEMASK, 0xffffffff);

	// �ǂ�ȏ����������������ɃX�e���V���o�b�t�@�ɏ������ނ̂��H�@�̐ݒ�i�P�O���P�P�j
	pDev->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_LESS); // "�P�O"���X�e���V���l�ȉ��Ȃ�`��

	// �X�e���V���e�X�g�ɕs���i�̏ꍇ�E�E�E�������Ȃ�
	pDev->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);

	//�X�e���V���e�X�g�ɍ��i���AZ�e�X�g�ɂ͕s���i�Ȃ牽�����Ȃ�
	pDev->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);

	// �X�e���V���e�X�g�Ɛ[�x�e�X�g�ɍ��i�̎��E�E�E�������Ȃ��B
	pDev->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_KEEP); 
}

//============================================================================
// �X�e���V���o�b�t�@�̐ݒ�����ɖ߂�
//============================================================================
void CStencilMask::ResetStencilState()
{
	// �X�e���V���o�b�t�@���g�p����Ȃ�ݒ肷��
	if (!s_bStencil)
	{
		return;
	}

	// �f�o�C�X���擾
	LPDIRECT3DDEVICE9 pDev = CRenderer::GetDeviece();

	// Z�o�b�t�@�ւ̏������݂����ɖ߂�
	pDev->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

	// Z�o�b�t�@�ɓo�^����
	pDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	// �X�e���V���e�X�g�𖳌��ɂ���
	pDev->SetRenderState(D3DRS_STENCILENABLE, FALSE);

	// �X�e���V���Q�ƒl�̐ݒ�
	pDev->SetRenderState(D3DRS_STENCILREF, 0x0A); // �X�e���V���Q�ƒl���P�O�ɂ���B

	// �X�e���V���}�X�N�ݒ�i�������Ɠ���)
	pDev->SetRenderState(D3DRS_STENCILMASK, 0xffffffff);

	// �X�e���V���}�X�N�̐ݒ�B�i�ǂ̃r�b�g�Ŕ�r���邩�v�𐧌�
	pDev->SetRenderState(D3DRS_STENCILWRITEMASK, 0xffffffff);

	// �ǂ�ȏ����������������ɃX�e���V���o�b�t�@�ɏ������ނ̂��H�@�̐ݒ�
	pDev->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_LESSEQUAL); // �K���X�e���V���o�b�t�@�ɕ`�����܂���A�Ƃ����ݒ�B
}
