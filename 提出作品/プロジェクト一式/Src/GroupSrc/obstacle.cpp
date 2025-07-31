//============================================================================
// 
// �Q�O�Q�T�N�S���Q�W���F�u���b�N�N���X���쐬 [Obstacle.cpp]
// Author : �Ŗ����z
// 
//============================================================================

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "Obstacle.h"
#include "bounding_box.h"
#include "summon_manager.h"
#include "move_info.h"
#include "StencilMask.h" // �X�e���V���}�X�N
#include "OBB.h"
#include "collision.h"
#include "SummonCommonData.h"
#include "summon_manager_state_start.h"
#include "main.h"
#include "game.h"
#include "calculation.h"
#include "Vec3LerpInfo.h"
#include "Vec3ExponertialInfo.h"

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
// �R���X�g���N�^
//============================================================================
CObstacle::CObstacle(bool bIsSummonManagerObj) :
	CObject_X{ LAYER::DEFAULT },
	m_pCollisionInfo_Box{ DBG_NEW CCollisionInfo_BoundingBox() },
	m_Type{OBSTACLETYPE::TYPE00_BLOCK},
	m_bCanbeDestroy{true},
	m_bIsSummonManagerObj{bIsSummonManagerObj},
	m_pOBB{DBG_NEW COBB()},
	m_AddRot{VEC3_INIT},
	m_bUseAddRot{true},
	m_SummonCommonData(this)
{
	// �����}�l�[�W���[�����������I�u�W�F�N�g�Ȃ�
	if (m_bIsSummonManagerObj)
	{
		CSummon_Manager::AddNumObj(); // �����}�l�[�W���[�����������I�u�W�F�N�g���𑝂₷
	}

	// �������񏉊���
	m_pCollisionInfo_Box->SetUpdateInfo(GetPos(), VEC3_INIT, VEC3_INIT, GetSize(), GetVtxMax(), GetVtxMin(), false, false, false, false);
}

//============================================================================
// �f�X�g���N�^
//============================================================================
CObstacle::~CObstacle()
{
	// �����Ȃ�
}

//============================================================================
// �����ݒ�
//============================================================================
HRESULT CObstacle::Init()
{
	// X�I�u�W�F�N�g�N���X�̏����ݒ�
	if (FAILED(CObject_X::Init()))
	{
		assert(false && "�v���C���[�̏����ݒ�Ɏ��s");
	}

	SetType(CObject::TYPE::OBSTACLE); // �^�C�v���u��Q���v�ɐݒ�

	return S_OK;
}

//============================================================================
// �I������
//============================================================================
void CObstacle::Uninit()
{
	// ��������I��
	if (m_pCollisionInfo_Box != nullptr)
	{
		delete m_pCollisionInfo_Box;   
		m_pCollisionInfo_Box = nullptr;
	}

	// OBB����I��
	if (m_pOBB)
	{
		delete m_pOBB;
		m_pOBB = nullptr;
	}

	// �����}�l�[�W���[�����������I�u�W�F�N�g�Ȃ珢���}�l�[�W���[�I�u�W�F�N�g�����f�N�������g
	if (m_bIsSummonManagerObj)
	{
		CSummon_Manager::SubNumObj();
	}

	// X�I�u�W�F�N�g�N���X�̏I������
	CObject_X::Uninit();
}

//============================================================================
// �X�V����
//============================================================================
void CObstacle::Update()
{
	// �g�U����
	if (m_upSpreader)
	{
		CObstacle& r = *this;
		m_upSpreader->Spread(r);
	}

	// �����I�u�W�F�N�g���ʃf�[�^�X�V
	m_SummonCommonData.Update();

	// �����I�u�W�F�N�g���ʃf�[�^���玩�g�̏����X�V����
	UpdateFromSummonCommonData();

	Rotating(); // ��]����

	// X�I�u�W�F�N�g�N���X�̍X�V
	CObject_X::Update();

	// ��������X�V����
	UpdateCollisionInfo();
}

//============================================================================
// �`�揈��
//============================================================================
void CObstacle::Draw()
{
	// �X�e���V���o�b�t�@�̕`��ݒ�
	CStencilMask::SetOtherStencilState();

	// X�I�u�W�F�N�g�N���X�̕`�揈��
	CObject_X::Draw();

	// �X�e���V���o�b�t�@�̕`��ݒ�����ɖ߂�
	CStencilMask::SetOtherResetStencilState();
}

//============================================================================
// OBB���擾
//============================================================================
COBB* CObstacle::GetOBB() const
{
	return m_pOBB;
}

//============================================================================
// �W�F�C�\������������
//============================================================================
void CObstacle::InitFromJson(const JSON& Json)
{
	vector<float> vRotation; // ��]��

	JSON CopyJson = Json;

	// �����I�u�W�F�N�g���ʃf�[�^������
	m_SummonCommonData.InitFromJson(CopyJson);

	// ��]�ʂ�ݒ�
	if (Json.contains("Rot"))
	{
		vRotation = Json["Rot"].get<vector<float>>();
		SetAddRot(D3DXVECTOR3(vRotation[0], vRotation[1], vRotation[2]));
	}

	// �����I�u�W�F�N�g���ʃf�[�^���玩�g�̏����X�V
	UpdateFromSummonCommonData();
}

