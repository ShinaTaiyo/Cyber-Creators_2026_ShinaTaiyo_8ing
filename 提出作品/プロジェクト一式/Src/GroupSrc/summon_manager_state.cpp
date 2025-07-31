//============================================================================
// 
// �Q�O�Q�T�N�T���P�P���F�����}�l�[�W���[����� [Summon_Manager_State_state.cpp]
// Author : �Ŗ����z
// 
//============================================================================

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "Summon_Manager_state.h" // ���w�b�_
#include "game.h"                 // �Q�[���V�[��
#include "pch.h"                  // �l�X�ȃC���N���[�h�����g��
#include "renderer.h"             // �f�o�b�O�Ȃ�
#include "input_keyboard.h"       // �L�[���͏��
#include "manager.h"              // �}�l�[�W���[
#include "calculation.h"          // �v�Z
#include "SummonGroupSelector.h"  // �����O���[�v�I��

// === �I�u�W�F�N�g ===
#include "object_X.h"     // �I�u�W�F�N�gX
#include "object_3D.h"    // �I�u�W�F�N�g3D
#include "obstacle.h"     // ��Q��
#include "player.h"       // �v���C���[
#include "HoleObstacle.h" // ���󂫏�Q��
#include "zone.h"         // �]�[��
#include "VisionBlocker.h" // ����W�Q�I�u�W�F�N�g
#include "VisionBlockSpawner.h" // ����W�Q�I�u�W�F�N�g�X�|�i�[
#include "StencilMask.h"  // �X�e���V���}�X�N

// === �V�F�[�_�[ ===
#include "SmokeEffect.h" // ��
#include "ToneAdjust.h"  // �F���ω�

// === ��� ===
#include "PlayerState_BeforeThrust.h" // �v���C���[��ԁF�˂�������O


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
// �R���X�g���N�^
//====================================================
CSummon_Manager_State::CSummon_Manager_State() : 
	m_nGroup{0},
	m_bJustSummon{false}
{
	std::fstream ReadingFile; // �t�@�C���ǂݍ��ݗp

	ReadingFile.open("Data\\JSON\\SummonPattern_List.Json", std::ios::in); // �����p�^�[�����X�gVer2�̃W�F�C�\���t�@�C�����J��

	// �t�@�C��������ɊJ���Ă����珢���p�^�[�����X�g���i�[
	if (ReadingFile.good())
	{
		ReadingFile >> m_JsonSummonPatternList; // �W�F�C�\���i�[

		// �z�񂶂�Ȃ������珈�����I��
		if (!m_JsonSummonPatternList.is_array())
		{
			assert(false && "�����p�^�[�����X�g���z��^���Ⴀ��܂���");
		}
	}

	ReadingFile.close(); // �t�@�C�������
}

//====================================================
// �f�X�g���N�^
//====================================================
CSummon_Manager_State::~CSummon_Manager_State()
{
	// �����Ȃ�
}

//====================================================
// �X�V����
//====================================================
void CSummon_Manager_State::Update(CSummon_Manager* pManager)
{
	m_bJustSummon = false; // ���傤�Ǐ��������t���O���}�C�t���[�����Z�b�g
	DestroyOutOfRange(); // �w��͈͊O�ɏo���I�u�W�F�N�g��j������
}

//====================================================
// �W�F�C�\���������擾
//====================================================
const JSON& CSummon_Manager_State::GetJsonSummonGroup() const
{
	return m_JsonSummonGroup;
}

//====================================================
// �W�F�C�\�������p�^�[�����X�g���擾
//====================================================
const JSON& CSummon_Manager_State::GetJsonSummonPatternList() const
{
	return m_JsonSummonPatternList;
}

//====================================================
// �I��ł���O���[�v�ԍ��擾
//====================================================
const int& CSummon_Manager_State::GetChooseGroup() const
{
	return m_nGroup;
}

//====================================================
// �I��ł���O���[�v�ԍ��ݒ�
//====================================================
void CSummon_Manager_State::SetChooseGroup(int nChoose)
{
	m_nGroup = nChoose;
}

