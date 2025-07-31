//============================================================================
// 
// �Q�O�Q�T�N�T���P�P���F�����}�l�[�W���[��ԃN���X����� [summon_Manager_state_start.h]
// Author : �Ŗ����z
// 
//============================================================================

#ifndef _SUMMON_MANAGER_STATE_START_H_
#define _SUMMON_MANAGER_STATE_START_H_	// ��d�C���N���[�h�h�~

//****************************************************
// �C���N���[�h
//****************************************************
#include "summon_manager_state.h"

//****************************************************
// using�f�B���N�e�B�u
//****************************************************
using namespace useful;

//****************************************************
// �O���錾
//****************************************************
class CSummonGroupSelector; // �����O���[�v�I���N���X

//****************************************************
// �����}�l�[�W���[�X�e�[�g�F�J�n�N���X
//****************************************************
class CSummon_Manager_State_Start : public CSummon_Manager_State
{
public:

	// <special function>
	CSummon_Manager_State_Start();           // �f�t�H���g�R���X�g���N�^
	CSummon_Manager_State_Start(int nMaxPhase);           // �R���X�g���N�^
	~CSummon_Manager_State_Start() override; // �f�X�g���N�^

	// <function>
	void Update(CSummon_Manager* pManager) override; // �X�V����

	// �t�F�[�Y�ɏ����������ǂ���
	const bool& GetSuccessPhase() const; // �擾
	void SetSuccessPhase(bool bSuccess); // �ݒ�

	// �t�F�[�Y�۔���t���[�����ǂ���
	const bool& GetPhaseJudgmentFrame() const; // �擾

	// �t�F�[�Y���J�n�������ǂ���
	const bool& GetStartPhase() const; // �擾

	// �S�Ẵt�F�[�Y���I���������ǂ���
	const bool& GetAllPhaseComplete() const; // �擾

	// �����N�[���^�C�����ǂ���
	const bool& GetSummonCoolTime() const; // �擾

	// �A�������񐔂̎擾
	int GetPhaseComboCnt() const { return m_nPhaseComboCnt; }

	// ���݂̏����񐔂̎擾
	int GetNowPhase() const { return m_nNumPhase; }

	// �����N�[���^�C���J�E���g���擾
	int GetSummonCoolTimeCnt() const { return m_nSummonCoolTimeCnt; };

	CSummonGroupSelector* GetSummonGroupSelector() const; // �����O���[�v�I���I�u�W�F�N�g���擾

	// <staticfunction>
	static const int& GetMaxSummonCoolTime(); // �ő叢���N�[���^�C�����擾����
private:
	// === �ÓI�����o�ϐ� ===

	static constexpr int
		s_nMaxRandomSummonRate = 120, // �����_�������ő僌�[�g
		s_nInitialSummonRate = 90,    // ���������_���������[�g
		s_nMaxSummonCoolTime = 120,    // �ő叢���N�[���^�C��
		s_nNormalSlowFrame = 90,      // �ʏ�X���[����
		s_nNormalMaxPhase = 20,       // �ʏ�ő�t�F�[�Y��
		s_nMaxPlayerControlFrame = s_nNormalSlowFrame,    // �ʏ�̃v���C���[����\�ő�t���[�����i�X���[���Ԃ����j
		s_nAddDifficultyScore = 4; // �t�F�[�Y�N���A���ɉ��Z�����Փx�X�R�A

	// === �����o�ϐ� ===

	bool
		m_bSuccessPhase, // �t�F�[�Y�ɏ����������ǂ���
		m_bPhaseJudgmentFrame,   // �t�F�[�Y�۔���t���[�������ǂ���
		m_bStartPhase,      // �t�F�[�Y�J�n
		m_bAllPhaseComplete, // �S�Ẵt�F�[�Y���I���������ǂ���
		m_bSummonCoolTime;  // �����N�[���^�C�������ǂ���
	int
		m_nSummonCoolTimeCnt,   // �����ɃN�[���^�C������������
		m_nSummonNum,           // ������
		m_nMaxPhase,            // �ő�t�F�[�Y��
		m_nPhaseComboCnt,       // �t�F�[�Y�A�������񐔃J�E���g
		m_nNumPhase,            // ���t�F�[�Y�ڂ�
		m_nSummonCoolTimeFrame; // �����N�[���^�C���̃t���[����
	CSummonGroupSelector* m_pSummonGroupSelector; // �����O���[�v�ԍ���I������I�u�W�F�N�g

	// === �����o�֐� ===

	bool RandumSummon();  // �����_������
	bool CoolTimeAfterChooseSummon(); // �����N�[���^�C����ɑI���O���[�v����������
	void VisionBlockerEvent() override; // ����W�Q�I�u�W�F�N�g�C�x���g�istart��Ԃł́A���݂̓�Փx�ɂ���ėʂ��ς��j
	bool SetSummonGroup(int nSummonGroup) override; // �O���[�v����
};
#endif // _Summon_MANAGER_H_