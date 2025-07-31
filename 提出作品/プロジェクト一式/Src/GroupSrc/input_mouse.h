//============================================================================
// 
// �}�E�X���́A�w�b�_�t�@�C�� [input_mouse.h]
// Author : ���c����
// 
//============================================================================

#ifndef _INPUT_MOUSE_H_
#define _INPUT_MOUSE_H_	// ��d�C���N���[�h�h�~

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "input.h"

//****************************************************
// �}�E�X�N���X
//****************************************************
class CInputMouse : public CInput
{
public:

	// <special function>
	CInputMouse();	// �R���X�g���N�^
	~CInputMouse();	// �f�X�g���N�^

	// <function>
	HRESULT Init(HINSTANCE hInstance, HWND hWnd) override;	// �����ݒ�
	void	Uninit() override;								// �I������
	void	Update() override;								// �X�V����

	// <getter>
	bool		GetPress(int nKey);		// �v���X���擾
	bool		GetTrigger(int nKey);	// �g���K�[���擾
	bool		GetRelease(int nKey);	// �����[�X���擾
	D3DXVECTOR2	GetMouseMove();			// �}�E�X�̈ړ��ʂ��擾
	float		GetWheelScroll();		// �z�C�[���̃X�N���[���ʂ��擾

private:

	// <data>
	DIMOUSESTATE2 m_State;			// �v���X���
	DIMOUSESTATE2 m_StateTrigger;	// �g���K�[���
	DIMOUSESTATE2 m_StateRelease;	// �����[�X���
};

#endif	// _INPUT_KEYBOARD_