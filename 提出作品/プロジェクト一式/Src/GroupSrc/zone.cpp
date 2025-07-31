//============================================================================
// 
// �Q�O�Q�T�N�T���X���F�Z�[�t�]�[���N���X���쐬 [Zone.cpp]
// Author : �Ŗ����z
// 
//============================================================================

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "zone.h"
#include "summon_manager.h"
#include "bounding_box.h"
#include "SummonCommonData.h"
#include "main.h"
#include "summon_manager_state_start.h"
#include "StencilMask.h" // �X�e���V���}�X�N
#include "game.h"
#include "Vec3ExponertialInfo.h"
#include "calculation.h"
// �I�u�W�F�N�g�����p
#include "texture_manager.h"

// ���͗p
#include "manager.h"

//****************************************************
// �ÓI�����o�錾
//****************************************************
const std::string CZone::s_aSafeZoneTextureName[static_cast<int>(CZone::TYPE::MAX)] =
{
	"SafeZone_000"
};

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
CZone::CZone(bool bIsSummonManagerObj) :
	CObject_3D{ LAYER::DEFAULT },
	m_pCollisionInfo_Box{DBG_NEW CCollisionInfo_BoundingBox()},
	m_SummonCommonData(this),
	m_bIsSummonManagerObj{ bIsSummonManagerObj }
{
	// �����}�l�[�W���[�����������I�u�W�F�N�g�Ȃ�
	if (m_bIsSummonManagerObj)
	{
		CSummon_Manager::AddNumObj(); // �����}�l�[�W���[�����������I�u�W�F�N�g���𑝂₷
	}

	SetType(CObject::TYPE::ZONE); // �^�C�v���u�Z�[�t�]�[���v�ɐݒ�
}

//============================================================================
// �f�X�g���N�^
//============================================================================
CZone::~CZone()
{
	// �����Ȃ�
}

//============================================================================
// �����ݒ�
//============================================================================
HRESULT CZone::Init()
{
	// 3D�I�u�W�F�N�g�N���X�̏����ݒ�
	if (FAILED(CObject_3D::Init()))
	{
		assert(false && "�v���C���[�̏����ݒ�Ɏ��s");
	}

	return S_OK;
}

//============================================================================
// �I������
//============================================================================
void CZone::Uninit()
{
	// �����}�l�[�W���[�����������I�u�W�F�N�g�Ȃ�
	if (m_bIsSummonManagerObj)
	{
		CSummon_Manager::SubNumObj(); // �����}�l�[�W���[�����������I�u�W�F�N�g�������炷
	}

	// ��������I��
	if (m_pCollisionInfo_Box)
	{
		delete m_pCollisionInfo_Box;
		m_pCollisionInfo_Box = nullptr;
	}

	// 3D�I�u�W�F�N�g�N���X�̏I������
	CObject_3D::Uninit();
}

//============================================================================
// �X�V����
//============================================================================
void CZone::Update()
{
	// �����I�u�W�F�N�g���ʃf�[�^�X�V
	m_SummonCommonData.Update();
	
	// �����I�u�W�F�N�g���ʃf�[�^���玩�g�̏����X�V
	UpdateFromSummonCommonData();

	// 3D�I�u�W�F�N�g�N���X�̍X�V
	CObject_3D::Update();

	const D3DXVECTOR3 Size = GetSize(); // �T�C�Y

	// ����������X�V
	m_pCollisionInfo_Box->SetUpdateInfo(
		GetPos(),VEC3_INIT,VEC3_INIT,D3DXVECTOR3(Size.x, Size.y,5.0f),D3DXVECTOR3(Size.x,Size.y,5.0f),D3DXVECTOR3(-Size.x,-Size.y,-5.0f),
		false,false,false,false);
}

//============================================================================
// �`�揈��
//============================================================================
void CZone::Draw()
{
	// 3D�I�u�W�F�N�g�N���X�̕`�揈��
	CObject_3D::Draw();
}

//============================================================================
// �W�F�C�\������������
//============================================================================
void CZone::InitFromJson(const JSON& Json)
{
	JSON CopyJson = Json;

	// �����I�u�W�F�N�g���ʃf�[�^������
	m_SummonCommonData.InitFromJson(CopyJson);

	// �����I�u�W�F�N�g���ʃf�[�^���玩�g�̏����X�V
	UpdateFromSummonCommonData();
}

//============================================================================
// ����
//============================================================================
CZone* CZone::Create(TYPE Type, D3DXVECTOR3 InitPos, D3DXVECTOR3 Size, bool bIsSummmonManagerObj)
{
	// �C���X�^���X�𐶐�
	CZone* pZone = DBG_NEW CZone(bIsSummmonManagerObj);

	if (pZone == nullptr)
	{ // �������s
		assert(false);
	}

	// �^�C�v��ݒ�
	pZone->SetType(CObject::TYPE::ZONE);

	// �����ݒ�
	pZone->Init();

	// �����ʒu�̐ݒ�
	pZone->SetPos(InitPos);

	// �����T�C�Y�̐ݒ�
	pZone->SetInitSize(Size);
	pZone->SetSize(Size);

	// �]�[���̔�����̐ݒ�
	pZone->m_pCollisionInfo_Box->SetUpdateInfo(InitPos, VEC3_INIT, VEC3_INIT,
		D3DXVECTOR3(Size.x, Size.y, 5.0f), D3DXVECTOR3(Size.x, Size.y, 5.0f), D3DXVECTOR3(-Size.x, -Size.y, -5.0f),
		false, false, false, false);

	// �e�N�X�`����ݒ�
	pZone->BindTex(CTexture_Manager::GetInstance()->RefRegistry().BindAtKey(s_aSafeZoneTextureName[static_cast<int>(Type)]));

	// �A�j���[�V�����p�p�����[�^���쐬
	const int
		nChangeRate = 3;
	const float
		fWidth = (1.0f / 4.0f),
		fHeight = (1.0f / 4.0f);

	// ���̃C���X�^���X�Ƀe�N�X�`���A�j���[�^�[������
	Compose(pZone->RefTextureAnimator(),
		[nChangeRate, fWidth, fHeight]() -> up<CTextureAnimator>
		{
			up<CTextureAnimator> up = make_unique<CTextureAnimator>();

			up->SetChangeRate(nChangeRate);
			up->SetTexSplitWidth(fWidth);
			up->SetTexSplitHeight(fHeight);

			return up;
		});

	return pZone;
}

//============================================================================
// ������擾
//============================================================================
CCollisionInfo* CZone::GetCollisionInfo() const
{
	return m_pCollisionInfo_Box;
}

//============================================================================
// �����}�l�[�W���[�����������I�u�W�F�N�g���ǂ����擾
//============================================================================
const bool& CZone::GetSummonManagerObj() const
{
	return m_bIsSummonManagerObj;
}

//============================================================================
// �����I�u�W�F�N�g���ʃf�[�^�擾
//============================================================================
CSummonCommonData& CZone::GetSummonCommonData()
{
	return m_SummonCommonData;
}

//============================================================================
// �����I�u�W�F�N�g���ʃf�[�^�ݒ�
//============================================================================
void CZone::SetSummonCommonData(CSummonCommonData SummonCommonData)
{
	m_SummonCommonData = SummonCommonData;
}

//============================================================================
// �����I�u�W�F�N�g���ʃf�[�^���玩�g�̃f�[�^���X�V����
//============================================================================
void CZone::UpdateFromSummonCommonData()
{
	SetPos(m_SummonCommonData.GetPos());
	SetRot(m_SummonCommonData.GetRot());
	SetScale(m_SummonCommonData.GetScale());
}