//============================================================================
// ��ގ擾
//============================================================================
const CObstacle::OBSTACLETYPE& CObstacle::GetObstacleType() const
{
	return m_Type;
}

//============================================================================
// ��ސݒ�
//============================================================================
void CObstacle::SetObstacleType(OBSTACLETYPE Type)
{
	m_Type = Type;
}

//============================================================================
// �������Z�ʐݒ�
//============================================================================
void CObstacle::SetAddRot(D3DXVECTOR3 AddRot)
{
	m_AddRot = AddRot;
}

//============================================================================
// �������Z�ʎ擾
//============================================================================
const D3DXVECTOR3& CObstacle::GetAddRot() const
{
	return m_AddRot;
}

//============================================================================
// ���������Z�������邩�ǂ����擾
//============================================================================
const bool& CObstacle::GetUseAddRot() const
{
	return m_bUseAddRot;
}

//============================================================================
// ���������Z�������邩�ǂ����ݒ�
//============================================================================
void CObstacle::SetUseAddRot(bool bUse)
{
	m_bUseAddRot = bUse;
}

//============================================================================
// �����I�u�W�F�N�g���ʃf�[�^�擾
//============================================================================
CSummonCommonData& CObstacle::GetSummonCommonData()
{
	return m_SummonCommonData;
}

//============================================================================
// �����I�u�W�F�N�g���ʃf�[�^�ݒ�
//============================================================================
void CObstacle::SetSummonCommonData(CSummonCommonData SummonCommonData)
{
	m_SummonCommonData = SummonCommonData;
}

//============================================================================
// �j��\���ǂ����擾
//============================================================================
const bool& CObstacle::GetCanbeDestroy() const
{
	return m_bCanbeDestroy;
}

//============================================================================
// �j��\���ǂ����ݒ�
//============================================================================
void CObstacle::SetCanbeDestroy(bool bDestroy)
{
	m_bCanbeDestroy = bDestroy;
}

//============================================================================
// �����}�l�[�W���[�����������I�u�W�F�N�g���ǂ����擾
//============================================================================
const bool& CObstacle::GetSummonManagerObj() const
{
	return m_bIsSummonManagerObj;
}

//============================================================================
// ����
//============================================================================
CObstacle* CObstacle::Create(const std::string& modelpath, OBSTACLETYPE Type, D3DXVECTOR3 InitPos, bool bIsSummonManagerObj)
{
	// �C���X�^���X�𐶐�
	CObstacle* pObstacle = DBG_NEW CObstacle(bIsSummonManagerObj);

	// �������s
	if (pObstacle == nullptr)
	{
		assert(false);
	}

	// �^�C�v��ݒ�
	pObstacle->SetType(TYPE::OBSTACLE);

	// ��Q���^�C�v�ݒ�
	pObstacle->SetObstacleType(Type);

	// ��Q���^�C�v�ݒ�
	pObstacle->SetObstacleType(CObstacle::OBSTACLETYPE::TYPE00_BLOCK);

	// �����ݒ�
	pObstacle->Init();

	// �����ʒu�̐ݒ�
	pObstacle->SetPos(InitPos);

	// ���f����ݒ�
	pObstacle->BindModel(CX_Manager::GetInstance()->RefRegistry().BindAtPath(modelpath));

	// �F������ݒ�
	//pObstacle->SetUseCol(true);                           // �F�������g�p
	//pObstacle->SetCol(D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f)); // �F������ݒ�

	// �T�C�Y��ݒ�
	pObstacle->SetSize(pObstacle->GetModel()->Size);

	// �ő咸�_�ݒ�
	pObstacle->SetBaseVtxMax(pObstacle->GetModel()->VtxMax);

	// �ŏ����_�ݒ�
	pObstacle->SetBaseVtxMin(pObstacle->GetModel()->VtxMin);

	// �������񏉊���
	pObstacle->m_pCollisionInfo_Box->SetUpdateInfo(pObstacle->GetPos(), VEC3_INIT, VEC3_INIT, pObstacle->GetSize(), pObstacle->GetVtxMax(), pObstacle->GetVtxMin(), false, false, false, false);

	// OBB�����񏉊���
	COBB* pOBB = pObstacle->m_pOBB;
	pOBB->SetPos(pObstacle->GetPos());       // �ʒu�ݒ�
	pOBB->SetPosOld(pObstacle->GetPos());    // 1f�O�̈ʒu�ݒ�
	pOBB->SetVtxMax(pObstacle->GetVtxMax()); // �ő咸�_�ݒ�
	pOBB->SetVtxMin(pObstacle->GetVtxMin()); // �ŏ����_�ݒ�

	return pObstacle;
}

