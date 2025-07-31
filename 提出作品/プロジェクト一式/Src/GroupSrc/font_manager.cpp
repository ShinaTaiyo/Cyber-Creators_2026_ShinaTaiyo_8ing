//============================================================================
// 
// �t�H���g�}�l�[�W���[ [font_manager.cpp]
// Author : ���c����
// 
//============================================================================

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "font_manager.h"

// �e���v���[�g��`�t�@�C��
#include "register.tpp"

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
CFont_Manager::CFont_Manager() :
	m_upRegister(std::make_unique<CRegister<LPD3DXFONT>>())
{}

//============================================================================
// �f�X�g���N�^
//============================================================================
CFont_Manager::~CFont_Manager()
{
	// �I������
	Finalize();
}

//============================================================================
// ����������
//============================================================================
bool CFont_Manager::Initialize()
{
	// ���������p�̃����_�I�u�W�F�N�g�쐬
	std::function<LPD3DXFONT(const std::string&)> fpCreate =
		[](const std::string& path) -> LPD3DXFONT
	{
		// �t�H���g�̃|�C���^
		LPD3DXFONT pElement = nullptr;

		// �f�o�C�X���擾
		auto pDev = CRenderer::GetDeviece();

		// �K�v�Ȓǉ������擾
		const JSON& Json = hws::SafeOpenJsonFile(path);

		// �e�p�����[�^���L���X�g
		int
			nHeight = static_cast<int>(Json["Height"]),					// �����̑���
			nWidth = static_cast<int>(Json["Width"]),					// �����̕�
			nMipLevels = static_cast<int>(Json["MipLevels"]);			// �~�b�v�}�b�v�̃��x��
		const std::string& Name = hws::JsonConvertToSJIS(Json["Name"]);	// �t�H���g��

		// �t�H���g�𐶐�
		HRESULT hr = D3DXCreateFont(
			pDev,				// �f�o�C�X�̃|�C���^
			nHeight,			// �����̑���
			nWidth,				// �����̕�
			FW_HEAVY,			// �����̑���
			nMipLevels,			// �~�b�v�}�b�v�̃��x��
			FALSE,				// �C�^���b�N�̎w��
			SHIFTJIS_CHARSET,	// �����Z�b�g�̎w��
			OUT_DEFAULT_PRECIS,	// �o�͐��x
			DEFAULT_QUALITY,	// �o�͕i��
			DEFAULT_PITCH,		// �t�H���g�̃s�b�`
			Name.c_str(),		// �t�H���g��
			&pElement);			// �t�H���g�̃|�C���^

		// �������s
		if (FAILED(hr))
		{
			throw std::runtime_error("Failed to Create Texture");
		}

		// �쐬�������\�[�X�̃|�C���^��Ԃ�
		return pElement;
	};

	// ���W�X�^�̏���������
	if (!m_upRegister->Initialize(INITIALIZE_PATH, std::move(fpCreate)))
	{
		return false;
	}

	return true;
}

//============================================================================
// �I������
//============================================================================
void CFont_Manager::Finalize()
{
	// ��������p�̃����_�I�u�W�F�N�g�쐬
	std::function<void(LPD3DXFONT&)> fpRelease = 
		[](LPD3DXFONT& element) -> void
	{
		// ���\�[�X�����݂�����
		if (element)
		{
			// ������A�|�C���^��������
			element->Release();
			element = nullptr;
		}
	};

	// ���W�X�^�̏I������
	m_upRegister->Finalize(fpRelease);
}