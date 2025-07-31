//============================================================================
// 
// �Q�O�Q�T�N�V���Q�P���F�����}�l�[�W���[�I��������ԃN���X����� [summon_Manager_state_Choose.h]
// Author : �Ŗ����z
// 
//============================================================================

#ifndef _SUMMON_MANAGER_STATE_CHOOSE_H_
#define _SUMMON_MANAGER_STATE_CHOOSE_H_	// ��d�C���N���[�h�h�~

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

//****************************************************
// �����}�l�[�W���[�X�e�[�g�F�J�n�N���X
//****************************************************
class CSummon_Manager_State_Choose : public CSummon_Manager_State
{
public:

	// === ����֐� ===

	CSummon_Manager_State_Choose(bool bGuide);           // �R���X�g���N�^
	~CSummon_Manager_State_Choose() override; // �f�X�g���N�^

	// === ���C�t�T�C�N�������o�֐� ===

	void Update(CSummon_Manager* pManager) override; // �X�V����

	// === �����o�֐� ===

	// �K�C�h���邩�ǂ���
	const bool& GetGuide() const; // �擾
	void SetGuide(bool bGuide);   // �ݒ�

private:
	// === �����o�ϐ� ===

	int m_nChooseGroup; // �I���O���[�v�ԍ�
	bool m_bGuide;      // �K�C�h���邩�ǂ���

	// === �����o�֐� ===

	void SetAnticipationFrame(const JSON& SummonPattern) override; // ���؂莞�Ԃ�ݒ肷��
};
#endif // _Summon_MANAGER_H_