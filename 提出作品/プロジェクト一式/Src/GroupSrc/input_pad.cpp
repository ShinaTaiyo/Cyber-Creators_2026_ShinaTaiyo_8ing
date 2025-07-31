//============================================================================
// 
// �p�b�h���� [input_pad.cpp]
// Author : ���c����
// 
//============================================================================

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "input_pad.h"

//****************************************************
// �v���v���Z�b�T�f�B���N�e�B�u
//****************************************************
#define LEFT_JOYSTICK_CONVERT 0	// ���X�e�B�b�N���͂�ϊ�

//============================================================================
// 
// public�����o
// 
//============================================================================

//============================================================================
// �R���X�g���N�^
//============================================================================
CInputPad::CInputPad() :
	m_aKeyState{},
	m_aKeyStateTrigger{},
	m_aKeyStateRelease{}
{

}

//============================================================================
// �f�X�g���N�^
//============================================================================
CInputPad::~CInputPad()
{

}

//============================================================================
// �����ݒ�
//============================================================================
HRESULT CInputPad::Init(HINSTANCE hInstance, HWND hWnd)
{
	if (m_Input == nullptr)
	{
		// �C���v�b�g�N���X�̏����ݒ�
		if (FAILED(CInput::Init(hInstance, hWnd)))
		{
			return E_FAIL;
		}
	}

	// �v���X���N���A
	memset(&m_aKeyState, 0, sizeof(XINPUT_STATE));

	// �g���K�[���N���A
	memset(&m_aKeyStateTrigger, 0, sizeof(XINPUT_STATE));

	// �����[�X���N���A
	memset(&m_aKeyStateRelease, 0, sizeof(XINPUT_STATE));

	// XInput�̃X�e�[�g�ݒ��L���ɂ���
	XInputEnable(true);

	return S_OK;
}

//============================================================================
// �I������
//============================================================================
void CInputPad::Uninit()
{
	// XInput�̃X�e�[�g�ݒ�𖳌��ɂ���
	XInputEnable(false);

	// �C���v�b�g�N���X�̏I������
	CInput::Uninit();
}

//============================================================================
// �X�V����
//============================================================================
void CInputPad::Update()
{
	XINPUT_STATE StateJoypad = {};	// �p�b�h�̓��͏��

	// �p�b�h�̏�Ԃ��擾
	if (XInputGetState(0, &StateJoypad) == ERROR_SUCCESS)
	{
#if LEFT_JOYSTICK_CONVERT	// ���X�e�B�b�N���͂�ϊ�
		// ���X�e�B�b�N���͂��\���L�[�ɕϊ�
		g_state_joypad.press.Gamepad.wButtons |= ConvertJoyStick();
#else
		// �X�e�B�b�N���͂̒���
		AdjustJoyStick();
#endif	// LEFT_JOYSTICK_CONVERT

		// �p�b�h�̃����[�X����ۑ�
		m_aKeyStateRelease.Gamepad.wButtons = m_aKeyState.Gamepad.wButtons & ~StateJoypad.Gamepad.wButtons;

		// �p�b�h�̃g���K�[����ۑ�
		m_aKeyStateTrigger.Gamepad.wButtons = ~m_aKeyState.Gamepad.wButtons & StateJoypad.Gamepad.wButtons;
		m_aKeyStateTrigger.Gamepad.bRightTrigger = ~m_aKeyState.Gamepad.bRightTrigger & StateJoypad.Gamepad.bRightTrigger;
		
#if 0 // �p�b�h�̃v���X����ۑ�
		// �{�^���̂�
		m_aKeyState.Gamepad.wButtons = StateJoypad.Gamepad.wButtons;
#else
		// �W���C�X�e�B�b�N�E�g���K�[�{�^�����܂�
		m_aKeyState.Gamepad = StateJoypad.Gamepad;
#endif
	}
}

//============================================================================
// �v���X���擾
//============================================================================
bool CInputPad::GetPress(JOYKEY Key)
{
	return (m_aKeyState.Gamepad.wButtons & (0x01 << static_cast<int>(Key))) ? true : false;
}

//============================================================================
// �g���K�[���擾
//============================================================================
bool CInputPad::GetTrigger(JOYKEY Key)
{
	return (m_aKeyStateTrigger.Gamepad.wButtons & (0x01 << static_cast<int>(Key))) ? true : false;
}

//============================================================================
// �����[�X���擾
//============================================================================
bool CInputPad::GetRelease(JOYKEY Key)
{
	return (m_aKeyStateRelease.Gamepad.wButtons & (0x01 << static_cast<int>(Key))) ? true : false;
}

//============================================================
// �E�g���K�[�{�^���̓��͂��擾
//============================================================
bool CInputPad::GetDepthTriggerR()
{
	return (m_aKeyStateTrigger.Gamepad.bRightTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD);
}

//============================================================================
// ���X�e�B�b�N�̌X���擾
//============================================================================
CInputPad::JOYSTICK CInputPad::GetJoyStickL(void)
{
	// �X�e�B�b�N�̌X�����i�[
	JOYSTICK JoystickL =
	{
		m_aKeyState.Gamepad.sThumbLX,
		m_aKeyState.Gamepad.sThumbLY
	};

	return JoystickL;
}

//============================================================================
// �E�X�e�B�b�N�̌X���擾
//============================================================================
CInputPad::JOYSTICK CInputPad::GetJoyStickR(void)
{
	// �X�e�B�b�N�̌X�����i�[
	JOYSTICK JoystickR =
	{
		m_aKeyState.Gamepad.sThumbRX,
		m_aKeyState.Gamepad.sThumbRY
	};

	return JoystickR;
}

//============================================================================
// 
// private�����o
// 
//============================================================================

//============================================================================
// �X�e�B�b�N���͂̒���
//============================================================================
void CInputPad::AdjustJoyStick(void)
{
	// ��
	if (m_aKeyState.Gamepad.sThumbLX < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
		m_aKeyState.Gamepad.sThumbLX > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
		m_aKeyState.Gamepad.sThumbLY < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
		m_aKeyState.Gamepad.sThumbLY > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
	{
		m_aKeyState.Gamepad.sThumbLX = 0;
		m_aKeyState.Gamepad.sThumbLY = 0;
	}

	// �E
	if (m_aKeyState.Gamepad.sThumbRX < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
		m_aKeyState.Gamepad.sThumbRX > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
		m_aKeyState.Gamepad.sThumbRY < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
		m_aKeyState.Gamepad.sThumbRY > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
	{
		m_aKeyState.Gamepad.sThumbRX = 0;
		m_aKeyState.Gamepad.sThumbRY = 0;
	}
}