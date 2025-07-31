//============================================================================
// 
// ���[�V�����Z�b�g [motion_set.cpp]
// Author : ���c����
// 
//============================================================================

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "motion_set.h"
#include "object_Parts.h"

//****************************************************
// using�f�B���N�e�B�u
//****************************************************
using namespace useful;

//============================================================================
// 
// public�����o
// 
//============================================================================

//============================================================================
// �R���X�g���N�^
//============================================================================
CMotion_Set::CMotion_Set(LAYER Layer) :
	CObject{ Layer },
	m_Rot{ VEC3_INIT },
	m_Pos{ VEC3_INIT },
	m_bStop{ false },
	m_wNowMotion{ 0 },
	m_wNowKey{ 0 },
	m_wNowFrame{ 0 },
	m_vpModelParts{},
	m_pMostParent{ nullptr },
	m_wMaxParts{ 0 },
	m_wMaxMotion{ 0 },
	m_vpMotion{}
{
	m_vpModelParts.clear();
	m_vpMotion.clear();
}

//============================================================================
// �f�X�g���N�^
//============================================================================
CMotion_Set::~CMotion_Set()
{

}

//============================================================================
// �����ݒ�
//============================================================================
HRESULT CMotion_Set::Init()
{
	/* ���ۉ��΍� */

	return S_OK;
}

//============================================================================
// �I������
//============================================================================
void CMotion_Set::Uninit()
{
	for (WORD wCntMotion = 0; wCntMotion < m_wMaxMotion; ++wCntMotion)
	{
		// ���[�V�������̃|�C���^���쐬
		CMotion_Set::Motion* const pMotion = &m_vpMotion[wCntMotion];

		for (WORD wCntMotionKey = 0; wCntMotionKey < pMotion->wMaxKey; ++wCntMotionKey)
		{
			// �L�[���̃|�C���^���쐬
			CMotion_Set::Key* const pKey = &pMotion->vpKey[wCntMotionKey];

			// �ڕW�l���̃|�C���^�z���j��
			if (pKey->apDest != nullptr)
			{
				delete[] pKey->apDest;
				pKey->apDest = nullptr;
			}
		}
	}

	for (WORD wCntMotion = 0; wCntMotion < m_wMaxMotion; ++wCntMotion)
	{
		// ���[�V�������ƂɃL�[���x�N�^�[�̃N���A
		m_vpMotion[wCntMotion].vpKey.clear();
	}

	// ���[�V�������̃x�N�^�[�̃N���A
	m_vpMotion.clear();

	// �p�[�c�p�I�u�W�F�N�g�̏���
	for (auto it : m_vpModelParts)
	{
		// �j���\��
		it->SetRelease();
	}

	// ���f���p�[�c�x�N�^�[�̃N���A
	m_vpModelParts.clear();

	// �Ō�ɂ��̃��[�V�����Z�b�g��j���\��
	SetRelease();
}

//============================================================================
// �X�V����
//============================================================================
void CMotion_Set::Update()
{
	// ����
	Animation();

	// �����E���W�̏����p�[�c�ɃV���N��������
	SetSyncRot(m_Rot);
	SetSyncPos(m_Pos);
}

//============================================================================
// �`�揈��
//============================================================================
void CMotion_Set::Draw()
{
	/* ���ۉ��΍� */
}

//============================================================================
// �V���N�������𔽉f
//============================================================================
void CMotion_Set::SetSyncRot(D3DXVECTOR3 Rot)
{
	// �V���N��������e�p�[�c�ɔ��f
	m_pMostParent->SetSyncRot(Rot);
}

//============================================================================
// �V���N�����W�𔽉f
//============================================================================
void CMotion_Set::SetSyncPos(D3DXVECTOR3 Pos)
{
	// �V���N�����W��e�p�[�c�ɔ��f
	m_pMostParent->SetSyncPos(Pos);
}

//============================================================================
// �������擾
//============================================================================
D3DXVECTOR3 CMotion_Set::GetRot() const
{
	return m_Rot;
}

//============================================================================
// ������ݒ�
//============================================================================
void CMotion_Set::SetRot(D3DXVECTOR3 Rot)
{
	m_Rot = Rot;
}

//============================================================================
// ���W���擾
//============================================================================
D3DXVECTOR3 CMotion_Set::GetPos() const
{
	return m_Pos;
}

//============================================================================
// ���W��ݒ�
//============================================================================
void CMotion_Set::SetPos(D3DXVECTOR3 Pos)
{
	m_Pos = Pos;
}

//============================================================================
// �Đ����̃��[�V�����ԍ����擾
//============================================================================
WORD CMotion_Set::GetNowMotion() const
{
	return m_wNowMotion;
}

