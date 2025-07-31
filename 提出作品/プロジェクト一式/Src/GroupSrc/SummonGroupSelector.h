//============================================================================
// 
// �Q�O�Q�T�N�U���T���F�����O���[�v�Ǘ��N���X����� [SummonGroupSelector.h]
// Author : �Ŗ����z
// 
//============================================================================

//****************************************************
// 2�d�C���N���[�h�h�~
//****************************************************
#ifndef _SUMMONGROUPSELECTOR_H_
#define _SUMMONGROUPSELECTOR_H_	// ��d�C���N���[�h�h�~

//****************************************************
// �C���N���[�h
//****************************************************
#include "summon_manager.h" // �����}�l�[�W���[�̏����g�p

//****************************************************
// �����O���[�v�I���N���X
//****************************************************
class CSummonGroupSelector
{
public:
	// === �񋓌^ ===

	// �C�x���g
	enum class EVENT
	{
		NONE = 0,
		LEAF,
		SILHOUETTE,
		MAX
	};

	// === ����֐� ===

	CSummonGroupSelector();  // �R���X�g���N�^
	~CSummonGroupSelector(); // �f�X�g���N�^

	// === ���C�t�T�C�N�������o�֐� ===

	void Update(); // �X�V����

	// === �����o�֐� ===

	// �t�F�[�Y������
	const int& GetSuccessPhaseNum() const; // �擾
	void SetSuccessPhaseNum(int nNum);     // �ݒ�

	// �蓁�g�p��
	const int& GetNumUseHandSword() const; // �擾
	void SetNumUseHandSword(int nNum);     // �ݒ�

	// ��Փx�X�R�A�ݒ�
	const int& GetDifficultyScore() const; // �擾
	void SetDifficultyScore(int nScore);   // �ݒ�

	// �����O���[�v�ԍ�
	const int& GetSummonGroupNum() const;   // �擾
	void SetSummonGroupNum(int nNum); // �ݒ�

	void DecideSummonGroup();    // �����O���[�v�����肷��

	const CSummon_Manager::DIFFICULTY& GetDifficultyRank() const; // ���݂̓�Փx�����N�̎擾

	static const int& GetMaxDifficultyScore(); // ��Փx�X�R�A�ő�l���擾
private:
	// === �ÓI�����o�ϐ� ===

	// ��Փx���㏸������{�[�_�[�ƂȂ�X�R�A
	static constexpr int
		s_nDifficultyBorderScore[static_cast<int>(CSummon_Manager::DIFFICULTY::MAX)] =
	{
		1,20,40,80
	},
		s_nMaxDifficultyScore = 100; // ��Փx�ő�X�R�A

	// === �����o�ϐ� ===

	int
		m_nSuccessPhaseNum, // �t�F�[�Y������
		m_nNumUseHandSword, // �蓁�g�p��
		m_nDifficultyScore, // ��Փx���㏸������X�R�A
		m_nSummonGroupNum;  // �����O���[�v�ԍ�
	CSummon_Manager::DIFFICULTY m_DifficultyRank; // ��Փx�����N

	// === �����o�֐� ===

	void UpdateDifficultyRank();  // ��Փx�����N���X�V����
};
#endif // _Summon_MANAGER_H_