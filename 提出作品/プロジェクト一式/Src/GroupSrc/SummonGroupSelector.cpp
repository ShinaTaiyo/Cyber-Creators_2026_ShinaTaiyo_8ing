//============================================================================
// 
// �Q�O�Q�T�N�U���T���F�����O���[�v�I���N���X���쐬���� [SummonGroupSelector.cpp]
// Author : �Ŗ����z
// 
//============================================================================

//****************************************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "SummonGroupSelector.h"  // ���w�b�_
#include "summon_manager_state.h" // �����}�l�[�W���[�X�e�[�g
#include "game.h" // �Q�[���V�[��
#include "manager.h"     // �}�l�[�W���[
#include "object.h"      // �I�u�W�F�N�g
#include "renderer.h"    // �f�o�b�O�p
#include "calculation.h" // �v�Z�p
#include "VisionBlockSpawner.h" // ����W�Q�I�u�W�F�N�g�X�|�i�[

//****************************************************
// using�f�B���N�e�B�u
//****************************************************
using namespace useful;
using namespace std;

//============================================================================
//
// public�����o
//
//============================================================================

//============================================================================
// �R���X�g���N�^
//============================================================================
CSummonGroupSelector::CSummonGroupSelector() :
	m_nSuccessPhaseNum{ 0 },
	m_nNumUseHandSword{ 0 },
	m_nDifficultyScore{ 0 },
	m_DifficultyRank{CSummon_Manager::DIFFICULTY::EASY}
{
	// �����Ȃ�
}

//============================================================================
// �f�X�g���N�^
//============================================================================
CSummonGroupSelector::~CSummonGroupSelector()
{
	// �����Ȃ�
}

//============================================================================
// �X�V����
//============================================================================
void CSummonGroupSelector::Update()
{
	UpdateDifficultyRank(); // ��Փx�����N�X�V

	// ��Փx�X�R�A��͈͐���
	m_nDifficultyScore = Calculation::Value::LimitRange(m_nDifficultyScore, 0, s_nMaxDifficultyScore);

	// �f�o�b�O�\��
	CRenderer::SetDebugString("��Փx�X�R�A�F" + to_string(m_nDifficultyScore));
	CRenderer::SetDebugString("��Փx�����N�F" + to_string(static_cast<int>(m_DifficultyRank)));}

//============================================================================
// �t�F�[�Y�����񐔂��擾
//============================================================================
const int& CSummonGroupSelector::GetSuccessPhaseNum() const
{
	return m_nSuccessPhaseNum;
}

//============================================================================
// �t�F�[�Y�����񐔂�ݒ�
//============================================================================
void CSummonGroupSelector::SetSuccessPhaseNum(int nNum)
{
	m_nSuccessPhaseNum = nNum;
}

//============================================================================
// �蓁�g�p�񐔂��擾
//============================================================================
const int& CSummonGroupSelector::GetNumUseHandSword() const
{
	return m_nNumUseHandSword;
}

//============================================================================
// �蓁�g�p�񐔂�ݒ�
//============================================================================
void CSummonGroupSelector::SetNumUseHandSword(int nNum)
{
	m_nNumUseHandSword = nNum;
}

//============================================================================
// ��Փx�X�R�A���擾
//============================================================================
const int& CSummonGroupSelector::GetDifficultyScore() const
{
	return m_nDifficultyScore;
}

//============================================================================
// ��Փx�X�R�A��ݒ�
//============================================================================
void CSummonGroupSelector::SetDifficultyScore(int nScore)
{
	m_nDifficultyScore = nScore;
}

//============================================================================
// �����O���[�v�ԍ����擾����
//============================================================================
const int& CSummonGroupSelector::GetSummonGroupNum() const
{
	return m_nSummonGroupNum;
}

//============================================================================
// �����O���[�v�ԍ���ݒ肷��
//============================================================================
void CSummonGroupSelector::SetSummonGroupNum(int nNum)
{
	m_nSummonGroupNum = nNum;
}

