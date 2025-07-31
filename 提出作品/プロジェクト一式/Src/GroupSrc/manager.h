//============================================================================
// 
// �}�l�[�W���[�A�w�b�_�t�@�C�� [managaer.h]
// Author : ���c����
// 
//============================================================================

#ifndef _MANAGER_H_
#define _MANAGER_H_	// ��d�C���N���[�h�h�~

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "light.h"
#include "input_keyboard.h"
#include "input_mouse.h"
#include "input_pad.h"

//****************************************************
// �}�l�[�W���[�N���X
//****************************************************
class CManager final
{
public:

	// <special function>
	CManager(const CManager&) = delete;				// �R�s�[�R���X�g���N�^
	CManager& operator=(const CManager&) = delete;	// �R�s�[������Z�q
	CManager(CManager&&) = delete;					// ���[�u�R���X�g���N�^
	CManager& operator=(CManager&&) = delete;		// ���[�u������Z�q

	// <function>
	void Update();	// �X�V����
	void Draw();	// �`�揈��

	// <getter>
	CLight*	GetLight() const;	// ���C�g���擾

	// <static function>
	static HRESULT Create(HINSTANCE hInstance, HWND hWnd);	// ����
	static void	Release();									// ���

	// <static getter>
	static CManager*		GetManager();	// �}�l�[�W���[���擾
	static CInputKeyboard*	GetKeyboard();	// �L�[�{�[�h���擾
	static CInputMouse*		GetMouse();		// �}�E�X���擾
	static CInputPad*		GetPad();		// �p�b�h���擾

private:

	// <special function>
	CManager();		// �R���X�g���N�^
	~CManager();	// �f�X�g���N�^

	// <function>
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);	// �����ݒ�
	void	InitImGui(HWND hWnd);					// ImGui�̏����ݒ�
	void	Uninit();								// �I������
	void	UninitImGui();							// ImGui�̏I������

	// <datas>
	CInputKeyboard*	m_pKeyboard;	// �L�[�{�[�h
	CInputMouse*	m_pMouse;		// �}�E�X
	CInputPad*		m_pPad;			// �p�b�h
	CLight*			m_pLight;		// ���C�g

	// <static data>
	static CManager* m_pManager;	// �}�l�[�W���[�̖{��
};

#endif // _MANAGER_H_
