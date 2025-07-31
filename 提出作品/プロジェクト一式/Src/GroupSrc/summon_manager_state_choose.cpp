//============================================================================
// 
// �Q�O�Q�T�N�T���P�T���F�����O���[�v�� [summon_mmanager_state_Choose.cpp]
// Author : �Ŗ����z
// 
//============================================================================

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "summon_manager_state_Choose.h" // ���w�b�_
#include "summon_manager.h"
#include "calculation.h"
#include "player.h"
#include "PlayerState_GuideBeforeThrust.h" // �v���C���[��ԁF�u�˂��O�K�C�h�v
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
CSummon_Manager_State_Choose::CSummon_Manager_State_Choose(bool bGuide) :
	m_nChooseGroup{0},
	m_bGuide{ bGuide },
	CSummon_Manager_State()
{
	// ���̃E�C���h�E�̈ʒu���T�C�Y�����߂�
	ImGui::SetNextWindowSize(ImVec2(350, 150));
	ImGui::SetNextWindowPos(ImVec2(0, 150));
}

//====================================================
// �f�X�g���N�^
//====================================================
CSummon_Manager_State_Choose::~CSummon_Manager_State_Choose()
{
	// �����Ȃ�
}

//====================================================
// �X�V����
//====================================================
void CSummon_Manager_State_Choose::Update(CSummon_Manager* pManager)
{
#ifdef _DEBUG
	// �K�C�h�Ȃ��̎��͏����O���[�v��I�ׂ�悤�ɂ���
	if (!m_bGuide)
	{
		bool bSummon = false; // �������邩�ǂ���
		bool bChoose = false; // �I���������ǂ���
		ImGui::Begin(u8"�����O���[�v�ԍ���I������");

		// �{�^���ŏ����������O���[�v��I�т܂�
		if (ImGui::Button("-"))
		{
			m_nChooseGroup--;
			bChoose = true;
		}
		// �{�^���������т�
		ImGui::SameLine();
		if (ImGui::Button("+"))
		{
			m_nChooseGroup++;
			bChoose = true;
		}

		// �����O���[�v�I�����ɔz��͈̔͂𒴂�����l�𒲐�����
		if (bChoose)
		{
			m_nChooseGroup = Calculation::Value::LimitRange(m_nChooseGroup, 0, static_cast<int>(CSummon_Manager::SUMMONGROUP::MAX) - 1);
		}

		// �I���O���[�v��\������
		ImGui::Text(u8"�I���O���[�v�ԍ��F%d", m_nChooseGroup);

		// �`�F�b�N�{�b�N�X�Ńt���O�̃I���I�t
		ImGui::Checkbox(u8"�`�F�b�N�{�b�N�X���������u�ԂɑI���O���[�v����������܂�", &bSummon);

		// �����������ǂ����\��
		if (bSummon)
		{
			ImGui::Text(u8"�I���O���[�v�������I");
			SetSummonGroup(m_nChooseGroup);
		}
		else
		{
			ImGui::Text(u8"�I���O���[�v���������Ȃ�");
		}

		ImGui::End();
	}
#endif // _DEBUG

	// ���N���X�X�V
	CSummon_Manager_State::Update(pManager);
}

//====================================================
// �K�C�h���邩�ǂ����擾
//====================================================
const bool& CSummon_Manager_State_Choose::GetGuide() const
{
	return m_bGuide;
}

//====================================================
// �K�C�h���邩�ǂ����ݒ�
//====================================================
void CSummon_Manager_State_Choose::SetGuide(bool bGuide)
{
	m_bGuide = bGuide;
}

//====================================================
// ���؂�t���[������ݒ肷��
//====================================================
void CSummon_Manager_State_Choose::SetAnticipationFrame(const JSON& SummonPattern)
{
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(CObject::FindSpecificObject(CObject::TYPE::PLAYER)); // �v���C���[�ւ̃|�C���^
	int nAnticipationFrame = 120; // ���؂�t���[����

    // �u���؂�t���[�����v�̃L�[�����݂���Ȃ猩�؂�t���[������ݒ肷��
	if (SummonPattern.contains("AnticipationFrame"))
	{
		nAnticipationFrame = SummonPattern["AnticipationFrame"];

	}

	// �v���C���[���K�C�h�˂��O��Ԃɂ���
	if (m_bGuide)
	{
		pPlayer->SetState(DBG_NEW CPlayerState_GuideBeforeThrust(pPlayer, nAnticipationFrame));
	}
	// �v���C���[��˂��O��Ԃɂ���
	else
	{
		pPlayer->SetState(DBG_NEW CPlayerState_BeforeThrust(pPlayer, nAnticipationFrame));
	}
}
