//============================================================================
// 
// �}�E�X���� [input_mouse.cpp]
// Author : ���c����
// 
//============================================================================

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "input_mouse.h"

//============================================================================
// 
// public�����o
// 
//============================================================================

//============================================================================
// �R���X�g���N�^
//============================================================================
CInputMouse::CInputMouse() :
	m_State{},
	m_StateTrigger{},
	m_StateRelease{}
{

}

//============================================================================
// �f�X�g���N�^
//============================================================================
CInputMouse::~CInputMouse()
{

}

//============================================================================
// �����ݒ�
//============================================================================
HRESULT CInputMouse::Init(HINSTANCE hInstance, HWND hWnd)
{
	if (m_Input == nullptr)
	{
		// �C���v�b�g�N���X�̏����ݒ�
		if (FAILED(CInput::Init(hInstance, hWnd)))
		{
			return E_FAIL;
		}
	}

	// ���̓f�o�C�X(�}�E�X)�̐���
	if (FAILED(m_Input->CreateDevice(GUID_SysMouse, &m_pDevice, NULL)))
	{
		return E_FAIL;
	}

	// �f�[�^�t�H�[�}�b�g�̐ݒ�
	if (FAILED(m_pDevice->SetDataFormat(&c_dfDIMouse2)))
	{
		return E_FAIL;
	}

	// �������[�h��ݒ�F�t�H�A�O���E���h
	if (FAILED(m_pDevice->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	// �}�E�X�ւ̃A�N�Z�X�����l��
	m_pDevice->Acquire();

	return S_OK;
}

//============================================================================
// �I������
//============================================================================
void CInputMouse::Uninit()
{
	// ���̓f�o�C�X(�}�E�X)�̔j��
	if (m_pDevice != nullptr)
	{
		m_pDevice->Unacquire();
		m_pDevice->Release();
		m_pDevice = nullptr;
	}
}

//============================================================================
// �X�V����
//============================================================================
void CInputMouse::Update()
{
	DIMOUSESTATE2 StateMouse = {};	// �}�E�X���͏��

	// ���̓f�o�C�X����f�[�^���擾
	if (SUCCEEDED(m_pDevice->GetDeviceState(sizeof(DIMOUSESTATE2), &StateMouse)))
	{
		for (WORD wCntKey = 0; wCntKey < 8; ++wCntKey)
		{
			// �}�E�X�̃����[�X����ۑ�
			m_StateRelease.rgbButtons[wCntKey] = m_State.rgbButtons[wCntKey] & ~StateMouse.rgbButtons[wCntKey];

			// �}�E�X�̃g���K�[����ۑ�
			m_StateTrigger.rgbButtons[wCntKey] = ~m_State.rgbButtons[wCntKey] & StateMouse.rgbButtons[wCntKey];

			// �}�E�X�̃v���X����ۑ�
			m_State.rgbButtons[wCntKey] = StateMouse.rgbButtons[wCntKey];
		}

		// �}�E�X�̈ړ�����ۑ�
		m_State = StateMouse;
	}
	else
	{
		// �}�E�X�̃A�N�Z�X�����l��
		m_pDevice->Acquire();
	}
}

//============================================================================
// �v���X���擾
//============================================================================
bool CInputMouse::GetPress(int nKey)
{
	return ((m_State.rgbButtons[nKey] & 0x80) != 0) ? true : false;
}

//============================================================================
// �g���K�[���擾
//============================================================================
bool CInputMouse::GetTrigger(int nKey)
{
	return ((m_StateTrigger.rgbButtons[nKey] & 0x80) != 0) ? true : false;
}

//============================================================================
// �����[�X���擾
//============================================================================
bool CInputMouse::GetRelease(int nKey)
{
	return ((m_StateRelease.rgbButtons[nKey] & 0x80) != 0) ? true : false;
}

//============================================================================
// �}�E�X�̈ړ��ʂ��擾
//============================================================================
D3DXVECTOR2 CInputMouse::GetMouseMove()
{
	return { static_cast<float>(m_State.lX), static_cast<float>(m_State.lY) };
}

//============================================================================
// �z�C�[���̃X�N���[���ʂ��擾
//============================================================================
float CInputMouse::GetWheelScroll()
{
	return static_cast<float>(m_State.lZ);
}