//====================================================
// �O���[�v��������
//====================================================
bool CSummon_Manager_State::SetSummonGroup(int nSummonGroup)
{
	// === �����ݒ� ===
	m_bJustSummon = true; // ���傤�Ǐ�������
	ChangeJsonSummonGroup(nSummonGroup); // �����O���[�v���w��O���[�v�ɕύX
	const JSON& JsonSummonGroup = GetJsonSummonGroup(); // �����O���[�v
	SetAnticipationFrame(JsonSummonGroup); // ���؂�t���[������ݒ肷��
	ReleaseAllVisionBlockSpawner(); // �S�Ă̎���W�Q�I�u�W�F�N�g�X�|�i�[��j������
	ResetEvent(); // �C�x���g�ݒ胊�Z�b�g

	// �I�u�W�F�N�g�ȊO�Ȃ珈�����I�����܂�
	if (!JsonSummonGroup.is_object())
	{
		assert(false && "�����O���[�v���I�u�W�F�N�g�^���Ⴀ��܂���");
	}

	if (!JsonSummonGroup.contains("Objects"))
	{
		assert(false && "���݂̏����p�^�[����Object�̃L�[������܂���");
		return false;
	}

	SetEvent(JsonSummonGroup); // �C�x���g�ݒ菈��

	JSON JsonObj = JsonSummonGroup["Objects"]; // �W�F�C�\���I�u�W�F�N�g(�����I�u�W�F�N�g���)

	// �w��O���[�v�͈̓x�[�Xfor��
	for (auto& Obj : JsonObj)
	{
		string Name;  // �I�u�W�F�N�g��

		// ���O��ǂݍ��ݐ�������I�u�W�F�N�g�𔻕�
		if (Obj.contains("Name"))
		{
			Name = Obj["Name"];
		}

		// ���Ȃ�vector�^�œǂݍ��ށH
		//��Json�͔z���������vector�^�ɕϊ�����@�\�����邩��ł��BD3DXVECTOR3�ɂ͎����ϊ�����@�\���Ȃ��ł��B

		// �����O���[�v�̃I�u�W�F�N�g�P�ʂ̃��f���p�X�Ő����ϊ�
		OldModelPathConverterForSummonPettern(Obj);

		// ���f���p�X���擾
		const std::string& ModelPath = static_cast<std::string>(Obj["ModelName"]);

		// ��Q��
		if (Name == "Obstacle")
		{
			CObstacle* pObstacle = CObstacle::Create(ModelPath, CObstacle::OBSTACLETYPE::TYPE00_BLOCK, VEC3_INIT, true); // ��Q������
			pObstacle->InitFromJson(Obj); // �W�F�C�\������������
		}
		// �Z�[�t�]�[��
		else if (Name == "SafeZone")
		{
			CZone* pZone = CZone::Create(CZone::TYPE::SAFEZONE, VEC3_INIT, D3DXVECTOR3(10.0f, 10.0f, 0.0f), true);
			pZone->InitFromJson(Obj); // �W�F�C�\������������
		}
		// ��������Q��
		else if (Name == "HoleObstacle")
		{
			CHoleObstacle* pHoleObstacle = CHoleObstacle::Create(ModelPath, CObstacle::OBSTACLETYPE::TYPE00_BLOCK, VEC3_INIT, true);
			pHoleObstacle->InitFromJson(Obj); // �W�F�C�\������������
		}
	};

	return true;
}

//====================================================
// �X�e�[�g�ύX����
//====================================================
void CSummon_Manager_State::ChengeState(CSummon_Manager_State* pState, CSummon_Manager* pManager)
{
	pManager->SetChengeState(pState);
}

