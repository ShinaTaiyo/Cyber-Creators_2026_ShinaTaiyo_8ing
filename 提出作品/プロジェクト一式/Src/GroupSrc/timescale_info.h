//============================================================================
// 
// �Q�O�Q�T�N�T���P�T���F�^�C���X�P�[�����N���X���쐬 [timescale_info.h]
// Author : �Ŗ����z
// 
//============================================================================

#ifndef _TIMESCALE_INFO_H_
#define _TIMESCALE_INFO_H_	// ��d�C���N���[�h�h�~

//****************************************************
// using�f�B���N�e�B�u
//****************************************************
using namespace useful;

//****************************************************
// �^�C���X�P�[�����N���X
//****************************************************
class CTimeScale_Info
{
public:
	// === ����֐� ===

	CTimeScale_Info();  // �R���X�g���N�^
	~CTimeScale_Info(); // �f�X�g���N�^

	// === ���C�t�T�C�N�������o�֐� ===

	void Update(); // �X�V����

	// === �����o�֐� ===

	// �^�C���X�P�[��
	void SetTimeScale(float fTimeScale); // �ݒ�
	const float& GetTimeScale() const;   // �擾

	// �ړI�̃^�C���X�P�[���ɋ߂Â���
	void SetGoalTimeScale(float fGoalTimeScale, float fAdjust);

	// �^�C���X�P�[�����w��t���[���������ς���悤�ɐݒ�
	void SetTimeScaleChangeFrame(float fTimeScale, int nChangeFrame);

private:
	// === �����o�ϐ� ===

	int 
		m_nChangeFrame,           // �^�C���X�P�[����ς���t���[����
        m_nChangeFrameCnt;        // �^�C���X�P�[����ς���t���[�����J�E���g
	float m_fTimeScale;           // �^�C���X�P�[���l
	float m_fAdjustGoalTimeScale; // �ړI�̃^�C���X�P�[���ɋ߂Â���␳�l
	float m_fGoalTimeScale;       // �ړI�̃^�C���X�P�[��
	bool m_bUseChangeFrame;       // �^�C���X�P�[����ς���t���[�������g�p���邩�ǂ���
	bool m_bUseGoalTimeScale;     // �ړI�̃^�C���X�P�[�����g�p���邩�ǂ���

	// === �����o�֐� ===

	void TimeScaleChangeFrame(); // �^�C���X�P�[�����w��t���[���������ς��鏈��
	void GoalTimeScale(); // �ړI�̃^�C���X�P�[���ɋ߂Â���
	void ResetInfo(); // ��񏉊���
};
#endif // _Summon_MANAGER_H_