//============================================================================
// 
// private�����o
// 
//============================================================================

//============================================================================
// �ʒu�ҏW
//============================================================================
void CObstacle::ControlInfo_Pos()
{
	float fX = 0.0f; // X�ړ���
	float fZ = 0.0f; // Y�ړ���
	CInputKeyboard* pInputKeyboard = CManager::GetManager()->GetKeyboard(); // �L�[���͏��
	const D3DXVECTOR3 Pos = GetPos(); // �ʒu
	CRenderer::SetDebugString("�ʒu�ҏW�i�\���L�[�j�FX = " + to_string(Pos.x) + "�AY = " + to_string(Pos.y) + "�AZ = " + to_string(Pos.z));

	// === �ړ��������� ===

	if (pInputKeyboard->GetPress(DIK_UP))
	{
		fZ = 1.0f;
	}
	if (pInputKeyboard->GetPress(DIK_DOWN))
	{
		fZ = -1.0f;
	}
	if (pInputKeyboard->GetPress(DIK_RIGHT))
	{
		fX = 1.0f;
	}
	if (pInputKeyboard->GetPress(DIK_LEFT))
	{
		fX = -1.0f;
	}

	// L�V�t�g�������Ă��Ȃ���Ώ㉺���E
	if (!pInputKeyboard->GetPress(DIK_LSHIFT))
	{
		if (fX != 0.0f || fZ != 0.0f)
		{// XY
			float fRotXZ = atan2f(fX, fZ); // �ړ��p�xXZ
			SetPos(GetPos() + D3DXVECTOR3(sinf(fRotXZ) * 1.0f,cosf(fRotXZ) * 1.0f,0.0f));
		}
	}
	// L�V�t�g�������Ă���ΑO��ړ�
	else
	{
		if (fZ != 0.0f)
		{// Z
			SetPos(GetPos() + D3DXVECTOR3(0.0f,0.0f, 1.0f * fZ));
		}
	}
}

//============================================================================
// ��������X�V����
//============================================================================
void CObstacle::UpdateCollisionInfo()
{
	D3DXVECTOR3 Pos = GetPos(); // �ʒu�i�����ɓn������ŕς��j
	const D3DXVECTOR3& CurrentPos = GetPos(); // �Q�Ɨp�ʒu�i�ʒu��ݒ肵��������̈ʒu���g�p�ł���j
	const D3DXVECTOR3& Rot = GetRot();   // ����
	const D3DXVECTOR3& Size = GetSize(); // �T�C�Y
	const D3DXVECTOR3& VtxMax = GetVtxMax(); // �ő咸�_
	const D3DXVECTOR3& VtxMin = GetVtxMin(); // �ŏ����_

	// ��������X�V
	m_pCollisionInfo_Box->SetUpdateInfo(CurrentPos, VEC3_INIT, VEC3_INIT, Size, VtxMax, VtxMin, false, false, false, false);

	// OBB������X�V
	m_pOBB->SetPos(CurrentPos); // �ʒu���X�V
	m_pOBB->SetVtxMax(VtxMax);  // �ő咸�_���X�V
	m_pOBB->SetVtxMin(VtxMin);  // �ŏ����_���X�V
	m_pOBB->SetRot(Rot);        // �������X�V
}

//============================================================================
// �����I�u�W�F�N�g���ʃf�[�^���玩�g�̏����X�V����
//============================================================================
void CObstacle::UpdateFromSummonCommonData()
{
	SetPos(m_SummonCommonData.GetPos());
	SetRot(m_SummonCommonData.GetRot());
	SetScale(m_SummonCommonData.GetScale());
}

//============================================================================
// ��]����
//============================================================================
void CObstacle::Rotating()
{
	// �����̉��Z���J�n
	if (m_bUseAddRot && !m_SummonCommonData.GetExponertialRotInfo().GetUse())
	{
		const D3DXVECTOR3& SelfRot = GetRot();
		SetRot(SelfRot + m_AddRot);

		// �����I�u�W�F�N�g���ʃf�[�^�̌������X�V
		m_SummonCommonData.SetRot(SelfRot);
	}
}

//============================================================================
// �g�U
//============================================================================
void CObstacle::CSpreader::Spread(CObstacle& r)
{
	Vec3 TmpRot = r.GetRot();
	TmpRot += (m_Dir * m_fRate) * 0.015f;
	r.SetRot(TmpRot);

	// �����I�u�W�F�N�g���ʃf�[�^�̌������X�V
	r.GetSummonCommonData().SetRot(r.GetRot());

	Vec3 TmpPos = r.GetPos();
	TmpPos += m_Dir * m_fRate;
	r.SetPos(TmpPos);

	// �����I�u�W�F�N�g���ʃf�[�^�̈ʒu���X�V
	r.GetSummonCommonData().SetPos(r.GetPos());
}