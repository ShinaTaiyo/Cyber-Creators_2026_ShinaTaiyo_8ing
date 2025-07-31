//============================================================================
// 
// �p�b�h���́A�w�b�_�t�@�C�� [input_pad.h]
// Author : ���c����
// 
//============================================================================

#ifndef _INPUT_PAD_H_
#define _INPUT_PAD_H_	// ��d�C���N���[�h�h�~

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "input.h"

//****************************************************
// �p�b�h�N���X
//****************************************************
class CInputPad : public CInput
{
public:

	//****************************************************
	// �p�b�h�̃L�[����
	//****************************************************
	enum class JOYKEY
	{
		UP = 0,	// �\����
		DOWN,	// �\����
		LEFT,	// �\����
		RIGHT,	// �\���E
		START,	// �X�^�[�g�{�^��
		BACK,	// �o�b�N�{�^��
		L3,		// ���X�e�B�b�N��������
		R3,		// �E�X�e�B�b�N��������
		L,		// L�{�^��
		R,		// R�{�^��
		NONE1,
		NONE2,
		A,		// A�{�^��
		B,		// B�{�^��
		X,		// X�{�^��
		Y,		// Y�{�^��
		MAX,
	};

	//****************************************************
	// �X�e�B�b�N���̍\����
	//****************************************************
	struct JOYSTICK
	{
		SHORT X;	// X���̌X��
		SHORT Y;	// Y���̌X��
	};

	// <special function>
	CInputPad();	// �R���X�g���N�^
	~CInputPad();	// �f�X�g���N�^

	// <function>
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);	// �����ݒ�
	void	Uninit() override;						// �I������
	void	Update() override;						// �X�V����

	// <data>
	bool		GetPress(JOYKEY Key);	// �v���X���擾
	bool		GetTrigger(JOYKEY Key);	// �g���K�[���擾
	bool		GetRelease(JOYKEY Key);	// �����[�X���擾
	bool		GetDepthTriggerR();		// �E�g���K�[�{�^���̎擾
	JOYSTICK	GetJoyStickL();			// ���X�e�B�b�N�̌X���擾
	JOYSTICK	GetJoyStickR();			// �E�X�e�B�b�N�̌X���擾

private:

	// <function>
	void AdjustJoyStick();	// �X�e�B�b�N�̒���

	// <data>
	XINPUT_STATE m_aKeyState;			// �v���X���
	XINPUT_STATE m_aKeyStateTrigger;	// �g���K�[���
	XINPUT_STATE m_aKeyStateRelease;	// �����[�X���
};

#endif	// _INPUT_PAD_H_