//====================================================
// �����O���[�v��ύX����
//====================================================
void CSummon_Manager_State::ChangeJsonSummonGroup(int nChooseGroup)
{
	m_JsonSummonGroup.clear(); // �����O���[�v�W�F�C�\����������
	std::fstream ReadingFile;  // �t�@�C���ǂݍ��ݗp

	m_nGroup = nChooseGroup; // �I���O���[�v�ԍ��ύX

	// �����p�^�[�����X�g���珢���p�^�[���̃W�F�C�\���t�@�C���̃p�X���w�肵����
	int nSize = m_JsonSummonPatternList.size(); // ���f���p�X�̐�

	// �z��O�A�N�Z�X���Ă��珈���𒆒f
	if (!Calculation::Value::IsAccessSafeForArray(m_nGroup, nSize))
	{
		assert(false && "�����O���[�v�ύX�����Ŕz��O�A�N�Z�X�ł��I");
		return;
	}

	// �����p�^�[�����X�g��FilePass�̃L�[���Ȃ��̂ŏ����𒆒i
	if (!m_JsonSummonPatternList[m_nGroup].contains("FilePass"))
	{
		assert(false && "�����p�^�[�����X�g�Ƀt�@�C���p�X�̃L�[������܂���");
	}

	const std::string FileName = m_JsonSummonPatternList[m_nGroup]["FilePass"]; // �w��O���[�v�̃p�X�̖��O

	ReadingFile.open(FileName, std::ios::in); // �w��O���[�v�̃W�F�C�\���t�@�C�����J��

	// �t�@�C�����J���ĂȂ���Ώ������I������
	if (!ReadingFile.good())
	{
		assert(false && "�t�@�C��������ɊJ���Ă��܂���");
		return;
	}

	ReadingFile >> m_JsonSummonGroup; // �W�F�C�\�������O���[�v�i�[

	// ���x�����̃L�[���Ȃ���Ώ������I��
	if (!m_JsonSummonPatternList[m_nGroup].contains("LevelName"))
	{
		assert(false && "���x�����̃L�[������܂���");
	}

	string DifficultyName = m_JsonSummonPatternList[m_nGroup]["LevelName"]; // ���x����

	ReadingFile.close(); // �t�@�C�������
}

//====================================================
// �C�x���g�ݒ�
//====================================================
void CSummon_Manager_State::SetEvent(const JSON& SummonPattern)
{
	// �u�^�O�v�̃L�[�����݂���Ȃ�C�x���g�𔭐�������
	if (SummonPattern.contains("Tag"))
	{
		string EventTag = SummonPattern["Tag"];

		// ����W�Q�I�u�W�F�N�g�i�t���ρj����������
		if (EventTag == "Leaf")
		{
			VisionBlockerEvent();
		}
		else if (EventTag == "Smork")
		{
			SmorkEvent();
		}
	}
}

