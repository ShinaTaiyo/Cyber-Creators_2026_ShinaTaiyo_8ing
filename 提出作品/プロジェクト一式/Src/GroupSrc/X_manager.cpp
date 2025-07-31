//============================================================================
// 
// X���f���}�l�[�W���[ [X_manager.cpp]
// Author : ���c����
// 
//============================================================================

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "X_manager.h"

// �f�o�C�X�擾�p
#include "renderer.h"

//****************************************************
// using�f�B���N�e�B�u
//****************************************************
using namespace useful;

//============================================================================
// 
// private�����o
// 
//============================================================================

//============================================================================
// �f�t�H���g�R���X�g���N�^
//============================================================================
CX_Manager::CX_Manager() :
	m_upRegistry(std::make_unique<CRegistry<MODEL*>>()),
	m_mCache{}
{}

//============================================================================
// �f�X�g���N�^
//============================================================================
CX_Manager::~CX_Manager()
{
	// �I������
	Finalize();
}

//============================================================================
// ����������
//============================================================================
bool CX_Manager::Initialize()
{
	// ���������p�̃����_�I�u�W�F�N�g�쐬
	std::function<MODEL*(const std::string&)> fpCreate =
		[this](const std::string& path) -> MODEL*
	{
		// X���f���̃C���X�^���X�𐶐�
		MODEL* pElement = DBG_NEW MODEL();

		// �f�o�C�X���擾
		auto pDev = CRenderer::GetDeviece();

		// X���f���̐���
		HRESULT hr = D3DXLoadMeshFromX(
			path.c_str(),
			D3DXMESH_SYSTEMMEM,
			pDev,
			nullptr,
			&pElement->pBuffMat,
			nullptr,
			&pElement->dwNumMat,
			&pElement->pMesh);

		// �������s
		if (FAILED(hr))
		{
			throw std::runtime_error("Failed to Import X Model : " + path);
		}

		// ���f���̃T�C�Y���擾����
		pElement->Size = LoadSize(path, pElement->VtxMax, pElement->VtxMin);

		// �}�e���A���f�[�^�ւ̃|�C���^���擾
		D3DXMATERIAL* pMat = (D3DXMATERIAL*)pElement->pBuffMat->GetBufferPointer();

		// �}�e���A�������A�}�e���A���F���̃|�C���^�z����m��
		pElement->apColMat = DBG_NEW D3DXCOLOR[pElement->dwNumMat];

		// �}�e���A�������A�e�N�X�`�����̃|�C���^�z����m��
		pElement->apTex = DBG_NEW LPDIRECT3DTEXTURE9[pElement->dwNumMat];

		// �}�e���A�����ƂɃe�N�X�`���̗L�����m�F
		for (WORD wCntMat = 0; wCntMat < pElement->dwNumMat; ++wCntMat)
		{
			// �}�e���A���̐F�����f�������ɕۑ����Ă���
			pElement->apColMat[wCntMat] = pMat[wCntMat].MatD3D.Diffuse;

			// �e�N�X�`�������擾���s
			if (pMat[wCntMat].pTextureFilename == nullptr)
			{
				// �e�N�X�`���̃|�C���^�����������Ă���
				pElement->apTex[wCntMat] = nullptr;

				continue;
			}

			/* �e�N�X�`���̐����O�ɁA�L���b�V�����ɓ���̂��̂��Ȃ����`�F�b�N */
			auto it = m_mCache.find(pMat[wCntMat].pTextureFilename);

			/* ���݂���Ύg���܂킷 */
			if (it != m_mCache.end())
			{
				pElement->apTex[wCntMat] = it->second;
			}
			else
			{
				/* �����ꍇ�̂ݐV�K���� */
				hr = D3DXCreateTextureFromFileA(
					pDev,
					pMat[wCntMat].pTextureFilename,
					&pElement->apTex[wCntMat]);

				/* ���̃e�N�X�`���̏����L���b�V���ɒǉ� */
				m_mCache[pMat[wCntMat].pTextureFilename] = pElement->apTex[wCntMat];
			}

			// �e�N�X�`���������s
			if (FAILED(hr))
			{
				// �G���[�n���h�����O
				switch (hr)
				{
				case D3DERR_INVALIDCALL:
					MessageBoxA(nullptr, "�����ȌĂяo���i�������s���Ȃǁj", "�G���[", MB_OK);
					break;
				case D3DERR_OUTOFVIDEOMEMORY:
					MessageBoxA(nullptr, "�r�f�I�������s��", "�G���[", MB_OK);
					break;
				case E_OUTOFMEMORY:
					MessageBoxA(nullptr, "�������s��", "�G���[", MB_OK);
					break;
				default:
					MessageBoxA(nullptr, "�e�N�X�`���ǂݍ��ݎ��s", "�G���[", MB_OK);
					break;
				}

				throw std::runtime_error("Failed to Create X Model - Texture");
			}
		}

		// �C���X�^���X�̃|�C���^��Ԃ�
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
// �T�C�Y�ǂݍ���
//============================================================================
D3DXVECTOR3 CX_Manager::LoadSize(const std::string& path, D3DXVECTOR3& VtxMax, D3DXVECTOR3& VtxMin)
{
	// �Y�����f���t�@�C����W�J
	std::ifstream ModelFile(path);

	// �W�J�Ɏ��s
	if (!ModelFile.good())
	{
		return VEC3_INIT;
	}

	// �ėp������i�[�p
	std::string Str;

	// �e�L�X�g���璸�_���̍s������
	while (std::getline(ModelFile, Str))
	{
		// ���_���𔭌������̂Ō����I��
		if (Str == "Mesh {")
		{
			// �s�v�ȍs����x�ǂݍ���
			std::getline(ModelFile, Str);

			break;
		}
	}

	// ��r�����p�ɐ��l�����Ă���
	Vec3 ResultMin = { FLT_MAX, FLT_MAX, FLT_MAX };
	Vec3 ResultMax = { FLT_MIN, FLT_MIN, FLT_MIN };

	// �擾�\��̎�������
	const WORD wAxis = 3;

	// ���l���r���Ă���
	while (std::getline(ModelFile, Str))
	{
		// �I������
		if (Str == " ")
		{
			break;
		}

		// ���l�i�[�p
		float fNum[wAxis] = { 0.0f, 0.0f, 0.0f };

		// �ǂ݂Ƃ������l����U�i�[����
		for (WORD wCntAxis = 0; wCntAxis < wAxis; ++wCntAxis)
		{
			fNum[wCntAxis] = std::stof(Str);
			Str = Str.substr(Str.find(";") + 1, Str.back());
		}

		// �e���̍ő�E�ŏ��n�_���X�V
		if (ResultMin.x > fNum[0])
		{
			ResultMin.x = fNum[0];
		}

		if (ResultMin.y > fNum[1])
		{
			ResultMin.y = fNum[1];
		}

		if (ResultMin.z > fNum[2])
		{
			ResultMin.z = fNum[2];
		}

		if (ResultMax.x < fNum[0])
		{
			ResultMax.x = fNum[0];
		}

		if (ResultMax.y < fNum[1])
		{
			ResultMax.y = fNum[1];
		}

		if (ResultMax.z < fNum[2])
		{
			ResultMax.z = fNum[2];
		}
	}

	// �t�@�C�������
	ModelFile.close();

	VtxMax = ResultMax; // �ő咸�_
	VtxMin = ResultMin; // �ŏ����_
	
	return ResultMax;
}

//============================================================================
// �I������
//============================================================================
void CX_Manager::Finalize()
{
	// ��������p�̃����_�I�u�W�F�N�g�쐬
	std::function<void(MODEL*&)> fpRelease =
		[](MODEL*& element) -> void
	{
		// �}�e���A���p�̃e�N�X�`����j��
		for (WORD wCntMat = 0; wCntMat < element->dwNumMat; ++wCntMat)
		{
			if (element->apTex[wCntMat] != nullptr)
			{
				element->apTex[wCntMat]->Release();
				element->apTex[wCntMat] = nullptr;
			}
		}

		// �e�N�X�`���|�C���^�z��̉��
		if (element->apTex != nullptr)
		{
			delete[] element->apTex;
			element->apTex = nullptr;
		}

		// �}�e���A���F�|�C���^�z��̔j��
		if (element->apColMat != nullptr)
		{
			delete[] element->apColMat;
			element->apColMat = nullptr;
		}

		// �}�e���A���o�b�t�@�̔j��
		if (element->pBuffMat != nullptr)
		{
			element->pBuffMat->Release();
			element->pBuffMat = nullptr;
		}

		// ���b�V���o�b�t�@�̔j��
		if (element->pMesh != nullptr)
		{
			element->pMesh->Release();
			element->pMesh = nullptr;
		}

		// �C���X�^���X�����
		if (element)
		{
			delete element;
			element = nullptr;
		}
	};

	// ���W�X�g���̏I������
	m_upRegistry->Finalize(fpRelease);
}