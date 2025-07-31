//============================================================================
// 
// �I�u�W�F�N�g�Ǘ� [object.cpp]
// Author : ���c����
// 
//============================================================================

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "object.h"
#include "renderer.h"
#include "summon_manager_state_start.h"

//****************************************************
// �ÓI�����o�̏�����
//****************************************************
int		 CObject::m_nNumAll = 0;								// �I�u�W�F�N�g����
CObject* CObject::m_pTop[static_cast<WORD>(LAYER::MAX)] = {};	// �擪�I�u�W�F�N�g�̃|�C���^
CObject* CObject::m_pCur[static_cast<WORD>(LAYER::MAX)] = {};	// �I�[�I�u�W�F�N�g�̃|�C���^

//============================================================================
// 
// public�����o
// 
//============================================================================

//============================================================================
// �R���X�g���N�^
//============================================================================
CObject::CObject() :
	m_wPriority{ static_cast<WORD>(LAYER::BG) },
	m_pPrev{ nullptr },
	m_pNext{ nullptr },
	m_Type{ TYPE::NONE },
	m_bDeath{ false },
	m_ObjType{ OBJTYPE::NONE },
	m_pTimeScaleInfo{ DBG_NEW CTimeScale_Info() },
	m_bPause{ false },
	m_bIsSummonManagerObj{ false }
{
	// ���̃I�u�W�F�N�g�����X�g�ɓo�^
	if (m_pCur[m_wPriority] == nullptr)
	{ // �I�[�I�u�W�F�N�g������ (�I�u�W�F�N�g��1�����݂��Ȃ�)

		// ���̃I�u�W�F�N�g��擪�ƏI�[�ɓo�^
		m_pTop[m_wPriority] = this;
		m_pCur[m_wPriority] = this;
	}
	else
	{ // �I�[�I�u�W�F�N�g������

		// ���݂̏I�[�����̃I�u�W�F�N�g�̑O�Ƃ��ēo�^
		m_pPrev = m_pCur[m_wPriority];

		// �V���ȏI�[�Ƃ��Ă��̃I�u�W�F�N�g��o�^
		m_pCur[m_wPriority] = this;

		// �O�̃I�u�W�F�N�g�̎��ɂ��̃I�u�W�F�N�g��o�^
		m_pPrev->m_pNext = this;
	}

	// �������J�E���g�A�b�v
	m_nNumAll++;
}

//============================================================================
// �`��D��x�w��R���X�g���N�^
//============================================================================
CObject::CObject(LAYER Priority) :
	m_wPriority{ static_cast<WORD>(Priority) },
	m_pPrev{ nullptr },
	m_pNext{ nullptr },
	m_Type{ TYPE::NONE },
	m_bDeath{ false },
	m_ObjType{ OBJTYPE::NONE },
	m_pTimeScaleInfo{ DBG_NEW CTimeScale_Info() },
	m_bPause{ false },
	m_bIsSummonManagerObj{ false }
{
	// ���̃I�u�W�F�N�g�����X�g�ɓo�^
	if (m_pCur[m_wPriority] == nullptr)
	{ // �I�[�I�u�W�F�N�g������ (�I�u�W�F�N�g��1�����݂��Ȃ�)
		
		// ���̃I�u�W�F�N�g��擪�ƏI�[�ɓo�^
		m_pTop[m_wPriority] = this;
		m_pCur[m_wPriority] = this;
	}
	else
	{ // �I�[�I�u�W�F�N�g������

		// ���݂̏I�[�����̃I�u�W�F�N�g�̑O�Ƃ��ēo�^
		m_pPrev = m_pCur[m_wPriority];

		// �V���ȏI�[�Ƃ��Ă��̃I�u�W�F�N�g��o�^
		m_pCur[m_wPriority] = this;

		// �O�̃I�u�W�F�N�g�̎��ɂ��̃I�u�W�F�N�g��o�^
		m_pPrev->m_pNext = this;
	}

	// �������J�E���g�A�b�v
	m_nNumAll++;
}

