//============================================================================
// 
// �Q�O�Q�T�N�T���R�O���F�v���C���[���V�[�����Ƃɔh�������� [TestScene.h]
// Author : ���c����
// 
//============================================================================

#ifndef _TestScene_H_
#define _TestScene_H_	// ��d�C���N���[�h�h�~

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
class CVisionBlockSpawner; // ����W�Q�I�u�W�F�N�g�X�|�i�[
class CEffectAccumulation;  // �u�C�v���~�σV�F�[�_�[

//****************************************************
// �Q�[���N���X
//****************************************************
class CTestScene : public CScene
{
public:

	// === ���C�t�T�C�N�������o�֐� ===

	void Update() override;		// �X�V����
	void Draw() override;		// �`�揈��

	// === �����o�֐� ===

	void To_Next() override; // ���̃V�[����

	// === �ÓI�����o�֐� ===

	static CTestScene* Create();	// �Q�[���𐶐�
	static bool& GetTest(); // �e�X�g�z�u���m�F���邩�ǂ���
private:
	// === �ÓI�����o�ϐ� ===
	static bool s_bTest; // �e�X�g�z�u���m�F���邩�ǂ���

	// === ����֐� ===
	CTestScene();			// �R���X�g���N�^
	~CTestScene() override;	// �f�X�g���N�^

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
	CEffectAccumulation* m_pEffectAccumulation; // �u�C�v�̒~�σV�F�[�_�[�̃|�C���^
	CToneAdjust* m_pToneAdjust;             // �F���ω��̃V�F�[�_�[�̃|�C���^
	CSmokeEffect* m_pSmokeEffect;           // ���̃V�F�[�_�[�̃|�C���^
};

#endif // _TestScene_H_