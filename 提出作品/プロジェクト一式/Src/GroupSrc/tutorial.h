//============================================================================
// 
// �Q�O�Q�T�N�T���R�O���F�v���C���[���V�[�����Ƃɔh�������� [Tutorial.h]
// Author : ���c����
// 
//============================================================================

#ifndef _TUTORIAL_H_
#define _TUTORIAL_H_	// ��d�C���N���[�h�h�~

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
class CTutorialState;  // �`���[�g���A����ԃN���X
class CTutorialPlayer; // �Q�[���v���C���[
class CVisionBlockSpawner; // ����W�Q�I�u�W�F�N�g�X�|�i�[
class CEffectAccumulation;  // �u�C�v���~�σV�F�[�_�[

//****************************************************
// �Q�[���N���X
//****************************************************
class CTutorial : public CScene
{
public:

	// === ���C�t�T�C�N�������o�֐� ===

	void Update() override;		// �X�V����
	void Draw() override;		// �`�揈��

	// === �����o�֐� ===

	void To_Next() override; // ���̃V�[����
	void SetState(CTutorialState* pState); // ��Ԑݒ�

	// === �ÓI�����o�֐� ===

	static CTutorial* Create();	// �Q�[���𐶐�
private:

	// === ����֐� ===
	CTutorial();			// �R���X�g���N�^
	~CTutorial() override;	// �f�X�g���N�^

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

	// === �����o�ϐ� ===

	bool m_bEnd;                            // �Q�[���V�[���I���t���O
	int m_nCntFrame;                        // �t���[�������J�E���g����
	bool m_bEditSummon;                     // ��������I�u�W�F�N�g��ώ킷��
	CEffectAccumulation* m_pEffectAccumulation; // �u�C�v�̒~�σV�F�[�_�[�̃|�C���^
	CToneAdjust* m_pToneAdjust;             // �F���ω��̃V�F�[�_�[�̃|�C���^
	CTutorialState* m_pState;               // ��Ԃւ̃|�C���^
	CSmokeEffect* m_pSmokeEffect;           // ���̃V�F�[�_�[�̃|�C���^
};

#endif // _Tutorial_H_