//============================================================================
// �f�X�g���N�^
//============================================================================
CObject::~CObject()
{
	// �^�C���X�P�[�����I��
	if (m_pTimeScaleInfo != nullptr)
	{
		delete m_pTimeScaleInfo;    // ���
		m_pTimeScaleInfo = nullptr; // �|�C���^������
	}
}

//============================================================================
// ����ݒ�
//============================================================================
void CObject::SetRelease()
{
	if (!m_bDeath)
	{
		m_bDeath = true;
	}
}

//============================================================================
// �������
//============================================================================
void CObject::Release()
{
	int nPriority = m_wPriority;	// �v���C�I���e�B���R�s�[���Ă���

	if (m_pPrev == nullptr)
	{ // �O�̃I�u�W�F�N�g������ (���̃I�u�W�F�N�g���擪)

		if (m_pNext == nullptr)
		{ // ���̃I�u�W�F�N�g������ (�I�u�W�F�N�g���S�Ė����Ȃ�)
			
			m_pTop[nPriority] = nullptr;	// �擪�I�u�W�F�N�g�̃|�C���^��������
			m_pCur[nPriority] = nullptr;	// �I�[�I�u�W�F�N�g�̃|�C���^��������
		}
		else
		{ // ���̃I�u�W�F�N�g�͂���

			// �V���Ȑ擪�Ƃ��Ď��̃I�u�W�F�N�g��o�^
			m_pTop[nPriority] = m_pNext;

			// ���̃I�u�W�F�N�g�̑O�̃|�C���^��������
			m_pNext->m_pPrev = nullptr;
		}
	}
	else
	{ // �O�̃I�u�W�F�N�g������

		if (m_pNext == nullptr)
		{ // ���̃I�u�W�F�N�g������ (���̃I�u�W�F�N�g���I�[)

			// �V���ȏI�[�Ƃ��đO�̃I�u�W�F�N�g�o�^
			m_pCur[nPriority] = m_pPrev;

			// �O�̃I�u�W�F�N�g�̎��̃|�C���^��������
			/* ���������u���̃I�u�W�F�N�g�̎��̃|�C���^(m_pNext)��n���v�Ƃ����s�ׂȂ�A���g�̗L���ɂ�����炸�Ȃ���̂ł� */
			m_pPrev->m_pNext = nullptr;
		}
		else
		{ // ���̃I�u�W�F�N�g������

			// �O�̃I�u�W�F�N�g�̎��ɂ��̃I�u�W�F�N�g�̎���o�^
			m_pPrev->m_pNext = m_pNext;

			// ���̃I�u�W�F�N�g�̑O��
			m_pNext->m_pPrev = m_pPrev;
		}
	}

	Uninit();		// �I������
	delete this;	// �����������
	m_nNumAll--;	// �������J�E���g�_�E��
}

//============================================================================
// ���S�t���O���擾
//============================================================================
const bool& CObject::GetDeath() const
{
	return m_bDeath;
}

//============================================================================
// �^�C�v���擾
//============================================================================
const CObject::TYPE& CObject::GetType() const
{
	return m_Type;
}

//============================================================================
// �^�C�v��ݒ�
//============================================================================
void CObject::SetType(TYPE Type)
{
	m_Type = Type;
}

//============================================================================
// �^�C���X�P�[�������擾
//============================================================================
CTimeScale_Info* CObject::GetTimeScaleInfo() const
{
	return m_pTimeScaleInfo;
}

//============================================================================
// �I�u�W�F�N�g�^�C�v�擾
//============================================================================
const CObject::OBJTYPE& CObject::GetObjType() const
{
	return m_ObjType;
}

//============================================================================
// �I�u�W�F�N�g���ރ^�C�v��ݒ�
//============================================================================
void CObject::SetObjType(OBJTYPE ObjType)
{
	m_ObjType = ObjType;
}

//============================================================================
// ���S�t���O�擾
//============================================================================
const bool& CObject::GetRelease() const
{
	return m_bDeath;
}

//============================================================================
// �|�[�Y�t���O�擾
//============================================================================
const bool& CObject::GetPause() const
{
	return m_bPause;
}

//============================================================================
// �|�[�Y�t���O�ݒ�
//============================================================================
void CObject::SetPause(bool bPause)
{
	m_bPause = bPause;
}