//============================================================================
// �Đ����̃��[�V�����ԍ���ݒ�
//============================================================================
void CMotion_Set::SetNowMotion(WORD nIdx)
{
	m_bStop = false;
	m_wNowMotion = nIdx;
	m_wNowKey = 0;
	m_wNowFrame = 0;
}

//============================================================================
// �X�g�b�v�t���O���擾
//============================================================================
bool CMotion_Set::GetStopState() const
{
	return m_bStop;
}

//============================================================================
// �e�p�[�c���擾
//============================================================================
CObject_Parts* CMotion_Set::GetParentParts()
{
	return m_pMostParent;
}

//============================================================================
// ����
//============================================================================
CMotion_Set* CMotion_Set::Create(LAYER Layer, TYPE Type, JSON Json)
{
	// ���[�V�����Z�b�g�𐶐�
	CMotion_Set* pNew = DBG_NEW CMotion_Set(Layer);

	// �������s
	if (!pNew)
	{
		return nullptr;
	}

	// ���p�[�c�����擾
	pNew->m_wMaxParts = static_cast<WORD>(Json["MaxParts"]);

	// �p�[�c�����̃p�[�c�I�u�W�F�N�g���s���Đ���
	for (WORD wCntParts = 0; wCntParts < pNew->m_wMaxParts; ++wCntParts)
	{
		pNew->m_vpModelParts.push_back(CObject_Parts::Create(Layer, static_cast<std::string>(Json["ModelType"][wCntParts]), nullptr));

		// �e�p�[�c�I�u�W�F�N�g�Ƀ^�C�v��ݒ肷��
		pNew->m_vpModelParts[wCntParts]->SetType(Type);

		/* ���}���u */
		pNew->m_vpModelParts[wCntParts]->SetPos({ -1000.0f, -1000.0f, -1000.0f });
		pNew->m_vpModelParts[wCntParts]->Update();
	}

	// �������ꂽ�p�[�c�ɑ΂��A�e��ݒ���s��
	for (WORD wCntParts = 0; wCntParts < pNew->m_wMaxParts; ++wCntParts)
	{
		// �e�p�[�c�̃C���f�b�N�X�ԍ����擾
		const SHORT& shParentIdx = static_cast<SHORT>(Json["ParentIdx"][wCntParts]);

		// �p�[�c�̃|�C���^���R�s�[
		CObject_Parts* pParts = pNew->m_vpModelParts[wCntParts];

		if (shParentIdx == -1)
		{
			// �C���f�b�N�X��-1�̂��̂͐e�������Ȃ�
			pParts->SetParent(nullptr);

			// �e�ɂ̂ݏk�ڃI�t�Z�b�g�𔽉f
			pParts->SetScaleOffset(hws::JsonConvertToVec3(Json["ScaleOffset"]));

			// �e�p�[�c���Z�b�g
			pNew->m_pMostParent = pParts;
		}
		else
		{
			// �e�p�[�c�̃|�C���^��n��
			pParts->SetParent(pNew->m_vpModelParts[shParentIdx]);
		}

		// �I�t�Z�b�g�l��ݒ�
		pParts->SetRotOffset(hws::JsonConvertToVec3(Json["RotOffset"][wCntParts]));
		pParts->SetPosOffset(hws::JsonConvertToVec3(Json["PosOffset"][wCntParts]));
	}

	// �����[�V���������擾
	pNew->m_wMaxMotion = static_cast<WORD>(Json["MaxMotion"]);

	// ���[�V���������̃��[�V�������𐶐�
	pNew->m_vpMotion.resize(pNew->m_wMaxMotion);

	// ���[�V�������̐ݒ�
	for (WORD wCntMotion = 0; wCntMotion < pNew->m_wMaxMotion; ++wCntMotion)
	{
		// ���[�V�������̃|�C���^���쐬
		Motion* const pMotion = &pNew->m_vpMotion[wCntMotion];

		// ���[�v�t���O���擾
		pMotion->bLoop = static_cast<bool>(Json["Loop"][wCntMotion]);

		// ���[�V�����̑��L�[�����擾
		pMotion->wMaxKey = static_cast<WORD>(Json["MaxKey"][wCntMotion]);

		// �L�[���̐ݒ�
		for (WORD wCntMotionKey = 0; wCntMotionKey < pMotion->wMaxKey; ++wCntMotionKey)
		{
			// �L�[���I�u�W�F�N�g���쐬
			Key Key;

			// �L�[�̑��t���[�������擾
			Key.nMaxFrame = static_cast<WORD>(Json["MaxFrame"][wCntMotion][wCntMotionKey]);

			// �p�[�c�����̖ڕW�l���𐶐�
			Key.apDest = DBG_NEW CMotion_Set::KeyDest[pNew->m_wMaxParts];

			for (WORD wCntModelParts = 0; wCntModelParts < pNew->m_wMaxParts; ++wCntModelParts)
			{
				// �ڕW�l���̃|�C���^���쐬
				KeyDest* const pDest = &Key.apDest[wCntModelParts];

				// �e��p�����[�^��ݒ�
				pDest->ScaleTarget = hws::JsonConvertToVec3(Json["ScaleTarget"][wCntMotion][wCntMotionKey][wCntModelParts]);	// �ڕW�k��
				pDest->RotTarget = hws::JsonConvertToVec3(Json["RotTarget"][wCntMotion][wCntMotionKey][wCntModelParts]);		// �ڕW����
				pDest->PosTarget = hws::JsonConvertToVec3(Json["PosTarget"][wCntMotion][wCntMotionKey][wCntModelParts]);		// �ڕW���W
			}

			pMotion->vpKey.push_back(Key);
		}
	}

	/* ���[�V�����Z�b�g���̂��̂̃^�C�v�w�� */
	pNew->SetType(Type);

	return pNew;
}

