//============================================================================
// 
// �Q�O�Q�T�N�T���P�T���F�����O���[�v�� [summon_mmanager_state_start.cpp]
// Author : �Ŗ����z
// 
//============================================================================

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "summon_manager_state_start.h" // ���w�b�_
#include "pch.h"                        // �l�X�ȃC���N���[�h�����g��
#include "obstacle.h"                   // ��Q��
#include "input_keyboard.h"             // �L�[����
#include "PlayerState_Thrusting.h"      // �v���C���[��ԁQ�˂�
#include "PlayerState_Returning.h"      // �v���C���[��ԁQ�߂�
#include "PlayerState_BeforeThrust.h"   // �v���C���[��ԁQ�˂�������O
#include "SummonGroupSelector.h"        // �����O���[�v�I��
#include "zone.h"                       // �]�[��
#include "game.h"                       // �Q�[��
#include "GamePlayer.h"                 // �Q�[���v���C���[
#include "manager.h"                    // �}�l�[�W���[
#include "VisionBlockSpawner.h"         // ����W�Q�I�u�W�F�N�g�X�|�i�[
#include "VisionBlocker.h"              // ����W�Q�I�u�W�F�N�g
#include "particle.h"                   // �p�[�e�B�N��
#include "renderer.h"                   // �����_���[�i�f�o�b�O�\���Ȃǁj
#include "SummonCommonData.h"           // �����O���[�v���ʃf�[�^
#include "StencilMask.h"                // �X�e���V���}�X�N
#include "HoleObstacle.h"               // ��������Q��

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

//====================================================
// �f�t�H���g�R���X�g���N�^
//====================================================
CSummon_Manager_State_Start::CSummon_Manager_State_Start() :
	m_nSummonCoolTimeCnt{ s_nMaxSummonCoolTime },
	m_bSummonCoolTime{ true },
	m_bStartPhase{ false },
	m_nSummonCoolTimeFrame{ 0 },
	m_bSuccessPhase{ false },
	m_nSummonNum{ 0 },
	m_nPhaseComboCnt{ 0 },
	m_nNumPhase{0},
	m_nMaxPhase{ s_nNormalMaxPhase },
	m_bAllPhaseComplete{ false },
	m_bPhaseJudgmentFrame{ false },
	m_pSummonGroupSelector{ DBG_NEW CSummonGroupSelector() }
{
	// �����Ȃ�
}

//====================================================
// �ő�t�F�[�Y����p�R���X�g���N�^
//====================================================
CSummon_Manager_State_Start::CSummon_Manager_State_Start(int nMaxPhase) :
	m_nSummonCoolTimeCnt{ s_nMaxSummonCoolTime},
	m_bSummonCoolTime{ true},
	m_bStartPhase{false},
	m_nSummonCoolTimeFrame{0},
	m_bSuccessPhase{false},
	m_nSummonNum{0},
    m_nPhaseComboCnt{0},
	m_nNumPhase{ 0 },
	m_nMaxPhase{ nMaxPhase },
	m_bAllPhaseComplete{false},
	m_bPhaseJudgmentFrame{false},
	m_pSummonGroupSelector{ DBG_NEW CSummonGroupSelector() }
{
	// �����Ȃ�
}

//====================================================
// �f�X�g���N�^
//====================================================
CSummon_Manager_State_Start::~CSummon_Manager_State_Start()
{
	// �����O���[�v�I���I�u�W�F�N�g�I��
	if (m_pSummonGroupSelector)
	{
		delete m_pSummonGroupSelector;
		m_pSummonGroupSelector = nullptr;
	}
}

