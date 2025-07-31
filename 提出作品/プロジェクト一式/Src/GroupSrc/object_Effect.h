//============================================================================
// 
// �G�t�F�N�g�I�u�W�F�N�g�A�w�b�_�t�@�C�� [object_Effect.h]
// Author : ���c����
// 
//============================================================================

#ifndef _OBJECT_EFFECT_H_
#define _OBJECT_EFFECT_H_	// ��d�C���N���[�h�h�~

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "object_Billboard.h"

// �e�N�X�`�����蓖�ėp
#include "texture_manager.h"

//****************************************************
// �G�t�F�N�g�I�u�W�F�N�g�N���X
//****************************************************
class CObject_Effect : public CObject_Billboard
{
public:

	// <special function>
	CObject_Effect(LAYER Priority = LAYER::BG);	// �R���X�g���N�^
	~CObject_Effect() override = 0;				// �f�X�g���N�^

	// <function>
	HRESULT Init() override;	// �����ݒ�
	void	Uninit() override;	// �I������
	void	Update() override;	// �X�V����
	void	Draw() override;	// �`�揈��

	// <setter>
	void SetCorrectionCoef(float fCoef);	// �␳�W����ݒ�

	// <getter/setter>
	const D3DXVECTOR3& GetSizeTarget() const;	// �ڕW�T�C�Y���擾
	void SetSizeTarget(D3DXVECTOR3 SizeTarget);	// �ڕW�T�C�Y��ݒ�

	// <getter/setter>
	const D3DXVECTOR3& GetRotTarget() const;	// �ڕW�������擾
	void SetRotTarget(D3DXVECTOR3 RotTarget);	// �ڕW������ݒ�

	// <getter/setter>
	const D3DXVECTOR3& GetPosTarget() const;	// �ڕW���W���擾
	void SetPosTarget(D3DXVECTOR3 PosTarget);	// �ڕW���W��ݒ�

	// <getter/setter>
	const D3DXCOLOR& GetColTarget() const;	// �ڕW�F���擾
	void SetColTarget(D3DXCOLOR ColTarget);	// �ڕW�F��ݒ�

	// <getter/setter>
	const int& GetMaxDuration();	// �ő���Ԃ��擾
	void SetMaxDuration(int nMax);	// �ő���Ԃ�ݒ�

	// <getter>
	const int& GetDuration();	// �p�����Ԃ��擾

private:

	// <function>
	void CountDuration();		// �p���J�E���g
	void CorrectToTaraget();	// �ڕW�l�ւ̕␳

	// <data>
	float		m_fCorrectionCoef;	// �␳�W��
	D3DXVECTOR3	m_SizeTarget;		// �ڕW�T�C�Y
	D3DXVECTOR3	m_RotTarget;		// �ڕW����
	D3DXVECTOR3	m_PosTarget;		// �ڕW���W
	D3DXCOLOR	m_ColTarget;		// �ڕW�F
	int			m_nMaxDuration;		// �ő����
	int			m_nDuration;		// �p������
};

#endif // _OBJECT_BILLBOARD_H_