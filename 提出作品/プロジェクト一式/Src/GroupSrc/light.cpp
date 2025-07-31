//============================================================================
// 
// ���C�g [light.cpp]
// Author : ���c����
// 
//============================================================================

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "light.h"

// �L�[�{�[�h�擾�p
#include "manager.h"

// �f�o�C�X�擾�p
#include "renderer.h"

//============================================================================
// �R���X�g���N�^
//============================================================================
CLight::CLight()
{
	// ���C�g���N���A
	ZeroMemory(&m_light[0], sizeof(D3DLIGHT9));

	for (int i = 0; i < MAX_LIGHT; i++)
	{ // ���C�g�̌����N���A
		m_tempDirection[i] = { 0.0f, 0.0f, 0.0f };
	}

	// ����ԍ����Z�b�g
	m_nID = 0;

	// �����̃e���v�����Z�b�g
	SetTempDirection();

	// ���C�g����
	Create();

	// ���񔽉f
	SetLight();
}

//============================================================================
// �f�X�g���N�^
//============================================================================
CLight::~CLight()
{

}

//============================================================================
// �����ݒ�
//============================================================================
HRESULT CLight::Init()
{
	return S_OK;
}

//============================================================================
// �X�V����
//============================================================================
void CLight::Update()
{
#if 0
#ifdef _DEBUG
	// ����ԍ��ύX
	ChangeID();

	// ����
	Control();

	// ���Z�b�g
	Reset();

	// ���C�g���Z�b�g
	SetLight();
#endif	// _DEBUG
#endif
}

//============================================================================
// �����̃e���v�����Z�b�g
//============================================================================
void CLight::SetTempDirection()
{
	// ���C�g�̌����̃e���v�����Z�b�g
	m_tempDirection[0] = { 0.2f, -0.8f, 0.4f };
	m_tempDirection[1] = { -0.2f, 0.8f, -0.4f };
	m_tempDirection[2] = { 0.8f, 0.1f, 0.4f };
}

//============================================================================
// ���C�g����
//============================================================================
void CLight::Create()
{
	// �f�o�C�X���擾
	LPDIRECT3DDEVICE9 pDev = CRenderer::GetDeviece();

	for (int i = 0; i < MAX_LIGHT; i++)
	{ // ���C�g��񏉊��ݒ�

		// ���C�g�̎�ނ�ݒ�
		m_light[i].Type = D3DLIGHT_DIRECTIONAL;

		// ���C�g�̊g�U����ݒ�
		m_light[i].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// ���C�g�̕�����ݒ�
		m_light[i].Direction = m_tempDirection[i];

		// ���C�g��L���ɂ���
		pDev->LightEnable(i, TRUE);
	}
}

//============================================================================
// ����ԍ��ύX
//============================================================================
void CLight::ChangeID()
{
	// ���C�g�ԍ��ύX
	if (CManager::GetKeyboard()->GetTrigger(DIK_LSHIFT))
	{
		m_nID < MAX_LIGHT ? m_nID++ : m_nID = 0;
	}
}

//============================================================================
// ���C�g����
//============================================================================
void CLight::Control()
{
	// X�����ύX
	if (CManager::GetKeyboard()->GetPress(DIK_5))
	{
		m_light[m_nID].Direction.x += 0.1f;
	}
	else if (CManager::GetKeyboard()->GetPress(DIK_6))
	{
		m_light[m_nID].Direction.x -= 0.1f;
	}

	// Y�����ύX
	if (CManager::GetKeyboard()->GetPress(DIK_7))
	{
		m_light[m_nID].Direction.y += 0.1f;
	}
	else if (CManager::GetKeyboard()->GetPress(DIK_8))
	{
		m_light[m_nID].Direction.y -= 0.1f;
	}

	// Z�����ύX
	if (CManager::GetKeyboard()->GetPress(DIK_9))
	{
		m_light[m_nID].Direction.z += 0.1f;
	}
	else if (CManager::GetKeyboard()->GetPress(DIK_0))
	{
		m_light[m_nID].Direction.z -= 0.1f;
	}
}

//============================================================================
// ���Z�b�g
//============================================================================
void CLight::Reset()
{
	if (CManager::GetKeyboard()->GetTrigger(DIK_R))
	{
		// ���C�g�̕��������Z�b�g
		m_light[m_nID].Direction = m_tempDirection[m_nID];
	}
}

//============================================================================
// ���C�g���Z�b�g
//============================================================================
void CLight::SetLight()
{
	// �f�o�C�X���擾
	LPDIRECT3DDEVICE9 pDev = CRenderer::GetDeviece();

	for (int i = 0; i < MAX_LIGHT; i++)
	{
		// �x�N�g�����R�s�[
		D3DXVECTOR3 vecDir = m_light[i].Direction;

		// �x�N�g���𐳋K��
		D3DXVec3Normalize(&vecDir, &vecDir);

		// ���K���x�N�g���𔽉f
		m_light[i].Direction = vecDir;

		// ���C�g��ݒ肷��
		pDev->SetLight(i, &m_light[i]);
	}
}
//============================================================================
// ���C�g���擾
//============================================================================
D3DXVECTOR3 CLight::GetLightDir()
{
	return m_light[0].Direction;   // ���C�g��0�Ԗڂ��擾
}
