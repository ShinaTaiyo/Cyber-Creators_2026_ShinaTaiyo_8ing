//============================================================================
// 
// �Q�O�Q�T�N�T���Q�O���F�J������ԃN���X����� [camera_state_Normal.h]
// Author : �Ŗ����z
// 
//============================================================================

#ifndef _CAMERA_STATE_NORMAL_H_
#define _CAMERA_STATE_NORMAL_H_	// ��d�C���N���[�h�h�~

//****************************************************
// �C���N���[�h
//****************************************************
#include "camera_state.h" // �J�������

//****************************************************
// using�f�B���N�e�B�u
//****************************************************
using namespace useful;

//****************************************************
// �J�����X�e�[�g�N���X
//****************************************************
class CCamera_State_Normal : public CCamera_State
{
public:
	// === ����֐� ===

	CCamera_State_Normal();  // �R���X�g���N�^
	~CCamera_State_Normal(); // �f�X�g���N�^

	// === ���C�t�T�C�N���֐� ===

	void Update(CCamera* pCamera) override; // �X�V����
private:
};
#endif // _Summon_MANAGER_H_