//====================================================
// �X�V����
//====================================================
void CSummon_Manager_State_Start::Update(CSummon_Manager* pManager)
{
	// === �������J�n���� ===

	// �S�Ẵt�F�[�Y���I�����Ă����珈�������Ȃ�
	if (m_bAllPhaseComplete)
	{
		return;
	}

	if (m_pSummonGroupSelector)
	{
		m_pSummonGroupSelector->Update();
	}
	
	// �����_�������i�C���Q�[���j
	RandumSummon();

	// ���N���X�X�V
	CSummon_Manager_State::Update(pManager);

	// �f�o�b�O�\��
	CRenderer::SetDebugString("�X�e�[�g�ύX�iV)�F�����J�n���[�h");
	CRenderer::SetDebugString("�����N�[���^�C���F" + to_string(m_nSummonCoolTimeCnt));
	CRenderer::SetDebugString("�����}�l�[�W���[�I�u�W�F�N�g���F" + to_string(CSummon_Manager::GetNumObj()));
	CRenderer::SetDebugString("���݂̃t�F�[�Y�F" + to_string(m_nNumPhase + 1) + "/" + to_string(m_nMaxPhase));
	CRenderer::SetDebugString("�t�F�[�Y�A�������񐔁F" + to_string(m_nPhaseComboCnt));
}

//====================================================
// �t�F�[�Y�ɏ����������ǂ����擾
//====================================================
const bool& CSummon_Manager_State_Start::GetSuccessPhase() const
{
	return m_bSuccessPhase;
}

//====================================================
// �t�F�[�Y�ɏ����������ǂ����ݒ�
//====================================================
void CSummon_Manager_State_Start::SetSuccessPhase(bool bSuccess)
{
	m_bSuccessPhase = bSuccess;
}

//====================================================
// �t�F�[�Y�۔���t���[�����ǂ���
//====================================================
const bool& CSummon_Manager_State_Start::GetPhaseJudgmentFrame() const
{
	return m_bPhaseJudgmentFrame;
}

//====================================================
// �t�F�[�Y���J�n�������ǂ���
//====================================================
const bool& CSummon_Manager_State_Start::GetStartPhase() const
{
	return m_bStartPhase;
}

//====================================================
// �I�����ǂ���
//====================================================
const bool& CSummon_Manager_State_Start::GetAllPhaseComplete() const
{
	return m_bAllPhaseComplete;
}

//====================================================
// �����N�[���^�C�����ǂ����擾
//====================================================
const bool& CSummon_Manager_State_Start::GetSummonCoolTime() const
{
	return m_bSummonCoolTime;
}

//====================================================
// �����O���[�v�I���I�u�W�F�N�g���擾����
//====================================================
CSummonGroupSelector* CSummon_Manager_State_Start::GetSummonGroupSelector() const
{
	return m_pSummonGroupSelector;
}

//====================================================
// �����N�[���^�C���ő�t���[�����擾
//====================================================
const int& CSummon_Manager_State_Start::GetMaxSummonCoolTime()
{
	return s_nMaxSummonCoolTime;
}

//====================================================
// �����_����������
//====================================================
bool CSummon_Manager_State_Start::RandumSummon()
{
	// === �����ɕK�v�ȏ���錾�A������ ===

	bool bSummon = false; // �����������ǂ���
	int nNumSummonObj = CSummon_Manager::GetNumObj(); // �����I�u�W�F�N�g��
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(CObject::FindSpecificObject(CObject::TYPE::PLAYER));   // �v���C���[�ւ̃|�C���^�擾
	CPlayerState_Returning * pReturning = dynamic_cast<CPlayerState_Returning*>(pPlayer->GetState()); // �߂��Ԃւ̃|�C���^�擾
	m_bPhaseJudgmentFrame = false; // ���ɉ۔�����\�ɂ������킯�ł͂Ȃ��̂�

	// === �����_���������J�n ===

	// �����N�[���^�C������Ȃ���Ώ������J�n����
	if (!m_bSummonCoolTime)
	{
		// �����I�u�W�F�N�g���Ȃ��Ȃ�A
		if (nNumSummonObj <= 0)
		{
			// �v���C���[���߂��ԈȊO�Ȃ珈�����I��
			if (pReturning == nullptr)
			{
				return false;
			}

			m_nNumPhase++;

			// �S�Ẵt�F�[�Y���I�����Ă����炻�̃t���O���I���ɂ���
			if (m_nNumPhase == m_nMaxPhase)
			{
				m_bAllPhaseComplete = true;
			}

			m_bSummonCoolTime = true; // �����N�[���^�C���ɂ���
			m_nSummonCoolTimeCnt = s_nMaxSummonCoolTime; // �����N�[���^�C���t���[������ݒ�

			// �P��ł���������Ă��Ȃ��ꍇ�A�۔�����s��Ȃ�
			if (m_nSummonNum > 0)
			{
				m_bPhaseJudgmentFrame = true; // ����̃t�F�[�Y�̉۔�����\�ɂ���
			}
			// ����̃t�F�[�Y�̉۔��������̂ŁA���������Ȃ��Փx�X�R�A�𑝂₷
			if (m_bSuccessPhase)
			{
				int nDifficultyScore = m_pSummonGroupSelector->GetDifficultyScore();
				m_nPhaseComboCnt++; // �t�F�[�Y�A�������񐔂��C���N�������g
				m_pSummonGroupSelector->SetDifficultyScore(nDifficultyScore + s_nAddDifficultyScore);
			}
			// ����̃t�F�[�Y�����s�����Ȃ�t�F�[�Y�A�������񐔂����Z�b�g
			else
			{
				m_nPhaseComboCnt = 0;
			}
		}
	}
	// �����N�[���^�C���Ȃ�
	else
	{
		if (m_pSummonGroupSelector)
		{
			bSummon = CoolTimeAfterChooseSummon(); // �N�[���^�C���I����ɑI���O���[�v������
		}
	}

	return bSummon;
}