//============================================================================
// 
// protected�����o
// 
//============================================================================

//============================================================================
// ���[�V������ݒ�
//============================================================================
void CMotion_Set::SetMotion(LAYER Layer, JSON Json)
{
	// ���p�[�c�����擾
	m_wMaxParts = static_cast<WORD>(Json["MaxParts"]);

	// �p�[�c�����̃p�[�c�I�u�W�F�N�g���s���Đ���
	for (WORD wCntParts = 0; wCntParts < m_wMaxParts; ++wCntParts)
	{
		m_vpModelParts.push_back(CObject_Parts::Create(Layer, static_cast<std::string>(Json["ModelType"][wCntParts]), nullptr));
	}

	// �������ꂽ�p�[�c�ɑ΂��A�e��ݒ���s��
	for (WORD wCntParts = 0; wCntParts < m_wMaxParts; ++wCntParts)
	{
		// �e�p�[�c�̃C���f�b�N�X�ԍ����擾
		const SHORT& shParentIdx = static_cast<SHORT>(Json["ParentIdx"][wCntParts]);

		// �p�[�c�̃|�C���^���R�s�[
		CObject_Parts* pParts = m_vpModelParts[wCntParts];

		if (shParentIdx == -1)
		{
			// �C���f�b�N�X��-1�̂��̂͐e�������Ȃ�
			pParts->SetParent(nullptr);

			// �e�ɂ̂ݏk�ڃI�t�Z�b�g�𔽉f
			pParts->SetScaleOffset(hws::JsonConvertToVec3(Json["ScaleOffset"]));

			// �e�p�[�c���Z�b�g
			m_pMostParent = pParts;
		}
		else
		{
			// �e�p�[�c�̃|�C���^��n��
			pParts->SetParent(m_vpModelParts[shParentIdx]);
		}

		// �I�t�Z�b�g�l��ݒ�
		pParts->SetRotOffset(hws::JsonConvertToVec3(Json["RotOffset"][wCntParts]));
		pParts->SetPosOffset(hws::JsonConvertToVec3(Json["PosOffset"][wCntParts]));
	}

	// �����[�V���������擾
	m_wMaxMotion = static_cast<WORD>(Json["MaxMotion"]);

	// ���[�V���������̃��[�V�������𐶐�
	m_vpMotion.resize(m_wMaxMotion);

	// ���[�V�������̐ݒ�
	for (WORD wCntMotion = 0; wCntMotion < m_wMaxMotion; ++wCntMotion)
	{
		// ���[�V�������̃|�C���^���쐬
		Motion* const pMotion = &m_vpMotion[wCntMotion];

		// ���[�v�t���O���擾
		pMotion->bLoop = static_cast<bool>(Json["Loop"][wCntMotion]);

		// ���[�V�����̑��L�[�����擾
		pMotion->wMaxKey = static_cast<WORD>(Json["MaxKey"][wCntMotion]);

		// �L�[���̐ݒ�
		for (WORD wCntMotionKey = 0; wCntMotionKey < pMotion->wMaxKey; ++wCntMotionKey)
		{
			// �L�[���I�u�W�F�N�g���쐬
			Key Key;

			// �L�[�̑��t���[�������擾
			Key.nMaxFrame = static_cast<WORD>(Json["MaxFrame"][wCntMotion][wCntMotionKey]);

			// �p�[�c�����̖ڕW�l���𐶐�
			Key.apDest = DBG_NEW CMotion_Set::KeyDest[m_wMaxParts];

			for (WORD wCntModelParts = 0; wCntModelParts < m_wMaxParts; ++wCntModelParts)
			{
				// �ڕW�l���̃|�C���^���쐬
				KeyDest* const pDest = &Key.apDest[wCntModelParts];

				// �e��p�����[�^��ݒ�
				pDest->ScaleTarget = hws::JsonConvertToVec3(Json["ScaleTarget"][wCntMotion][wCntMotionKey][wCntModelParts]);	// �ڕW�k��
				pDest->RotTarget = hws::JsonConvertToVec3(Json["RotTarget"][wCntMotion][wCntMotionKey][wCntModelParts]);		// �ڕW����
				pDest->PosTarget = hws::JsonConvertToVec3(Json["PosTarget"][wCntMotion][wCntMotionKey][wCntModelParts]);		// �ڕW���W
			}

			pMotion->vpKey.push_back(Key);
		}
	}
}

