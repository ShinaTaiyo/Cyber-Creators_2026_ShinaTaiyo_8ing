//============================================================================
// 
// �e�N�X�`���}�l�[�W���[ [texture_manager.cpp]
// Author : ���c����
// 
//============================================================================

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "texture_manager.h"

// �f�o�C�X�擾�p
#include "renderer.h"

//============================================================================
// 
// private�����o
// 
//============================================================================

//============================================================================
// �f�t�H���g�R���X�g���N�^
//============================================================================
CTexture_Manager::CTexture_Manager() :
	m_upRegistry(std::make_unique<CRegistry<LPDIRECT3DTEXTURE9>>())
{}

//============================================================================
// �f�X�g���N�^
//============================================================================
CTexture_Manager::~CTexture_Manager()
{
	// �I������
	Finalize();
}

//============================================================================
// ����������
//============================================================================
bool CTexture_Manager::Initialize()
{
	// ���������p�̃����_�I�u�W�F�N�g�쐬
	std::function<LPDIRECT3DTEXTURE9(const std::string&)> fpCreate = 
		[](const std::string& path) -> LPDIRECT3DTEXTURE9
	{
		// �e�N�X�`���̃|�C���^
		LPDIRECT3DTEXTURE9 pElement = nullptr;

		// �f�o�C�X���擾
		auto pDev = CRenderer::GetDeviece();

		// �e�N�X�`���𐶐�
		HRESULT hr = D3DXCreateTextureFromFileA(
			pDev,
			path.c_str(),
			&pElement);

		// �������s
		if (FAILED(hr))
		{
			throw std::runtime_error("Failed to Create Texture");
		}

		// �e�N�X�`���̃|�C���^��Ԃ�
		return pElement;
	};

	// ���W�X�g���̏���������
	if (!m_upRegistry->Initialize(INITIALIZE_PATH, std::move(fpCreate)))
	{
		return false;
	}

	return true;
}

//============================================================================
// �I������
//============================================================================
void CTexture_Manager::Finalize()
{
	// ��������p�̃����_�I�u�W�F�N�g�쐬
	std::function<void(LPDIRECT3DTEXTURE9&)> fpRelease =
		[](LPDIRECT3DTEXTURE9& element) -> void
	{
		// �e�N�X�`�������
		if (element)
		{
			element->Release();
			element = nullptr;
		}
	};

	// ���W�X�g���̏I������
	m_upRegistry->Finalize(fpRelease);
}