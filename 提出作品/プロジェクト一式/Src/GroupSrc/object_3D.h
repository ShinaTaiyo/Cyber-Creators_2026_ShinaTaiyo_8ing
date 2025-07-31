//============================================================================
// 
// 3D�I�u�W�F�N�g�A�w�b�_�t�@�C�� [object_3D.h]
// Author : ���c����
// 
//============================================================================

#ifndef _OBJECT_3D_H_
#define _OBJECT_3D_H_	// ��d�C���N���[�h�h�~

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "object.h"

//****************************************************
// 3D�I�u�W�F�N�g�N���X
//****************************************************
class CObject_3D : public CObject
{
	//****************************************************
	// �^�G�C���A�X�̒�`
	//****************************************************
	using BUFF_VTX = LPDIRECT3DVERTEXBUFFER9;
	using BUFF_TEX = LPDIRECT3DTEXTURE9;

public:

	// <special function>
	CObject_3D(LAYER Priority = LAYER::BG);	// �R���X�g���N�^
	~CObject_3D() override;					// �f�X�g���N�^

	// <function>
	HRESULT	Init() override;	// �����ݒ�
	void	Uninit() override;	// �I������
	void	Update() override;	// �X�V����
	void	Draw() override;	// �`�揈��

	// <setter>
	void BindTex(LPDIRECT3DTEXTURE9 pTex);	// �e�N�X�`������

	// <getter/setter>
	const D3DXVECTOR3& GetSize() const;	// �T�C�Y�擾
	void SetSize(D3DXVECTOR3 Size);		// �T�C�Y�ݒ�

	// <getter/setter>
	const D3DXVECTOR3& GetRot() const;	// �����擾
	void SetRot(D3DXVECTOR3 Rot);		// �����ݒ�

	// <getter/setter>
	const D3DXVECTOR3& GetPos() const;	// ���W�擾
	void SetPos(D3DXVECTOR3 Pos);		// ���W�ݒ�

	// <getter/setter>
	const D3DXVECTOR3& GetScale() const; // �g�嗦�擾
	void SetScale(D3DXVECTOR3 Scale);    // �g�嗦�ݒ�

	//<getter/setter>
	const D3DXVECTOR3& GetInitSize() const; // �����T�C�Y�擾
	void SetInitSize(D3DXVECTOR3 Size);     // �����T�C�Y�ݒ�

	// <getter/setter>
	const D3DXCOLOR& GetCol() const;	// �F�擾
	void SetCol(D3DXCOLOR Col);			// �F�ݒ�

	// <getter/setter>
	const float& GetAlpha() const;	// �A���t�@�l�擾
	void SetAlpha(float fAlpha);	// �A���t�@�l�ݒ�

	// <getter>
	const float& GetLength() const;	// �W�J�p�Ίp���擾

	// <static function>
	static CObject_3D* Create();	// ����

	// �e�N�X�`���A�j���[�^�[�|�C���^�̎Q��
	inline useful::up<CTextureAnimator>& RefTextureAnimator() { return m_upTextureAnimator; }

private:

	/// <summary> ���_�� </summary>
	static constexpr WORD NUM_VTX = 4;

	/// <summary> �v���~�e�B�u�� </summary>
	static constexpr WORD NUM_PRIM = 2;

	// <function>
	HRESULT CreateVtxBuff();	// ���_�o�b�t�@�̐���
	void	SetMtxWorld();		// ���[���h�s��ݒ�
	void    UpdateSize();       // �T�C�Y�X�V

	// <data>
	BUFF_VTX	m_pVtxBuff;	// ���_�o�b�t�@�̃|�C���^
	BUFF_TEX	m_pTex;		// �e�N�X�`���̃|�C���^
	D3DXVECTOR3	m_Size;		// �T�C�Y
	D3DXVECTOR3 m_InitSize;	// �����T�C�Y
	D3DXVECTOR3	m_Rot;		// ����
	D3DXVECTOR3	m_Pos;		// ���W
	D3DXVECTOR3 m_Scale;	// �g�嗦
	D3DXCOLOR	m_Col;		// �F
	float		m_fLength;	// �W�J�p�Ίp��
	float		m_fAngle;	// �Ίp���p�p�x
	D3DXMATRIX	m_MtxWorld;	// ���[���h�s��

	// �e�N�X�`���A�j���[�^�[������
	std::unique_ptr<CTextureAnimator> m_upTextureAnimator;
};

#endif // _OBJECT_3D_H_