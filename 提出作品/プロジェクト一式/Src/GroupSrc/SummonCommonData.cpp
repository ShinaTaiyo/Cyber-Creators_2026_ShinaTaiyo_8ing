//============================================================================
// 
// �Q�O�Q�T�N�U���Q�O���F�����I�u�W�F�N�g�ɋ��ʂ̃f�[�^���������� [SummonCommonData.cpp]
// Author : �Ŗ����z
// 
//============================================================================

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "SummonCommonData.h" 
#include "game.h"
#include "calculation.h"

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
// �R���X�g���N�^�i�f�t�H���g)
//====================================================
CSummonCommonData::CSummonCommonData() : 
	m_pHostObj{nullptr}
{
	// �����Ȃ�
}

//====================================================
// �R���X�g���N�^
//====================================================
CSummonCommonData::CSummonCommonData(CObject* pHostObj) :
	m_Pos{ VEC3_INIT },
	m_Rot{ VEC3_INIT },
	m_Scale{ VEC3_INIT },
	m_LerpPosInfo{},
	m_ExponertialRotInfo{},
	m_LerpScaleInfo{},
	m_pHostObj{pHostObj}
{
	// �����Ȃ�
}

//====================================================
// �f�X�g���N�^
//====================================================
CSummonCommonData::~CSummonCommonData()
{
	// �����Ȃ�
}

//====================================================
// �X�V����
//====================================================
void CSummonCommonData::Update()
{
	Moving(); // �ړ�����
	Rotating(); // ��]����
	Scaling(); // �g�又��
}

//====================================================
// �ʒu�擾
//====================================================
const D3DXVECTOR3& CSummonCommonData::GetPos() const
{
	return m_Pos;
}

//====================================================
// �ʒu�ݒ�
//====================================================
void CSummonCommonData::SetPos(D3DXVECTOR3 Pos)
{
	m_Pos = Pos;
}

//====================================================
// �����擾
//====================================================
const D3DXVECTOR3& CSummonCommonData::GetRot() const
{
	return m_Rot;
}

//====================================================
// �����ݒ�
//====================================================
void CSummonCommonData::SetRot(D3DXVECTOR3 Rot)
{
	m_Rot = Rot;
}

//====================================================
// �g�嗦�擾
//====================================================
const D3DXVECTOR3& CSummonCommonData::GetScale() const
{
	return m_Scale;
}

//====================================================
// �g�嗦�ݒ�
//====================================================
void CSummonCommonData::SetScale(D3DXVECTOR3 Scale)
{
	m_Scale = Scale;
}

//====================================================
// �ړ����擾
//====================================================
CMove_Info& CSummonCommonData::GetMoveInfo()
{
	return m_MoveInfo;
}

//====================================================
// �ʒu���`��ԏ��
//====================================================
CVec3LerpInfo& CSummonCommonData::GetLerpPosInfo()
{
	return m_LerpPosInfo;
}

//====================================================
// �����w���������
//====================================================
CVec3ExponertialInfo& CSummonCommonData::GetExponertialRotInfo()
{
	return m_ExponertialRotInfo;
}

//====================================================
// �W�F�C�\������������
//====================================================
void CSummonCommonData::InitFromJson(JSON& Json)
{
	vector<float> vPos;    // �ʒu
	vector<float> vMove;   // �ړ���
	vector<float> vRot;    // ����
	vector<float> vScale;  // �g�嗦
	vector<float> vRotation; // ��]��

	// �ʒu�ݒ�
	if (Json.contains("Pos"))
	{
		vPos = Json["Pos"].get<vector<float>>();
		// �n���Z���{���̈ʒu���珢���\��ʒu�܂Ő��`��Ԃ���
		m_Pos = D3DXVECTOR3(0.0f,0.0f,500.0f);
		m_LerpPosInfo.SetInitialize(30, m_Pos, D3DXVECTOR3(vPos[0], vPos[1], vPos[2]));
	}
	// �����ݒ�
	if (Json.contains("Rotation"))
	{
		vRot = Json["Rotation"].get<vector<float>>();
		// �ŏ��̌����������_���ɂ��A���X�ɖړI�̌����Ɏw����������
		m_Rot = Calculation::Value::RandVector3(-D3DXVECTOR3(s_fRandumRotRate, s_fRandumRotRate, s_fRandumRotRate), 
			D3DXVECTOR3(s_fRandumRotRate, s_fRandumRotRate, s_fRandumRotRate));
		m_ExponertialRotInfo.SetInitialize(0.15f,D3DXVECTOR3(vRot[0], vRot[1], vRot[2]));
	}
	// �g�嗦�ݒ�
	if (Json.contains("Scale"))
	{
		vScale = Json["Scale"].get<vector<float>>();
		// �ŏ��͂��Ȃ菬�����g�嗦�ɐݒ肵�A�ړI�̊g�嗦�ɐ��`��Ԃ���
		SetScale(D3DXVECTOR3(0.01f, 0.01f, 0.01f));
		m_LerpScaleInfo.SetInitialize(45, GetScale(), D3DXVECTOR3(vScale[0], vScale[1], vScale[2]));
	}

	m_MoveInfo.SetMove(D3DXVECTOR3(0.0f, 0.0f, s_fNormalMoveZ)); // �ړ��ʂ͌Œ肵�܂�
}

//====================================================
// ���O�擾
//====================================================
const std::string& CSummonCommonData::GetName() const
{
	return m_Name;
}

//====================================================
// ���O�ݒ�
//====================================================
void CSummonCommonData::SetName(std::string Name)
{
	m_Name = Name;
}

//============================================================================
// 
// private�����o
//
//============================================================================

//====================================================
// �ړ�����
//====================================================
void CSummonCommonData::Moving()
{
	// �ʒu�̐��`��Ԃ��s��
	if (m_LerpPosInfo.GetUse())
	{
		// �w���������ڕW�ʒu�X�V
		m_LerpPosInfo.Update(m_Pos,m_pHostObj);
	}
	// �ʒu�̐��`��Ԃ����Ȃ��Ȃ畁�ʂɈړ�����
	else
	{
		// �ړ�����
		m_MoveInfo.Update(m_Pos, m_pHostObj);
	}
}

//====================================================
// ��]����
//====================================================
void CSummonCommonData::Rotating()
{
	const bool& bUseExponertialRot = m_ExponertialRotInfo.GetUse(); // �����̎w�����������邩�ǂ���

	// �����̐��`��Ԃ�����Ȃ�s��
	if (bUseExponertialRot)
	{
		m_ExponertialRotInfo.Update(m_Rot,m_pHostObj); // �ڕW�̌����Ɏw������
	}
}

//====================================================
// �g�又��
//====================================================
void CSummonCommonData::Scaling()
{
	m_LerpScaleInfo.Update(m_Scale,m_pHostObj); // �ڕW�̊g�嗦�ɐ��`���
}
