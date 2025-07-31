//============================================================================
// 
// �Q�O�Q�T�N�T���P�P���F�����}�l�[�W���[����� [summon_manager.h]
// Author : �Ŗ����z
// 
//============================================================================

#ifndef _SUMMON_MANAGER_H_
#define _SUMMON_MANAGER_H_	// ��d�C���N���[�h�h�~

//****************************************************
// �C���N���[�h
//****************************************************
#include "object.h" 

//****************************************************
// �O���錾
//****************************************************
class CSummon_Manager_State; // �����}�l�[�W���[��ԃN���X

//****************************************************
// using�f�B���N�e�B�u
//****************************************************
using namespace useful;

//****************************************************
// �����}�l�[�W���[�N���X
//****************************************************
class CSummon_Manager : public CObject // �p�����Ȃ�
{
public:
	// === �񋓌^ ===

    // �������
	struct SummonInfo
	{
		std::string Name;              // ���O
		int nSummonFrame = 0;          // �����t���[��
		D3DXVECTOR3 Pos = VEC3_INIT;   // �ʒu
		D3DXVECTOR3 Move = VEC3_INIT;  // �ړ����� 
		D3DXVECTOR3 Scale = VEC3_INIT; // �g�嗦
	};

	// ��Փx
	enum class DIFFICULTY
	{
		EASY = 0, // �ȒP
		NORMAL,   // ����
		HARD,     // ���
		EXPERT,   // �����Y
		MAX
	};

	// �����O���[�v
	enum class SUMMONGROUP
	{
		PATTERN_1 = 0,
		PATTERN_2,
		PATTERN_3,
		PATTERN_4,
		PATTERN_5,
		PATTERN_6,
		PATTERN_7,
		PATTERN_8,
		PATTERN_11,
		TAIYO_1,
		MAX
	};

	// === ����֐� ===

	CSummon_Manager();  // �R���X�g���N�^
	~CSummon_Manager(); // �f�X�g���N�^

	// === ���C�t�T�C�N�������o�֐� ===

	HRESULT	Init() override; // ����������
	void Uninit() override;  // �I������
	void Update() override;  // �X�V����
	void Draw() override;    // �`�揈��

	// === �����o�֐� ===

	// �X�e�[�g
	void SetChengeState(CSummon_Manager_State* pState); // �ύX
	CSummon_Manager_State* GetState() const; // �擾

	// === �ÓI�����o�֐� ===
	static CSummon_Manager* Create();
	static void AddNumObj(); // �����}�l�[�W���[�̃I�u�W�F�N�g���𑝂₷
	static void SubNumObj(); // �����}�l�[�W���[�̃I�u�W�F�N�g�������炷
	static int GetNumObj();  // �����}�l�[�W���[�����I�u�W�F�N�g���擾
	static void StopSummonObj(); // �����I�u�W�F�N�g�̓������~�߂�
private:	
	// === �ÓI�����o�ϐ� ===

	static int s_nNumObj; // �����I�u�W�F�N�g�����J�E���g����

	// === �����o�ϐ� ===

	CSummon_Manager_State* m_pSummonManagerState; // �����}�l�[�W���[�X�e�[�g
};
#endif // _Summon_MANAGER_H_