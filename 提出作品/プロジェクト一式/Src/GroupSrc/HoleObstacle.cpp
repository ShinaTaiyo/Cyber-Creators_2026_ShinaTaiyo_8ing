//============================================================================
// 
// �Q�O�Q�T�N�U���P�P���F��������Q���N���X���쐬 [HoleObstacle.cpp]
// Author : �Ŗ����z
// 
//============================================================================

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "HoleObstacle.h"
#include "hole.h"
#include "bounding_box.h"
#include "summon_manager.h"
#include "move_info.h"
#include "OBB.h"
#include "collision.h"
#include "RotatingCylinder.h"

// ���͗p
#include "manager.h"
#include "renderer.h"
#include "input_keyboard.h"
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
// �ÓI�����o�錾
//============================================================================

//============================================================================
// �R���X�g���N�^
//============================================================================
CHoleObstacle::CHoleObstacle(bool bIsSummonManagerObj) :
	CObstacle(bIsSummonManagerObj),
	m_pHole{CHole::Create(VEC3_INIT)}
{
	// �����Ȃ�
}

//============================================================================
// �f�X�g���N�^
//============================================================================
CHoleObstacle::~CHoleObstacle()
{
	// �����Ȃ�
}

//============================================================================
// �����ݒ�
//============================================================================
HRESULT CHoleObstacle::Init()
{
	// ��Q���̏����ݒ�
	if (FAILED(CObstacle::Init()))
	{
		assert(false && "�v���C���[�̏����ݒ�Ɏ��s");
	}

	SetType(CObject::TYPE::HOLEOBSTACLE); // �^�C�v���u��������Q���v�ɐݒ�

	return S_OK;
}

//============================================================================
// �I������
//============================================================================
void CHoleObstacle::Uninit()
{
	// ���̏I������
	if (m_pHole)
	{
		m_pHole->SetRelease(); // ���S�t���O��ݒ�
		m_pHole = nullptr;
	}

	// ��Q���̏I������
	CObstacle::Uninit();
}

//============================================================================
// �X�V����
//============================================================================
void CHoleObstacle::Update()
{
	CObstacle::Update();    // ��Q���X�V

	m_pHole->SetParentPos(GetPos()); // ���ɐe�̈ʒu��ݒ肷��
	m_pHole->SetParentRot(GetRot()); // ���ɐe�̌�����ݒ肷��
	m_pHole->ExtraUpdate(); // ���X�V�i�I�u�W�F�N�g����͌Ă΂�Ȃ��̂Ŏ蓮�ōX�V�j
}

//============================================================================
// �`�揈��
//============================================================================
void CHoleObstacle::Draw()
{
	// ��Q���̕`�揈��
	CObstacle::Draw();
}

//============================================================================
// �W�F�C�\������������
//============================================================================
void CHoleObstacle::InitFromJson(const JSON& Json)
{
	// === �����Ɏg�p�������錾�A������ ===

	vector<float> vHolePos;
	vector<float> vHoleRot;
	vector<float> vHoleScale;
	CRotatingCylinder* pHoleRotCyl = m_pHole->GetRotatingCylinder(); // ���̉�]�~�����

	// === �W�F�C�\�������������J�n ===

	// ���̈ʒu�ݒ�
	if (Json.contains("HoleOffset"))
	{
		vHolePos = Json["HoleOffset"].get<vector<float>>();
		m_pHole->SetPos(D3DXVECTOR3(vHolePos[0], vHolePos[1], vHolePos[2]));
		pHoleRotCyl->SetPos(GetPos()); // ���̉�]�~���ʒu�ݒ�
	}

	// ���̌����ݒ�
	if (Json.contains("HoleRot"))
	{
		vHoleRot = Json["HoleRot"].get<vector<float>>();
		m_pHole->SetRot(D3DXVECTOR3(vHoleRot[0], vHoleRot[1], vHoleRot[2]));
		pHoleRotCyl->SetPos(GetRot()); // ���̉�]�~�������ݒ�
	}

	// ���̊g�嗦�ݒ�
	if (Json.contains("HoleScale"))
	{
		vHoleScale = Json["HoleScale"].get<vector<float>>();
		m_pHole->SetScale(D3DXVECTOR3(vHoleScale[0], vHoleScale[1], vHoleScale[2]));
	}

	CObstacle::InitFromJson(Json); // ��Q���W�F�C�\������������
}

//============================================================================
// �����擾
//============================================================================
CHole* CHoleObstacle::GetHole() const
{
	return m_pHole;
}

//============================================================================
// ����
//============================================================================
CHoleObstacle* CHoleObstacle::Create(const std::string& modelpath, CObstacle::OBSTACLETYPE Type, D3DXVECTOR3 InitPos, bool bIsSummonManagerObj)
{
	// �C���X�^���X�𐶐�
	CHoleObstacle* pHoleObstacle = DBG_NEW CHoleObstacle(bIsSummonManagerObj);

	// �������s
	if (pHoleObstacle == nullptr)
	{
		assert(false);
	}

	// �^�C�v��ݒ�
	pHoleObstacle->SetType(TYPE::HOLEOBSTACLE);

	// �����ݒ�
	pHoleObstacle->Init();

	// �����ʒu�̐ݒ�
	pHoleObstacle->SetPos(InitPos);

	// ���f����ݒ�
	pHoleObstacle->BindModel(CX_Manager::GetInstance()->RefRegistry().BindAtPath(modelpath));

	// �T�C�Y��ݒ�
	pHoleObstacle->SetSize(pHoleObstacle->GetModel()->Size);

	// �ő咸�_�ݒ�
	pHoleObstacle->SetBaseVtxMax(pHoleObstacle->GetModel()->VtxMax);

	// �ŏ����_�ݒ�
	pHoleObstacle->SetBaseVtxMin(pHoleObstacle->GetModel()->VtxMin);

	// OBB�����񏉊���
	COBB* pOBB = pHoleObstacle->GetOBB();        // OBB���ւ̃|�C���^
	pOBB->SetPos(pHoleObstacle->GetPos());       // �ʒu�ݒ�
	pOBB->SetPosOld(pHoleObstacle->GetPos());    // 1f�O�̈ʒu�ݒ�
	pOBB->SetVtxMax(pHoleObstacle->GetVtxMax()); // �ő咸�_�ݒ�
	pOBB->SetVtxMin(pHoleObstacle->GetVtxMin()); // �ŏ����_�ݒ�

	return pHoleObstacle;
}