//============================================================================
// �O�̃|�C���^���擾
//============================================================================
CObject* CObject::GetPrev()
{
	return m_pPrev;
}

//============================================================================
// ���̃|�C���^���擾
//============================================================================
CObject* CObject::GetNext()
{
	return m_pNext;
}

//============================================================================
// �S�������
//============================================================================
void CObject::ReleaseAll()
{
	for (int nCntPriority = 0; nCntPriority < static_cast<WORD>(LAYER::MAX); nCntPriority++)
	{
		// �擪�I�u�W�F�N�g�̃|�C���^���R�s�[
		CObject* pObj = m_pTop[nCntPriority];

		// ���̃I�u�W�F�N�g�������Ȃ�܂�
		while (pObj != nullptr)
		{
			// ���̃I�u�W�F�N�g�̃|�C���^���R�s�[
			CObject* pNext = pObj->m_pNext;

			// �������
			pObj->Release();

			// ���̃I�u�W�F�N�g�����̃|�C���^�ŏ㏑��
			pObj = pNext;
		}
	}

	// UI�e�L�X�g�̔w�i��2D�|���S�����g�p���Ă���̂ŁAUI�e�L�X�g���S�ăN���A
	CRenderer::GetRenderer()->GetUiTextManager().Clear();
}

//============================================================================
// �S���S�t���O�ݒ菈��
//============================================================================
void CObject::AllSetRelease()
{
	for (int nCntPriority = 0; nCntPriority < static_cast<WORD>(LAYER::MAX); nCntPriority++)
	{
		// �擪�I�u�W�F�N�g�̃|�C���^���R�s�[
		CObject* pObj = m_pTop[nCntPriority];

		// ���̃I�u�W�F�N�g�������Ȃ�܂�
		while (pObj != nullptr)
		{
			// ���̃I�u�W�F�N�g�̃|�C���^���R�s�[
			CObject* pNext = pObj->m_pNext;

			// ����\��ɑΉ����Ă�����̂�
			if (pObj->GetType() != CObject::TYPE::NOALLRELEASE)
			{
				// �������
				pObj->SetRelease();
			}

			// ���̃I�u�W�F�N�g�����̃|�C���^�ŏ㏑��
			pObj = pNext;
		}
	}

	// UI�e�L�X�g�̔w�i��2D�|���S�����g�p���Ă���̂ŁAUI�e�L�X�g���S�ăN���A
	CRenderer::GetRenderer()->GetUiTextManager().Clear();
}

//============================================================================
// �S�X�V����
//============================================================================
void CObject::UpdateAll()
{
#ifdef _DEBUG	// �I�u�W�F�N�g����\��

	CRenderer::SetDebugString("���݂̃I�u�W�F�N�g��:" + std::to_string(m_nNumAll));

#endif	// _DEBUG

	for (int nCntPriority = 0; nCntPriority < static_cast<WORD>(LAYER::MAX); nCntPriority++)
	{
		// �擪�I�u�W�F�N�g�̃|�C���^���R�s�[
		CObject* pObj = m_pTop[nCntPriority];

		// ���̃I�u�W�F�N�g�������Ȃ�܂�
		while (pObj != nullptr)
		{
			// ���̃I�u�W�F�N�g�̃|�C���^���R�s�[
			CObject* pNext = pObj->m_pNext;

			// �|�[�Y�t���O���I�t�Ȃ�
			if (!pObj->m_bPause)
			{
				// �X�V����
				pObj->Update();
			}

			// �^�C���X�P�[�����X�V
			pObj->m_pTimeScaleInfo->Update();

			// ���̃I�u�W�F�N�g�̃|�C���^���R�s�[
			pObj = pNext;
		}
	}
}