//====================================================
// �����N�[���^�C����ɏ�������
//====================================================
bool CSummon_Manager_State_Start::CoolTimeAfterChooseSummon()
{
	bool bSummon = false; // �����ɐ����������ǂ���
	m_bStartPhase = false; // �t�F�[�Y�J�n�t���O���Z�b�g

	// �����N�[���^�C���Ȃ�
	if (m_bSummonCoolTime)
	{
		m_nSummonCoolTimeFrame++; // �����N�[���^�C���t���[�������J�E���g����

		// �����N�[���^�C�����O�ȏ�Ȃ猸�炵������
		if (m_nSummonCoolTimeCnt > 0)
		{
			m_nSummonCoolTimeCnt--;
		}
		// �����N�[���^�C�����O�Ȃ�N�[���^�C��������������
		else
		{
			// �����O���[�v�I�����Ȃ����
			if (m_pSummonGroupSelector)
			{
				//pGamePlayer->SetControl(true);
				m_pSummonGroupSelector->DecideSummonGroup(); // �����O���[�v����
				int nSummonGroup = m_pSummonGroupSelector->GetSummonGroupNum(); // ���݂̏����O���[�v���擾
				bSummon = SetSummonGroup(nSummonGroup); // �����J�n
			}

			return bSummon;
		}
	}
	
	return false;
}

