//============================================================================
// 
// �Q�O�Q�T�N�T���P�P���F�����}�l�[�W���[����� [summon_manager.cpp]
// Author : ���c����
// 
//============================================================================

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "summon_manager.h" // ���w�b�_
#include "renderer.h" // �f�o�b�O�p
#include "summon_manager_state_start.h" // �����J�n���
#include "summon_manager_state.h" // �������

//****************************************************
// using�f�B���N�e�B�u
//****************************************************
using namespace useful;

//****************************************************
// �ÓI�����o�錾
//****************************************************
int CSummon_Manager::s_nNumObj = 0; // ���������I�u�W�F�N�g��

//============================================================================
//
// public�����o
//
//============================================================================

//====================================================
// �R���X�g���N�^
//====================================================
CSummon_Manager::CSummon_Manager() :
	m_pSummonManagerState{ DBG_NEW CSummon_Manager_State_Start() },
	CObject(CObject::LAYER::DEFAULT)
{
	// �^�C�v�ݒ�
	CObject::SetType(CObject::TYPE::SUMMONMANAGER);
}

//====================================================
// �f�X�g���N�^
//====================================================
CSummon_Manager::~CSummon_Manager()
{
	if (m_pSummonManagerState != nullptr)
	{// �����}�l�[�W���[�X�e�[�g���I��
		delete m_pSummonManagerState;    // ���I�����������
		m_pSummonManagerState = nullptr; // �|�C���^��������
	}
}

//====================================================
// ����������
//====================================================
HRESULT CSummon_Manager::Init()
{
	// �����Ȃ�
	return S_OK;
}

//====================================================
// �I������
//====================================================
void CSummon_Manager::Uninit()
{
	// �����Ȃ�
}

//====================================================
// �X�V����
//====================================================
void CSummon_Manager::Update()
{
	// �����}�l�[�W���[�X�e�[�g�ւ̃|�C���^�����݂��Ă�����
	if (m_pSummonManagerState != nullptr)
	{
		m_pSummonManagerState->Update(this);
	}
}

//====================================================
// �`�揈��
//====================================================
void CSummon_Manager::Draw()
{
	// �����Ȃ�
}

//====================================================
// �X�e�[�g�ύX����
//====================================================
void CSummon_Manager::SetChengeState(CSummon_Manager_State* pState)
{
	if (m_pSummonManagerState != nullptr)
	{// �����}�l�[�W���[�X�e�[�g���I��
		delete m_pSummonManagerState;    // ���I�����������
		m_pSummonManagerState = pState;  // �V�����X�e�[�g��ݒ�
	}
}

//====================================================
// �X�e�[�g�擾
//====================================================
CSummon_Manager_State* CSummon_Manager::GetState() const
{
	return m_pSummonManagerState;
}

//====================================================
// �����}�l�[�W���[��������
//====================================================
CSummon_Manager* CSummon_Manager::Create()
{
	CSummon_Manager* pSummonManager = DBG_NEW CSummon_Manager(); // �C���X�^���X����
	pSummonManager->Init(); // ����������

	return pSummonManager;
}

//====================================================
// ���������I�u�W�F�N�g���𑝂₷
//====================================================
void CSummon_Manager::AddNumObj()
{
	s_nNumObj++;
}

//====================================================
// ���������I�u�W�F�N�g�������炷
//====================================================
void CSummon_Manager::SubNumObj()
{
	s_nNumObj--;
}

//====================================================
// �����}�l�[�W���[�����I�u�W�F�N�g���擾
//====================================================
int CSummon_Manager::GetNumObj()
{
	return s_nNumObj;
}

//====================================================
// �����I�u�W�F�N�g�̓������~�߂�
//====================================================
void CSummon_Manager::StopSummonObj()
{
	// �f�t�H���g���C���[�̃��X�g�̐擪�|�C���^���擾
	CObject* pObj = CObject::GetTopObject(CObject::LAYER::DEFAULT);

	// �����I�u�W�F�N�g�̍X�V���~�߂�
	while (pObj)
	{
		// ���X�g�̎��̃I�u�W�F�N�g�ւ̃|�C���^���擾
		CObject* pNext = pObj->GetNext();

		if (pObj->GetType() == CObject::TYPE::OBSTACLE ||
			pObj->GetType() == CObject::TYPE::HOLEOBSTACLE ||
			pObj->GetType() == CObject::TYPE::ZONE)
		{
			pObj->SetPause(true);
		}

		// ���X�g�����ɐi�߂�
		pObj = pNext;
	}
}
