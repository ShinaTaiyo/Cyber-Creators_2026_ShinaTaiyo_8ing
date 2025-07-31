//============================================================================
// 
// ���͏����Ǘ��A�w�b�_�t�@�C�� [input.h]
// Author : ���c����
// 
//============================================================================

#ifndef _INPUT_H_
#define _INPUT_H_	// ��d�C���N���[�h�h�~

//****************************************************
// �}�N����`
//****************************************************
#define MAX_KEY	256	// �L�[�ő吔

//****************************************************
// �C���v�b�g�N���X
//****************************************************
class CInput
{
public:

	// <special function>
	CInput();	// �R���X�g���N�^
	 ~CInput();	// �f�X�g���N�^

	// <function>
	virtual HRESULT Init(HINSTANCE hInstance, HWND hWnd);	// �����ݒ�
	virtual void	Uninit();								// �I������
	virtual void	Update() = 0;							// �X�V����

protected:

	// <data>
	LPDIRECTINPUTDEVICE8 m_pDevice;	// ���̓f�o�C�X���

	// <static data>
	static LPDIRECTINPUT8 m_Input;	// DirectInput�I�u�W�F�N�g
};

#endif // _INPUT_H_