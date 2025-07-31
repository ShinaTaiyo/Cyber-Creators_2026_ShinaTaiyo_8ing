//============================================================================
// 
// �Q�O�Q�T�N�V���Q�R���FUI�p�e�L�X�g��`�悷�� [UIText.h]
// Author : �Ŗ����z
// 
//============================================================================

//****************************************************
// �Q�d�C���N���[�h�h�~
//****************************************************
#ifndef _UITEXT_H_
#define _UITEXT_H_

//****************************************************
// �O���錾
//****************************************************
class CObject_2D;

//****************************************************
// UI�e�L�X�g�N���X
//****************************************************
class CUIText
{
public:
	// === ����֐� ===

	// �R���X�g���N�^
	CUIText(
	    std::string Str, // ����
		int PosX, // �ʒuX
		int PosY, // �ʒuY
		int nFontSize, // �t�H���g�T�C�Y
		int nDispFrame, // �\���t���[����
		D3DXCOLOR Col, // �F����
		D3DXCOLOR BgColor // �e�L�X�g�w�i�̐F����
	);  
	~CUIText();	// �f�X�g���N�^

	// === ���C�t�T�C�N�������o�֐� ===

	void Update();  // �X�V����
	void Draw();    // �`�揈��

	// === �����o�֐� ===

	// �e�L�X�g�̉���
	const int& GetTextWidth() const; // �擾

	// �e�L�X�g�̍���
	const int& GetTextHeight() const; // �擾

	// �ʒuX
	const int& GetPosX() const; // �擾

	// �ʒuY
	const int& GetPosY() const; // �擾

	// �\���t���[����
	const int& GetDispFrame() const; // �擾

private:
	int m_nPosX;        // �ʒuX
	int m_nDispFrame;   // �\���t���[����
	int m_nPosY;        // �ʒuY
	int m_nFontSize;    // �t�H���g�T�C�Y
	LPD3DXFONT m_pFont; // �t�H���g
	std::string m_Str;  // ����
	D3DXCOLOR m_Col;    // �F����
	int m_nTextWidth;   // �e�L�X�g�̉���
	int m_nTextHeight;  // �e�L�X�g�̍���
	CObject_2D* m_pTextBG; // �e�L�X�g�w�i
};

#endif // _PALYER_H_