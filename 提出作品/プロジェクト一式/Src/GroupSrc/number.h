//============================================================================
// 
// �Q�O�Q�T�N�T���P�Q���F�^�C�}�[�ɂ��V�[���J�ڎ��� [number.h]
// Author : �Ŗ����z
// 
//============================================================================

#ifndef _NUMBER_H_
#define _NUMBER_H_	// ��d�C���N���[�h�h�~

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "object_2D.h"

//****************************************************
// �v���C���[�N���X
//****************************************************
class CNumber : public CObject_2D
{
public:
	// <special function>
	CNumber();				// �R���X�g���N�^
	~CNumber() override;	// �f�X�g���N�^

	// <function>
	HRESULT Init() override;	// �����ݒ�
	void	Uninit() override;	// �I������
	void	Update() override;	// �X�V����
	void	Draw() override;	// �`�揈��

	// <static function>
	static CNumber* Create(D3DXVECTOR3 InitPos);	// ����
private:
};

#endif // _PALYER_H_