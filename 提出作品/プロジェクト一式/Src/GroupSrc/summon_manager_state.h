//============================================================================
// 
// �Q�O�Q�T�N�T���P�P���F�����}�l�[�W���[��ԃN���X����� [summon_manager_state.h]
// Author : �Ŗ����z
// 
//============================================================================

#ifndef _SUMMON_MANAGER_STATE_H_
#define _SUMMON_MANAGER_STATE_H_	// ��d�C���N���[�h�h�~

//****************************************************
// �C���N���[�h
//****************************************************
#include "object.h"
#include "pch.h"
#include "summon_manager.h"

//****************************************************
// �O���錾
//****************************************************
class CSummonGroupSelector; // �����O���[�v�I��

//****************************************************
// using�f�B���N�e�B�u
//****************************************************
using namespace useful;

//****************************************************
// �����}�l�[�W���[�X�e�[�g�N���X
//****************************************************
class CSummon_Manager_State
{
public:
	// <special function>
	CSummon_Manager_State();      // �R���X�g���N�^
	virtual ~CSummon_Manager_State() = 0;     // �f�X�g���N�^

	// <function>
	virtual void Update(CSummon_Manager * pManager) = 0; // �X�V����

	// <getter>
	const JSON& GetJsonSummonGroup() const; // �������W�F�C�\���擾

	// <getter>
	const JSON& GetJsonSummonPatternList() const; // �����p�^�[�����X�g�擾

	// <getter/setter>
	const int& GetChooseGroup() const; // �I��ł���O���[�v�ԍ��擾
	void SetChooseGroup(int nChoose);  // �I��ł���O���[�v�ԍ��ݒ�

	virtual bool SetSummonGroup(int nSummonGroup); // �O���[�v��������

protected:
	void ChengeState(CSummon_Manager_State* pState,CSummon_Manager * pManager); // �X�e�[�g�ύX����
	void ChangeJsonSummonGroup(int nChooseGroup); // �W�F�C�\���̏����O���[�v��ύX����
private:
	// === �ÓI�����o�ϐ� ===

	static constexpr float
		s_fNormal_ReleaseRangeZ = -400.0f; // ��������j������Z�͈�

	// === �ϐ� ===

	int
		m_nGroup, // �����O���[�v�ԍ�
		m_nDifficultyScore; // ��Փx���㏸������X�R�A
	JSON 
		m_JsonSummonGroup, // �����O���[�v���
	    m_JsonSummonPatternList; // �����p�^�[�����܂Ƃ߂����f���p�X�̃W�F�C�\��
	bool m_bJustSummon; // ���傤�Ǐ����������ǂ���
	CSummon_Manager::DIFFICULTY m_DifficultyRank; // ��Փx�����N

	// === �����o�֐� ===

	void SetEvent(const JSON& SummonPattern); // �C�x���g��ݒ肷��
	virtual void VisionBlockerEvent(); // ����W�Q�C�x���g
	void ReleaseAllVisionBlockSpawner(); // �S�Ă̎���W�Q�I�u�W�F�N�g�X�|�i�[������
	void SmorkEvent(); // ���C�x���g
	void ResetEvent(); // �C�x���g�ݒ胊�Z�b�g
	void DestroyOutOfRange(); // ���͈͊O�ɏo�������I�u�W�F�N�g��j������
	virtual void SetAnticipationFrame(const JSON& SummonPattern); // ���؂莞�Ԃ�ݒ肷��
};
#endif // _Summon_MANAGER_H_