//====================================================
// ����W�Q�C�x���g���s��
//====================================================
void CSummon_Manager_State::VisionBlockerEvent()
{
	fstream ReadingFile; // �t�@�C���ǂݍ��ݗp
	ReadingFile.open("Data\\JSON\\VisionBlockerPattern_List.json", ios::in);
	CSummon_Manager::DIFFICULTY DifficultyRank = CSummon_Manager::DIFFICULTY::NORMAL; // ��Փx�����N�i�Œ�l��NORMAL�j

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
// �S�Ă̎���W�Q�I�u�W�F�N�g�X�|�i�[������
//====================================================
void CSummon_Manager_State::ReleaseAllVisionBlockSpawner()
{
	// �f�t�H���g���C���[�̃��X�g�̐擪�I�u�W�F�N�g�ւ̃|�C���^���擾
	CObject* pObj = CObject::GetTopObject(CObject::LAYER::DEFAULT);

	// ���X�g���王��W�Q�I�u�W�F�N�g�X�|�i�[��T���A�j������
	while (pObj)
	{
		CObject* pNext = pObj->GetNext(); // ���X�g�̎��̃I�u�W�F�N�g�ւ̃|�C���^���擾
		
		// ����W�Q�I�u�W�F�N�g�X�|�i�[�ȊO�Ȃ珈���𒆒f�����X�g�̃|�C���^�����ɐi�߂�
		if (pObj->GetType() != CObject::TYPE::VISIONBLOCKSPAWNER)
		{
			pObj = pNext;
			continue;
		}

		// �j������
		pObj->SetRelease();

		pObj = pNext; // ���X�g�̃|�C���^�����ɐi�߂�
	}
}

//====================================================
// ���C�x���g���s��
//====================================================
void CSummon_Manager_State::SmorkEvent()
{
	CStencilMask::SetUseStencil(true); // �X�e���V���}�X�N���g�p���A�V���G�b�g��\��
	CSmokeEffect::SetUseDraw(true); // ����`�悷��
	//CToneAdjust::SetUseDraw(true);  // �F���ω���`�悷��
}

//====================================================
// �C�x���g�ݒ胊�Z�b�g
//====================================================
void CSummon_Manager_State::ResetEvent()
{
	CStencilMask::SetUseStencil(false);  // �X�e���V���}�X�N�̃C�x���g�����Z�b�g
	CSmokeEffect::SetUseDraw(false); // ����`�悵�Ȃ�
	//CToneAdjust::SetUseDraw(false);  // �F���ω���`�悵�Ȃ�
}

//====================================================
// ���͈͊O�ɏo�������I�u�W�F�N�g��j������
//====================================================
void CSummon_Manager_State::DestroyOutOfRange()
{
	CObject* pObj = CObject::GetTopObject(CObject::LAYER::DEFAULT); // �f�t�H���g���C���[�̐擪���X�g���擾

	// �j������I�u�W�F�N�g�����X�g���猟��
	while (pObj != nullptr)
	{
		CObject* pNext = pObj->GetNext(); // ���̃I�u�W�F�N�g���擾
		const CObject::TYPE Type = pObj->GetType(); // �^�C�v

		// �^�C�v����Q���ƃ]�[���ƌ�������Q���ȊO�Ȃ珈�����X�L�b�v
		if (Type == CObject::TYPE::OBSTACLE ||
			Type == CObject::TYPE::ZONE ||
			Type == CObject::TYPE::HOLEOBSTACLE)
		{
			D3DXVECTOR3 OtherPos = VEC3_INIT; // ����̈ʒu

			// �^�C�v���u�]�[���v�Ȃ�I�u�W�F�N�g3D�Ƀ_�E���L���X�g���Ĉʒu���擾
			if (Type == CObject::TYPE::ZONE)
			{
				CObject_3D* pObj3D = dynamic_cast<CObject_3D*>(pObj);
				OtherPos = pObj3D->GetPos();
			}
			// �^�C�v���u��Q���v�܂��́u��������Q���v�Ȃ�I�u�W�F�N�gX�Ƀ_�E���L���X�g���Ĉʒu���擾
			else if (Type == CObject::TYPE::OBSTACLE ||
				Type == CObject::TYPE::HOLEOBSTACLE)
			{
				CObject_X* pObjX = dynamic_cast<CObject_X*>(pObj);
				OtherPos = pObjX->GetPos();
			}

			// ��둤��艜�ɂ�����j������
			if (OtherPos.z < s_fNormal_ReleaseRangeZ)
			{
				pObj->SetRelease();
			}
		}

		pObj = pNext; // ���X�g�����ɐi�߂�
	}

}

//====================================================
// ���؂�t���[������ݒ肷��
//====================================================
void CSummon_Manager_State::SetAnticipationFrame(const JSON& SummonPattern)
{
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(CObject::FindSpecificObject(CObject::TYPE::PLAYER)); // �v���C���[�ւ̃|�C���^

    // �u���؂�t���[�����v�̃L�[�����݂���Ȃ猩�؂�t���[������ݒ肷��
	if (SummonPattern.contains("AnticipationFrame"))
	{
		int nAnticipationFrame = SummonPattern["AnticipationFrame"];
		pPlayer->SetState(DBG_NEW CPlayerState_BeforeThrust(pPlayer, nAnticipationFrame));
	}
	// �u���؂�t���[�����v�̃L�[�����݂��Ȃ��̂ŌŒ�l��ݒ�
	else
	{
		pPlayer->SetState(DBG_NEW CPlayerState_BeforeThrust(pPlayer, 120));
	}
}