//============================================================================
// ��Փx�����N���X�V����
//============================================================================
void CSummonGroupSelector::UpdateDifficultyRank()
{
	int nDifficultyRank = static_cast<int>(m_DifficultyRank); // ��Փx�����N

	// ��Փx�X�R�A���{�[�_�[�𒴂����̂œ�Փx�����N���グ��BEXPERT�͍ő僉���N�Ȃ̂ŏグ�܂���
	if (m_DifficultyRank != CSummon_Manager::DIFFICULTY::EXPERT)
	{
		if (m_nDifficultyScore >= s_nDifficultyBorderScore[nDifficultyRank + 1])
		{
			m_DifficultyRank = static_cast<CSummon_Manager::DIFFICULTY>(nDifficultyRank + 1);
		}
	}

	// ��Փx�X�R�A���O�̃����N�̃{�[�_�[����������̂œ�Փx�����N��������BEASY�͍Œ჉���N�Ȃ̂ŉ����܂���B
	if (m_DifficultyRank != CSummon_Manager::DIFFICULTY::EASY)
	{
		if (m_nDifficultyScore < s_nDifficultyBorderScore[nDifficultyRank])
		{
			m_DifficultyRank = static_cast<CSummon_Manager::DIFFICULTY>(nDifficultyRank - 1);
		}
	}
}

//============================================================================
// �����O���[�v�����肷��
//============================================================================
void CSummonGroupSelector::DecideSummonGroup()
{
	// �����}�l�[�W���[�ւ̃|�C���^�擾
	CSummon_Manager* pSummonManager = dynamic_cast<CSummon_Manager*>(CObject::FindSpecificObject(CObject::TYPE::SUMMONMANAGER));

	// �����}�l�[�W���[�����݂��Ă���Ȃ�
	if (pSummonManager)
	{
		CSummon_Manager_State* pSummonManagerState = pSummonManager->GetState(); // �����}�l�[�W���[��Ԃւ̃|�C���^
		const JSON& SummonPatternList = pSummonManagerState->GetJsonSummonPatternList();    // �����p�^�[�����X�g
		vector<int> PossibleSummonGroupNum; // �����o����O���[�v�ԍ����I�z��
		CSummon_Manager::DIFFICULTY Difficulty = CSummon_Manager::DIFFICULTY::MAX; // ��Փx�i�ǂ�ɂ����Ă͂܂�Ȃ�MAX�ɏ�����)
		int nCnt = 0; // �z��J�E���g�p

		// �͈̓x�[�Xfor���őS�Ă̏����p�^�[��������
		for (auto& Obj : SummonPatternList)
		{
			// ���x�����̃L�[�����݂��Ȃ��Ȃ珈���𒆒f
			if (!Obj.contains("LevelName"))
			{
				assert(false && "�����p�^�[�����X�g�Ƀ��x�����̃L�[������܂���");
			}

			// �ǂ�ɂ����Ă͂܂�Ȃ�MAX�ɏ������i��Փx�Ƃ͕ʂ�"Test"�Ƃ������x��������̂ł����ǂݍ��܂Ȃ�����)
			Difficulty = CSummon_Manager::DIFFICULTY::MAX;

			// ��Փx����
			if (Obj["LevelName"] == "Easy")
			{
				Difficulty = CSummon_Manager::DIFFICULTY::EASY;
			}
			else if (Obj["LevelName"] == "Normal")
			{
				Difficulty = CSummon_Manager::DIFFICULTY::NORMAL;
			}
			else if (Obj["LevelName"] == "Hard")
			{
				Difficulty = CSummon_Manager::DIFFICULTY::HARD;
			}
			else if (Obj["LevelName"] == "Expert")
			{
				Difficulty = CSummon_Manager::DIFFICULTY::EXPERT;
			}

			if (Difficulty == m_DifficultyRank)
			{
				PossibleSummonGroupNum.push_back(nCnt); // ���݂̏����O���[�v�ԍ����i�[
			}
			
			nCnt++;
		}

		// ���݂̓�Փx�ɊY�����鏢���p�^�[�����Ȃ��ꍇ�A�����O���[�v�ԍ������肵�Ȃ�
		if (PossibleSummonGroupNum.size() > 0)
		{
			int nPossibleSummonRand = rand() % PossibleSummonGroupNum.size();

			// ���݂̓�Փx�����N�̃����_���ȃO���[�v�ԍ���ݒ肷��
			m_nSummonGroupNum = PossibleSummonGroupNum[nPossibleSummonRand];

			// �f�o�b�O�\��
			CRenderer::SetDebugString("��Փx�̏����O���[�v���F" + to_string(PossibleSummonGroupNum.size()));
		}
	}
}

//============================================================================
// ��Փx�����N�擾
//============================================================================
const CSummon_Manager::DIFFICULTY& CSummonGroupSelector::GetDifficultyRank() const
{
	return m_DifficultyRank;
}

//============================================================================
// �����O���[�v�����肷��
//============================================================================
const int& CSummonGroupSelector::GetMaxDifficultyScore()
{
	return s_nMaxDifficultyScore;
}