//============================================================================
// 
// private�����o
// 
//============================================================================

//============================================================================
// ����
//============================================================================
void CMotion_Set::Animation()
{
	// �t���[���J�E���g
	CountFrame();

	// �ڕW�l�ւ̕␳
	CorrectTarget();
}

//============================================================================
// �t���[���J�E���g
//============================================================================
void CMotion_Set::CountFrame()
{
	// �X�g�b�v���Ȃ珈�����Ȃ�
	if (m_bStop)
		return;

	// ���݂̃t���[�������C���N�������g
	m_wNowFrame++;

	// �t���[�������A���ݍĐ����̃L�[�̑��t���[�����ɒB������
	if (m_wNowFrame >= GetNowKey()->nMaxFrame)
	{
		// ���݂̃t���[���������Z�b�g
		m_wNowFrame = 0;

		// ���݂̃L�[�����C���N�������g
		m_wNowKey++;

		// �L�[�����A���ݍĐ����̃��[�V�����̑��L�[���ɒB������
		if (m_wNowKey >= GetNowMotionPtr()->wMaxKey)
		{
			// ���݂̃L�[�������Z�b�g
			m_wNowKey = 0;

			// ���[�v���Ȃ����[�V�����Ȃ�
			if (GetNowMotionPtr()->bLoop == false)
				m_bStop = true;
		}
	}
}

//============================================================================
// �ڕW�l�ւ̕␳
//============================================================================
void CMotion_Set::CorrectTarget()
{
	// �X�g�b�v���Ȃ珈�����Ȃ�
	if (m_bStop)
		return;

	// �t���[���̐i�s�x�����쐬 (���t���[���� - ���݂̃t���[��)
	WORD wFrameCoef = static_cast<WORD>(m_vpMotion[m_wNowMotion].vpKey[m_wNowKey].nMaxFrame - m_wNowFrame);

	// �e�p�[�c�ȊO�����ꂼ��̖ڕW�l�֕␳�����p�����[�^��ݒ肷��
	for (WORD wCntModelParts = 0; wCntModelParts < m_wMaxParts; ++wCntModelParts)
	{
		// �ڕW�k��
		Vec3 NewScale = m_vpModelParts[wCntModelParts]->GetScale();
		NewScale += (m_vpMotion[m_wNowMotion].vpKey[m_wNowKey].apDest[wCntModelParts].ScaleTarget - NewScale) / wFrameCoef;
		m_vpModelParts[wCntModelParts]->SetScale(NewScale);

		// �ڕW����
		Vec3 NewRot = m_vpModelParts[wCntModelParts]->GetRot();
		NewRot += (m_vpMotion[m_wNowMotion].vpKey[m_wNowKey].apDest[wCntModelParts].RotTarget - NewRot) / wFrameCoef;
		m_vpModelParts[wCntModelParts]->SetRot(NewRot);

		// �ڕW���W
		Vec3 NewPos = m_vpModelParts[wCntModelParts]->GetPos();
		NewPos += (m_vpMotion[m_wNowMotion].vpKey[m_wNowKey].apDest[wCntModelParts].PosTarget - NewPos) / wFrameCoef;
		m_vpModelParts[wCntModelParts]->SetPos(NewPos);
	}
}

//============================================================================
// ���݂̃��[�V�����̃|�C���^���擾
//============================================================================
CMotion_Set::Motion* const CMotion_Set::GetNowMotionPtr()
{
	return &m_vpMotion[m_wNowMotion];
}

//============================================================================
// ���݂̃L�[�̃|�C���^���擾
//============================================================================
CMotion_Set::Key* const CMotion_Set::GetNowKey()
{
	return &GetNowMotionPtr()->vpKey[m_wNowKey];
}