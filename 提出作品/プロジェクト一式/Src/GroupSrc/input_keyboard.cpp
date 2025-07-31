//============================================================================
// 
// �L�[�{�[�h���� [input_keyboard.cpp]
// Author : ���c����
// 
//============================================================================

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "input_keyboard.h"

//============================================================================
// 
// public�����o
// 
//============================================================================

//============================================================================
// �R���X�g���N�^
//============================================================================
CInputKeyboard::CInputKeyboard() :
	m_aKeyState{},
	m_aKeyStateTrigger{},
	m_aKeyStateRelease{}
{

}

//============================================================================
// �f�X�g���N�^
//============================================================================
CInputKeyboard::~CInputKeyboard()
{

}

//============================================================================
// �����ݒ�
//============================================================================
HRESULT CInputKeyboard::Init(HINSTANCE hInstance, HWND hWnd)
{
	if (m_Input == nullptr)
	{
		// �C���v�b�g�N���X�̏����ݒ�
		if (FAILED(CInput::Init(hInstance, hWnd)))
		{
			return E_FAIL;
		}
	}

	// ���̓f�o�C�X(�L�[�{�[�h)�̐���
	if (m_pDevice == nullptr)
	{
		if (FAILED(m_Input->CreateDevice(GUID_SysKeyboard, &m_pDevice, nullptr)))
		{
			return E_FAIL;
		}
	}

	// �f�[�^�t�H�[�}�b�g�̐ݒ�
	if (FAILED(m_pDevice->SetDataFormat(&c_dfDIKeyboard)))
	{
		return E_FAIL;
	}

	// �������[�h��ݒ�F�t�H�A�O���E���h
	if (FAILED(m_pDevice->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	// �L�[�{�[�h�ւ̃A�N�Z�X�����l��
	m_pDevice->Acquire();

	return S_OK;
}

//============================================================================
// �I������
//============================================================================
void CInputKeyboard::Uninit()
{
	// �C���v�b�g�N���X�̏I������
	CInput::Uninit();
}

//============================================================================
// �X�V����
//============================================================================
void CInputKeyboard::Update()
{
	BYTE StateKeyboard[MAX_KEY] = {};	// �L�[���͏��

	// ���̓f�o�C�X����f�[�^���擾
	if (SUCCEEDED(m_pDevice->GetDeviceState(sizeof(StateKeyboard), &StateKeyboard[0])))
	{
		for (WORD i = 0; i < MAX_KEY; ++i)
		{
			// �L�[�{�[�h�̃����[�X����ۑ�
			m_aKeyStateRelease[i] = m_aKeyState[i] & ~StateKeyboard[i];

			// �L�[�{�[�h�̃g���K�[����ۑ�
			m_aKeyStateTrigger[i] = ~m_aKeyState[i] & StateKeyboard[i];

			// �L�[�{�[�h�̃v���X����ۑ�
			m_aKeyState[i] = StateKeyboard[i];
		}
	}
	else
	{
		// �L�[�{�[�h�̃A�N�Z�X�����l��
		m_pDevice->Acquire();
	}
}

//============================================================================
// �v���X���擾
//============================================================================
bool CInputKeyboard::GetPress(int nKey)
{
	return ((m_aKeyState[nKey] & 0x80) != 0) ? true : false;
}

//============================================================================
// �g���K�[���擾
//============================================================================
bool CInputKeyboard::GetTrigger(int nKey)
{
	return ((m_aKeyStateTrigger[nKey] & 0x80) != 0) ? true : false;
}

//============================================================================
// �����[�X���擾
//============================================================================
bool CInputKeyboard::GetRelease(int nKey)
{
	return ((m_aKeyStateRelease[nKey] & 0x80) != 0) ? true : false;
}