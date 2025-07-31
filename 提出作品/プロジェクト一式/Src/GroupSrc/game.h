//============================================================================
// 
// �Q�O�Q�T�N�T���R�O���F�v���C���[���V�[�����Ƃɔh�������� [game.h]
// Author : ���c����
// 
//============================================================================

#ifndef _GAME_H_
#define _GAME_H_	// ��d�C���N���[�h�h�~

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "scene.h"
#include "summon_manager.h"
#include "timer.h" // �^�C�}�[
#include "EffectAccumulation.h" // �u�C�v�̒~�σV�F�[�_�[
#include "ToneAdjust.h"  // �F���ω��̃V�F�[�_�[
#include "SmokeEffect.h"  // ���̃V�F�[�_�[

//****************************************************
// �O���錾
//****************************************************
class CGameState;  // �Q�[����ԃN���X
class CGamePlayer; // �Q�[���v���C���[
class CVisionBlockSpawner; // ����W�Q�I�u�W�F�N�g�X�|�i�[
class CEffectAccumulation;  // �u�C�v���~�σV�F�[�_�[

class CSummon_Manager_State_Start;
class CObject_X;
class CMotion_Set;
class CSpace;			// ��Ԑ��䏈���p
class CSwayingPlayer;	// �h��Đ������p

//****************************************************
// �Q�[���N���X
//****************************************************
class CGame : public CScene
{
	//****************************************************
	// �ÓI�����o�萔�̐錾
	//****************************************************
	static const D3DXVECTOR3 POS_HARISENBON;	// �n���Z���{���̍��W

public:

	// === ���C�t�T�C�N�������o�֐� ===

	void Update() override;		// �X�V����
	void Draw() override;		// �`�揈��

	// === �����o�֐� ===

	void To_Next() override; // ���̃V�[����

	// === �ÓI�����o�֐� ===

	static CGame* Create();	// �Q�[���𐶐�

	// �n���Z���{���̍��W���擾
	inline const D3DXVECTOR3& GetPosHarisenbon() { return POS_HARISENBON; }

private:
	// === ����֐� ===
	CGame();			// �R���X�g���N�^
	~CGame() override;	// �f�X�g���N�^

	// === �\���� ===

	// ��{�������
	struct SummonInfo
	{
		std::string Name;                       // ���O
	    D3DXVECTOR3 Pos = { 0.0f,0.0f,0.0f };   // �ʒu
		D3DXVECTOR3 Move = { 0.0f,0.0f,0.0f };  // �ړ���
	    D3DXVECTOR3 Scale = { 0.0f,0.0f,0.0f }; // �g�嗦
		int nSummonFrame = 0;                   // �����t���[��
	};

	// === ���C�t�T�C�N�������o�֐� ===

	HRESULT Init() override;	            // �����ݒ�
	void	Uninit() override;	            // �I������

	// === �����o�֐� ===

	void GameOver();	// �Q�[���I�[�o�[
	void UpdateInCooltime(const CSummon_Manager_State_Start& ref);	// �N�[���^�C�����̍X�V
	void UpdateMountain(int nowPhase);	// �R�X�V

	// === �����o�ϐ� ===

	bool m_bEnd;                            // �Q�[���V�[���I���t���O
	int m_nCntFrame;                        // �t���[�������J�E���g����
	bool m_bEditSummon;                     // ��������I�u�W�F�N�g��ώ킷��
	CEffectAccumulation* m_pEffectAccumulation; // �u�C�v�̒~�σV�F�[�_�[�̃|�C���^
	CToneAdjust* m_pToneAdjust;             // �F���ω��̃V�F�[�_�[�̃|�C���^
	CSmokeEffect* m_pSmokeEffect;           // ���̃V�F�[�_�[�̃|�C���^

	CMotion_Set*					m_pEnemy;		// �G���[�V����
	CMotion_Set*					m_pFakeField;	// �^�������[�V����
	CObject_X*						m_pMountain;	// �R���f��
	std::unique_ptr<CSwayingPlayer>	m_upSwaying;	// �h��Đ�����������
};

#endif // _GAME_H_