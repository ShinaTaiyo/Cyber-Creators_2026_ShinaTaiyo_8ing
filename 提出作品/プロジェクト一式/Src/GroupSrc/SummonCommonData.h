//============================================================================
// 
// �Q�O�Q�T�N�U���Q�O���F�����O���[�v���ʃf�[�^�N���X�����[SummonCommonData.h]
// Author : �Ŗ����z
// 
//============================================================================

//****************************************************
// ��d�C���N���[�h�h�~
//****************************************************
#ifndef _SUMMONCOMMONDATA_H_
#define _SUMMONCOMMONDATA_H_

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "Vec3LerpInfo.h" // �R�����x�N�g�����`��ԏ��
#include "move_info.h" // �ړ����
#include "OBB.h" // OBB���
#include "timescale_info.h" // �^�C���X�P�[�����
#include "Vec3ExponertialInfo.h" // �w���������

//****************************************************
// �����I�u�W�F�N�g�����ʂŎ����Ă���f�[�^�N���X
//****************************************************
class CSummonCommonData
{
public:
	// === ����֐� ===
	CSummonCommonData(); // �f�t�H���g�R���X�g���N�^
	CSummonCommonData(
		CObject * pHostObj // ���̃N���X�������Ă���h��ւ̃|�C���^
	); // �R���X�g���N�^
	~CSummonCommonData(); // �f�X�g���N�^

	// === ���C�t�T�C�N�������o�֐� ===
	void Update(); // �X�V����

	// === �����o�֐� ===

	// �ʒu
	const D3DXVECTOR3& GetPos() const; // �擾
	void SetPos(D3DXVECTOR3 Pos); // �ݒ�

	// ����
	const D3DXVECTOR3& GetRot() const; // �擾
	void SetRot(D3DXVECTOR3 Rot); // �ݒ�

	// �g�嗦
	const D3DXVECTOR3& GetScale() const; // �擾
	void SetScale(D3DXVECTOR3 Scale); // �g�嗦

	// �ړ����
	CMove_Info& GetMoveInfo(); // �擾

	// �ʒu���`��ԏ��
	CVec3LerpInfo& GetLerpPosInfo(); // �擾

	// �����w���������
	CVec3ExponertialInfo& GetExponertialRotInfo(); // �擾

	// �W�F�C�\��������
	void InitFromJson(JSON& Json);

	// ���O
	const std::string& GetName() const; // �擾
	void SetName(std::string Name); // �ݒ�

private:
	// === �ÓI�����o�ϐ� ===
	static constexpr float
		s_fNormalMoveZ = -15.0f, // �ʏ��Z�ړ���
		s_fRandumRotRate = 20.0f; // �ŏ��̌����̗����l

	// === �����o�ϐ� ===
	D3DXVECTOR3
		m_Pos,   // �ʒu
		m_Rot,   // ����
		m_Scale; // �g�嗦
	std::string m_Name; // ���O
	CMove_Info m_MoveInfo; // �ړ����
	CVec3LerpInfo m_LerpPosInfo; // ���`��Ԉʒu���
	CVec3LerpInfo m_LerpScaleInfo; // ���`��Ԋg����
	CVec3ExponertialInfo m_ExponertialRotInfo; // �w��������]���
	CTimeScale_Info m_TimeScaleInfo; // �^�C���X�P�[�����
	CObject* m_pHostObj; // ���̃N���X�������Ă���h��ւ̃|�C���^
	bool m_bUse; // �g�p���邩�ǂ���

	// === �����o�֐� ===
	void Moving(); // �ړ�����
	void Rotating(); // ��]����
	void Scaling(); // �g�又��
};
#endif // _COLLISION_H_