//============================================================================
// �S��X�V����
//============================================================================
void CObject::LateUpdateAll()
{
	for (int nCntPriority = 0; nCntPriority < static_cast<WORD>(LAYER::MAX); nCntPriority++)
	{
		// �擪�I�u�W�F�N�g�̃|�C���^���R�s�[
		CObject* pObj = m_pTop[nCntPriority];

		// ���̃I�u�W�F�N�g�������Ȃ�܂�
		while (pObj != nullptr)
		{
			// ���̃I�u�W�F�N�g�̃|�C���^���R�s�[
			CObject* pNext = pObj->m_pNext;

			// ���S�t���O���o�Ă�����
			if (pObj->m_bDeath)
			{
				// �������
				pObj->Release();
			}

			// ���̃I�u�W�F�N�g�̃|�C���^���R�s�[
			pObj = pNext;
		}
	}

#ifdef _DEBUG
#if 0	/* �X�e���V���e�X�g�̐ݒ�ύX */
	ImGui::SetNextWindowSize({ -1, -1 });
	ImGui::SetNextWindowPos({ 0, 0 }, ImGuiCond_FirstUseEver);
	if (ImGui::Begin("Stencil - All Obect"))
	{
		D3DCMP_GREATEREQUAL;
		ImGui::InputInt("StencilRefValue", &StencilRefValue);
		ImGui::InputInt("StencilCmp", &StencilCmp);
		ImGui::InputInt("Pass", &Pass);
		ImGui::InputInt("ZFail", &ZFail);
		ImGui::InputInt("Fail", &Fail);
		ImGui::End();
	}
#endif
#endif // _DEBUG
}

//============================================================================
// �S�`�揈��
//============================================================================
void CObject::DrawAll()
{
	for (int nCntPriority = 0; nCntPriority < static_cast<WORD>(LAYER::MAX); nCntPriority++)
	{
		// �擪�I�u�W�F�N�g�̃|�C���^���R�s�[
		CObject* pObj = m_pTop[nCntPriority];

		// ���̃I�u�W�F�N�g�������Ȃ�܂�
		while (pObj != nullptr)
		{
			// ���̃I�u�W�F�N�g�̃|�C���^���R�s�[

			CObject* pNext = pObj->m_pNext;

			// �`�揈��
			pObj->Draw();

			// ���̃I�u�W�F�N�g�̃|�C���^���R�s�[
			pObj = pNext;
		}
	}
}

//============================================================================
// �擪�I�u�W�F�N�g�̃|�C���^�擾
//============================================================================
CObject* CObject::GetTopObject()
{
	return *m_pTop;
}

//============================================================================
// �擪�I�u�W�F�N�g�̃|�C���^�擾
//============================================================================
CObject* CObject::GetTopObject(WORD wPriority)
{
	return m_pTop[wPriority];
}

//============================================================================
// �擪�I�u�W�F�N�g�̃|�C���^�擾
//============================================================================
CObject* CObject::GetTopObject(LAYER Priority)
{
	return m_pTop[static_cast<WORD>(Priority)];
}

//============================================================================
// ����^�C�v�̃I�u�W�F�N�g��T��
//============================================================================
CObject* CObject::FindSpecificObject(TYPE Type)
{
	for (int nCntPriority = 0; nCntPriority < static_cast<WORD>(LAYER::MAX); nCntPriority++)
	{
		// �擪�I�u�W�F�N�g�̃|�C���^���R�s�[
		CObject* pObj = m_pTop[nCntPriority];

		// ���̃I�u�W�F�N�g�������Ȃ�܂�
		while (pObj != nullptr)
		{
			if (pObj->GetType() == Type)
			{
				// �^�C�v��v�Ȃ�Ԃ�
				return pObj;
			}

			// ���̃I�u�W�F�N�g�̃|�C���^���R�s�[
			pObj = pObj->m_pNext;
		}
	}

	return nullptr;
}

//============================================================================
// ����^�C�v�̃I�u�W�F�N�g�����擾
//============================================================================
int CObject::CountSpecificObject(TYPE Type)
{
	// ���J�E���g�p
	int nCount = 0;

	for (int nCntPriority = 0; nCntPriority < static_cast<WORD>(LAYER::MAX); nCntPriority++)
	{
		// �擪�I�u�W�F�N�g�̃|�C���^���R�s�[
		CObject* pObj = m_pTop[nCntPriority];

		// ���̃I�u�W�F�N�g�������Ȃ�܂�
		while (pObj != nullptr)
		{
			if (pObj->GetType() == Type)
			{
				nCount++;
			}

			pObj = pObj->GetNext();
		}
	}

	return nCount;
}