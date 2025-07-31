//============================================================================
// 
// �Q�O�Q�T�N�U���X���FOBB��������� [TutorialPlayer.h]
// Author : �Ŗ����z
// 
//============================================================================

//****************************************************
// �Q�d�C���N���[�h�h�~
//****************************************************
#ifndef _TUTORIALPLAYER_H_
#define _TUTORIALPLAYER_H_

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "player.h" // ���N���X������

//****************************************************
// �`���[�g���A���v���C���[�N���X
//****************************************************
class CTutorialPlayer : public CPlayer
{
public:

	// === ����֐� ===

	CTutorialPlayer();				// �R���X�g���N�^
	~CTutorialPlayer() override;	// �f�X�g���N�^

	// === ���C�t�T�C�N�������o�֐� ===

	HRESULT Init() override;	// �����ݒ�
	void	Uninit() override;	// �I������
	void	Update() override;	// �X�V����
	void	Draw() override;	// �`�揈��

	//// === �ÓI�����o�֐� ===

	static CTutorialPlayer* Create(D3DXVECTOR3 InitPos); // ����
private:
};

#endif // _PALYER_H_