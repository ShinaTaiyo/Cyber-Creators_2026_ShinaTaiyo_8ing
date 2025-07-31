//============================================================================
// 
// �L�[�{�[�h���́A�w�b�_�t�@�C�� [input_keyboard.h]
// Author : ���c����
// 
//============================================================================

#ifndef _INPUT_KEYBOARD_H_
#define _INPUT_KEYBOARD_H_	// ��d�C���N���[�h�h�~

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "input.h"

//****************************************************
// �L�[�{�[�h�N���X
//****************************************************
class CInputKeyboard : public CInput
{
public:

	// <special function>
	CInputKeyboard();	// �R���X�g���N�^
	~CInputKeyboard();	// �f�X�g���N�^

	// <function>
	HRESULT Init(HINSTANCE hInstance, HWND hWnd) override;	// �����ݒ�
	void	Uninit() override;								// �I������
	void	Update() override;								// �X�V����

	// <getter>
	bool GetPress(int nKey);	// �v���X���擾
	bool GetTrigger(int nKey);	// �g���K�[���擾
	bool GetRelease(int nKey);	// �����[�X���擾

private:

	// <data>
	BYTE m_aKeyState[MAX_KEY];			// �v���X���
	BYTE m_aKeyStateTrigger[MAX_KEY];	// �g���K�[���
	BYTE m_aKeyStateRelease[MAX_KEY];	// �����[�X���
};

#endif	// _INPUT_KEYBOARD_