//====================================================
// ����W�Q�C�x���g���s��
//====================================================
void CSummon_Manager_State_Start::VisionBlockerEvent()
{
	fstream ReadingFile; // �t�@�C���ǂݍ��ݗp
	ReadingFile.open("Data\\JSON\\VisionBlockerPattern_List.json", ios::in);
	CSummon_Manager::DIFFICULTY DifficultyRank = m_pSummonGroupSelector->GetDifficultyRank(); // ��Փx�����N�������O���[�v�I������擾

	// �t�@�C��������ɓǂݍ��߂Ă��Ȃ��������O����
	if (!ReadingFile.good())
	{
		assert(false && "����W�Q�p�^�[�����X�g������ɓǂݍ��߂Ă��܂���");
	}

	JSON PatternList; // ����W�Q�p�^�[�����X�g
	ReadingFile >> PatternList; // ����W�Q�p�^�[����ǂݍ���

	ReadingFile.close(); // �t�@�C�������

	// �z��^�œǂݍ��ނ̂ł������Ȃ��O����
	if (!PatternList.is_array())
	{
		assert(false && "����W�Q�p�^�[�����X�g���z��^���Ⴀ��܂���");
	}

	string PatternFilePass = PatternList[static_cast<int>(DifficultyRank)]; // ����̃p�^�[��

	ReadingFile.open(PatternFilePass, ios::in); // ����W�Q�p�^�[�����X�g�̎w�肵���p�X�̃t�@�C����ǂݍ���

	if (!ReadingFile.good())
	{
		assert(false && "����W�Q�p�^�[��������ɓǂݍ��߂Ă��܂���");
	}

	JSON JsonPattern;
	ReadingFile >> JsonPattern; // ����W�Q�p�^�[����ǂݍ���

	ReadingFile.close(); // �t�@�C�������

	// �ǂݍ��񂾃p�^�[���̑S�Ă̗v�f������
	for (auto& Obj : JsonPattern)
	{
		vector<float> vSpawnPos = Obj["SpawnPoint"]; // �����ʒu
		vector<float> vBaseMove = Obj["BaseMove"];   // �x�[�X�ړ���
		int nSpawnRate = Obj["SpawnRate"]; // �������[�g
		vector<float> vMaxSpawnRange = Obj["SpawnRange"]["Max"]; // �ő叢���͈�
		vector<float> vMinSpawnRange = Obj["SpawnRange"]["Min"]; // �ŏ������͈�
		float fMaxLeafSpeed = Obj["LeafSpeed"]["Max"]; // �ő�t���ϑ��x
		float fMinLeafSpeed = Obj["LeafSpeed"]["Min"]; // �Œ�t���ϑ��x
		float fMaxLeafSwayAddAngle = Obj["LeafSwayAddAngle"]["Max"]; // �ő�t���ς��h���������Z��
		float fMinLeafSwayAddAngle = Obj["LeafSwayAddAngle"]["Min"]; // �Œ�t���ς��h���������Z��
		float fLeafAddRotRange = Obj["LeafAddRotRange"]; // �t���ς���]������������Z�ʔ͈�
		int nLife = Obj["Life"]; // �̗�
		string LeafName = Obj["LeafType"]; // �t���ς̎�ޖ��O
		CVisionBlocker::TYPE LeafType = CVisionBlocker::TYPE::LEAF00; // �t���ώ��

		// �t���ς̖��O�ɂ���Ď�ނ����肷��
		if (LeafName == "Green")
		{
			LeafType = CVisionBlocker::TYPE::LEAF00;
		}
		else if (LeafName == "Yellow")
		{
			LeafType = CVisionBlocker::TYPE::LEAF01;
		}
		else if (LeafName == "Red")
		{
			LeafType = CVisionBlocker::TYPE::LEAF02;
		}

		// ����W�Q�I�u�W�F�N�g�X�|�i�[�𐶐�
		CVisionBlockSpawner::Create(
			D3DXVECTOR3(vSpawnPos[0], vSpawnPos[1], vSpawnPos[2]),
			D3DXVECTOR3(vBaseMove[0], vBaseMove[1], vBaseMove[2]),
			nSpawnRate,
			D3DXVECTOR3(vMaxSpawnRange[0], vMaxSpawnRange[1], vMaxSpawnRange[2]),
			D3DXVECTOR3(vMinSpawnRange[0], vMinSpawnRange[1], vMinSpawnRange[2]),
			fMaxLeafSpeed,
			fMinLeafSpeed,
			fMaxLeafSwayAddAngle,
			fMinLeafSwayAddAngle,
			fLeafAddRotRange,
			nLife,
			LeafType);
	}
}

//====================================================
// �O���[�v��������
//====================================================
bool CSummon_Manager_State_Start::SetSummonGroup(int nSummonGroup)
{
	m_bSuccessPhase = true; // �V���������O���[�v = �t�F�[�Y���n�܂�̂Ő��������������
	m_nSummonNum++; // �����񐔂��C���N�������g
	m_nSummonCoolTimeFrame = 0; // �����N�[���^�C���̃t���[������������
	m_bSummonCoolTime = false; // �����N�[���^�C�����I�t�ɂ���
	m_bStartPhase = true; // �t�F�[�Y�J�n
	return CSummon_Manager_State::SetSummonGroup(nSummonGroup);
}
