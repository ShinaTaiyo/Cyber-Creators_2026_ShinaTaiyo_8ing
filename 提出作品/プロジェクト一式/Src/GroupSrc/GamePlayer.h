//============================================================================
// 
// �Q�O�Q�T�N�U���X���FOBB��������� [GamePlayer.h]
// Author : �Ŗ����z
// 
//============================================================================

//****************************************************
// �Q�d�C���N���[�h�h�~
//****************************************************
#ifndef _GAMEPLAYER_H_
#define _GAMEPLAYER_H_

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "player.h" // ���N���X������

//****************************************************
// �O���錾
//****************************************************
class CTarget;        // �^�[�Q�b�g���
class CMove_Info;     // �ړ����
class CCollisionInfo; // �R���W�������
class CCollisionInfo_BoundingBox;      // ��������
class CCollisionInfo_BoundingCylinder; // �~��������
class COBB; // OBB���
class CPlayerState; // �v���C���[���

//****************************************************
// �Q�[���v���C���[�N���X
//****************************************************
class CGamePlayer : public CPlayer
{
public:

	// === ����֐� ===

	CGamePlayer();				// �R���X�g���N�^
	~CGamePlayer() override;	// �f�X�g���N�^

	// === ���C�t�T�C�N�������o�֐� ===

	HRESULT Init() override;	// �����ݒ�
	void	Uninit() override;	// �I������
	void	Update() override;	// �X�V����
	void	Draw() override;	// �`�揈��

	//// === �ÓI�����o�֐� ===

	static CGamePlayer* Create(D3DXVECTOR3 InitPos); // ����
private:
};

#endif // _PALYER_H_