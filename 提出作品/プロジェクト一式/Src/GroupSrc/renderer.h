//============================================================================
// 
// �����_���[�A�w�b�_�t�@�C�� [renderer.h]
// Author : ���c����
// 
//============================================================================

#ifndef _RENDERER_H_
#define _RENDERER_H_	// ��d�C���N���[�h�h�~

//****************************************************
// �C���N���[�h
//****************************************************
#include "UiTextManager.h"

//****************************************************
// �O���錾
//****************************************************
class CObject;

//****************************************************
// �����_���[�N���X
//****************************************************
class CRenderer final
{
public:

	// <special function>
	CRenderer(const CRenderer&) = delete;				// �R�s�[�R���X�g���N�^
	CRenderer& operator=(const CRenderer&) = delete;	// �R�s�[������Z�q
	CRenderer(CRenderer&&) = delete;					// ���[�u�R���X�g���N�^
	CRenderer& operator=(CRenderer&&) = delete;			// ���[�u������Z�q

	// <function>
	void Update();		// �X�V����
	void Draw();		// �`�揈��
	void PrintDebug();	// �f�o�b�O�\��

	// UI�e�L�X�g�}�l�[�W���[
	CUITextManager& GetUiTextManager(); // �擾

	// �t�H�O�̎n�_���Z�b�g
	inline void SetFogStart(float start)
	{
		m_fFogStart = start;
		m_pD3DDevice->SetRenderState(D3DRS_FOGSTART, *(DWORD*)(&m_fFogStart));
	}

	// �t�H�O�̏I�_���Z�b�g
	inline void SetFogEnd(float end)
	{
		m_fFogEnd = end;
		m_pD3DDevice->SetRenderState(D3DRS_FOGEND, *(DWORD*)(&m_fFogEnd));
	}

	// �t�H�O�J���[���Z�b�g
	inline void SetFogCol(D3DXCOLOR col) { m_pD3DDevice->SetRenderState(D3DRS_FOGCOLOR, col); }

	// ��ԕ`��t���O�̃Z�b�g
	inline void SetDrawSpace() { m_bDrawSpace = true; }

	// <static function>
	static HRESULT	Create(HWND hWnd, BOOL bWindiw);	// ����
	static void		Release();							// �j��
	static void		CorrectFogStart(float fStart);		// �t�H�O�̎n�_����
	static void		CorrectFogEnd(float fEnd);			// �t�H�O�̏I�_����

	// <static getter>
	static CRenderer*			GetRenderer();	// �����_���[���擾
	static LPDIRECT3DDEVICE9	GetDeviece();	// �f�o�C�X���擾

	// <static setter>
	static void	SetDebugString(std::string Str);			// �f�o�b�O������ɒǉ�
	static void	SetTimeString(std::string Str, int nCnt);	// �������f�o�b�O������ɒǉ�

private:

	//****************************************************
	// �ÓI�萔���`
	//****************************************************
	static constexpr float FOG_SRART = 1500.0f;	// �t�H�O�̏����n�_
	static constexpr float FOG_END = 2500.0f;	// �t�H�O�̏����I�_

	// <special function>
	CRenderer();	// �R���X�g���N�^
	~CRenderer();	// �f�X�g���N�^

	// <function>
	HRESULT Init(HWND hWnd, BOOL bWindiw);	// �����ݒ�
	void	Uninit();						// �I������

	void DrawTextMesh(CObject& ref);	// �e�L�X�g���b�V���p�̕`�揈��
	void DrawTexture(CObject& ref);		// �e�N�X�`���p�̕`�揈��

	// <data>
	LPDIRECT3D9									m_pD3D;			// Direct3D
	LPDIRECT3DDEVICE9							m_pD3DDevice;	// �f�o�C�X
	float										m_fFogStart;	// �t�H�O�̎n�_
	float										m_fFogEnd;		// �t�H�O�̏I�_
	LPD3DXFONT									m_pFont;		// �t�H���g
	LPD3DXFONT									m_pDebugFont;	// �f�o�b�O�t�H���g
	std::string									m_DebugStr;		// �\���p������
	std::vector<std::pair<std::string, int>>	m_TimeStr;		// ������������
	CUITextManager                              m_UiTextManager; // UI�e�L�X�g�}�l�[�W���[

	bool m_bDrawSpace;	// ��ԕ`��t���O

	// <static data>
	static CRenderer* m_pRenderer;	// �����_���[�̖{��
};

#endif // _RENDERER_H_