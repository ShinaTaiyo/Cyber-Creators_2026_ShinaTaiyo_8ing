//============================================================================
// 
// �e�L�X�g���b�V���I�u�W�F�N�g�A�w�b�_�t�@�C�� [object_TextMesh.h]
// Author : ���c����
// 
//============================================================================

#ifndef _OBJECT_TEXTMESH_H_
#define _OBJECT_TEXTMESH_H_	// ��d�C���N���[�h�h�~

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "object.h"

//****************************************************
// �e�L�X�g���b�V���N���X
//****************************************************
class CObject_TextMesh : public CObject
{
public:

	// <special function>
	CObject_TextMesh(LAYER Priority = LAYER::FRONT);	// �R���X�g���N�^
	~CObject_TextMesh() override;						// �f�X�g���N�^

	// <function>
	HRESULT	Init() override;	// �����ݒ�
	void	Uninit() override;	// �I������
	void	Update() override;	// �X�V����
	void	Draw() override;	// �`�揈��

	// <getter>
	LPDIRECT3DSURFACE9 GetSurface();	// �T�[�t�F�C�X���擾

	// <getter/setter>
	const std::string& GetText() const;	// �e�L�X�g���擾
	void SetText(std::string Text);		// �e�L�X�g��ݒ�

	// <getter/setter>
	const std::string& GetTextTarget() const;	// �ڕW�e�L�X�g���擾
	void SetTextTarget(std::string Text);		// �ڕW�e�L�X�g��ݒ�

	// <timer -> setter>
	void SetTimer(int nTimer);	// �^�C�}�[��ݒ�
 
	// <getter/setter>
	const D3DXVECTOR2& GetTextSize() const;	// �e�L�X�g�T�C�Y�擾
	void SetTextSize(D3DXVECTOR2 Size);		// �e�L�X�g�T�C�Y�ݒ�

	// <getter/setter>
	const D3DXVECTOR3& GetMeshSize() const;	// ���b�V���T�C�Y�擾
	void SetMeshSize(D3DXVECTOR3 Size);		// ���b�V���T�C�Y�ݒ�

	// <getter/setter>
	const D3DXVECTOR3& GetRot() const;	// �����擾
	void SetRot(D3DXVECTOR3 Rot);		// �����ݒ�

	// <getter/setter>
	const D3DXVECTOR3& GetPos() const;	// ���W�擾
	void SetPos(D3DXVECTOR3 Pos);		// ���W�ݒ�

	// <getter/setter>
	const D3DXCOLOR& GetCol() const;	// �F�擾
	void SetCol(D3DXCOLOR Col);			// �F�ݒ�

	// <getter/setter>
	const float& GetAlpha() const;	// �A���t�@�l�擾
	void SetAlpha(float fAlpha);	// �A���t�@�l�ݒ�

	// <getter>
	const float& GetLength() const;	// �W�J�p�Ίp���擾

	// <static function>
	static CObject_TextMesh* Create(JSON Json);	// ����

private:

	/// <summary> ���_�� </summary>
	static constexpr WORD NUM_VTX = 4;

	/// <summary> �v���~�e�B�u�� </summary>
	static constexpr WORD NUM_PRIM = 2;

	// <function>
	HRESULT CreateVtxBuff();	// ���_�o�b�t�@�̐���
	HRESULT CreateTex();		// �e�N�X�`���̐���
	void	DisapperByTimer();	// ���Ԍo�߂ɂ�����
	void	TextAnimation();	// �e�L�X�g����
	void	SetMtxWorld();		// ���[���h�s��ݒ�

	// <data>
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		// ���_�o�b�t�@�̃|�C���^
	LPDIRECT3DTEXTURE9		m_pTex;			// �e�N�X�`���̃|�C���^
	LPDIRECT3DSURFACE9		m_pSurface;		// �T�[�t�F�C�X�̃|�C���^
	std::string				m_Text;			// �e�L�X�g
	std::string				m_TextTarget;	// �ڕW�e�L�X�g
	int						m_nCntDisp;		// �e�L�X�g����J�E���g
	int						m_nTextSpeed;	// �e�L�X�g�X�s�[�h
	int						m_nTextDelay;	// �e�L�X�g�f�B���C
	int						m_nTimer;		// �^�C�}�[
	D3DXVECTOR2				m_TextSize;		// �e�L�X�g�T�C�Y
	D3DXVECTOR3				m_MeshSize;		// ���b�V���T�C�Y
	D3DXVECTOR3				m_Rot;			// ����
	D3DXVECTOR3				m_Pos;			// ���W
	D3DXCOLOR				m_Col;			// �F
	float					m_fLength;		// �W�J�p�Ίp��
	float					m_fAngle;		// �Ίp���p�p�x
	D3DXMATRIX				m_MtxWorld;		// ���[���h�s��
};

#endif // _OBJECT_TEXTMESH_H_