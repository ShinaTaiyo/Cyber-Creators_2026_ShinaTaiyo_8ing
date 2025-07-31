//============================================================================
// 
// �x�W�F�Ȑ��}�l�[�W���[ [bezier_manager.cpp]
// Author : ���c����
// 
//============================================================================

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "bezier_manager.h"

// �f�o�C�X�擾�p
#include "renderer.h"

//****************************************************
// �ÓI�����o�ϐ��̏�����
//****************************************************
std::string CBezier_Manager::RELOAD_PATH = "";

//============================================================================
// 
// public�����o
// 
//============================================================================

//============================================================================
// ���}���u
//============================================================================
void CBezier_Manager::ExplicitInitialize()
{
	// ���������p�̃����_�I�u�W�F�N�g�쐬
	std::function<std::unique_ptr<CQuadratic_Bezier>(const std::string&)> fpCreate =
		[](const std::string& path) -> std::unique_ptr<CQuadratic_Bezier>
	{
		// �x�W�F�Ȑ��̃C���X�^���X�𐶐�
		std::unique_ptr<CQuadratic_Bezier> upElement = make_unique<CQuadratic_Bezier>();

		// �������s
		if (!upElement)
		{
			return nullptr;
		}

		// ����������
		if (!upElement->Initialize())
		{
			return nullptr;
		}

		// �����p�̒ǉ��p�����[�^��W�J
		const JSON& Json = hws::SafeOpenJsonFile(path);

		// ����_�̍��W��ݒ�
		for (unsigned short wIdx = 0; wIdx < CQuadratic_Bezier::NUM_CONTROL; ++wIdx)
		{
			upElement->SetPosToControlPointForSelectedIdx(hws::JsonConvertToVec3(Json["Pos"][wIdx]), wIdx);
		}

		// �i�s���x���Z�b�g
		upElement->SetSpeed(static_cast<float>(Json["Speed"]));

		// ���]�g�p�t���O�̏�Ԃ��Z�b�g
		upElement->SetUseReverse(static_cast<bool>(Json["UseReverse"]));

		// �C���X�^���X�̃|�C���^��Ԃ�
		return upElement;
	};

	// �ēǂݍ��݃p�X���ݒ肳��Ă��Ȃ���Ύ����I�ɏ����f�[�^��ǂݍ���
	if (RELOAD_PATH.empty())
	{
		RELOAD_PATH = INITIALIZE_PATH;
	}

	// ���W�X�g���̏���������
	m_upRegistry->Initialize(RELOAD_PATH, std::move(fpCreate));

	// �x�W�F�Ȑ��C���X�^���X�̍ő吔��ێ�
	m_nMaxSizeRegistry = static_cast<int>(m_upRegistry->RefDynamicResource().size());

	return;
}

//============================================================================
// �X�V����
//============================================================================
bool CBezier_Manager::Update()
{
	// ���ݍX�V���̃x�W�F�Ȑ����������邽�߂̃J�E���g
	int nCntForCurrentIdx = 0;

	// �C���X�^���X���������ꂽ���Ԃ�1���X�V���s��
	for (auto& it : m_upRegistry->RefDynamicResource())
	{
		// ���݂̃J�E���g�����A�X�V�ΏۂƂȂ�C���f�b�N�X�łȂ����
		if (m_nIdxCurrentBeizier != nCntForCurrentIdx)
		{
			// �J�E���g�̃C���N�������g�݂̂ōă��[�v
			++nCntForCurrentIdx;
			
			continue;
		}

		// �ړ��_���I�[�ɓ��B����܂ŁA�Ώۂ̃C���f�b�N�X�̗v�f�̂ݍX�V
		if (it.second->Update())
		{
			// �ړ��_���I�[�ɓ��B�����Ƃ��A�C���f�b�N�X���C���N�������g
			++m_nIdxCurrentBeizier;
		}

		/* �X�V���̍��W */
		m_CurrentPos = it.second->GetPosTracePointOnTrajectoryLine();

		// �C���f�b�N�X�����v�f�̍ő吔���z���Ă��܂��ꍇ
		if (m_nIdxCurrentBeizier >= m_nMaxSizeRegistry)
		{
			// ���o�[�X�t���O���s�g�p�ł���Ȃ�X�V�����̒ʒm��Ԃ�
			if (!it.second->GetUseReverse())
			{
				return true;
			}

			// �X�V���Ԃ��ŏ��ɖ߂�
			m_nIdxCurrentBeizier = 0;
		}
		
		// ����ȏ�̌����𒆒f
		break;
	}

	return false;
}

//============================================================================
// �`�揈��
//============================================================================
void CBezier_Manager::Draw()
{
	// �S�C���X�^���X�̕`�揈��
	for (auto& it : m_upRegistry->RefDynamicResource())
	{
		it.second->Draw();
	}
}

//============================================================================
// 
// private�����o
// 
//============================================================================

//============================================================================
// �f�t�H���g�R���X�g���N�^
//============================================================================
CBezier_Manager::CBezier_Manager() :
	m_upRegistry(std::make_unique<CRegistry<std::unique_ptr<CQuadratic_Bezier>>>()),
	m_nIdxCurrentBeizier(0),
	m_nMaxSizeRegistry(0),
	m_CurrentPos{ 0.0, 0.0, 0.0 }
{}

//============================================================================
// �f�X�g���N�^
//============================================================================
CBezier_Manager::~CBezier_Manager()
{
	// �I������
	Finalize();
}

//============================================================================
// ����������
//============================================================================
bool CBezier_Manager::Initialize()
{
	// ���������p�̃����_�I�u�W�F�N�g�쐬
	std::function<std::unique_ptr<CQuadratic_Bezier>(const std::string&)> fpCreate = 
		[](const std::string& path) -> std::unique_ptr<CQuadratic_Bezier>
	{
		// �x�W�F�Ȑ��̃C���X�^���X�𐶐�
		std::unique_ptr<CQuadratic_Bezier> upElement = make_unique<CQuadratic_Bezier>();

		// �������s
		if (!upElement)
		{
			return nullptr;
		}

		// ����������
		if (!upElement->Initialize())
		{
			return nullptr;
		}

		// �����p�̒ǉ��p�����[�^��W�J
		const JSON& Json = hws::SafeOpenJsonFile(path);

		// ����_�̍��W��ݒ�
		for (unsigned short wIdx = 0; wIdx < CQuadratic_Bezier::NUM_CONTROL; ++wIdx)
		{
			upElement->SetPosToControlPointForSelectedIdx(hws::JsonConvertToVec3(Json["Pos"][wIdx]), wIdx);
		}

		// �i�s���x���Z�b�g
		upElement->SetSpeed(static_cast<float>(Json["Speed"]));

		// ���]�g�p�t���O�̏�Ԃ��Z�b�g
		upElement->SetUseReverse(static_cast<bool>(Json["UseReverse"]));

		// �C���X�^���X�̃|�C���^��Ԃ�
		return upElement;
	};

	// �ēǂݍ��݃p�X���ݒ肳��Ă��Ȃ���Ύ����I�ɏ����f�[�^��ǂݍ���
	if (RELOAD_PATH.empty())
	{
		RELOAD_PATH = INITIALIZE_PATH;
	}

	// ���W�X�g���̏���������
	if (!m_upRegistry->Initialize(RELOAD_PATH, std::move(fpCreate)))
	{
		return false;
	}

	// �x�W�F�Ȑ��C���X�^���X�̍ő吔��ێ�
	m_nMaxSizeRegistry = static_cast<int>(m_upRegistry->RefDynamicResource().size());

	return true;
}

//============================================================================
// �I������
//============================================================================
void CBezier_Manager::Finalize()
{
	// ��������p�̃����_�I�u�W�F�N�g�쐬
	std::function<void(std::unique_ptr<CQuadratic_Bezier>&)> fpRelease =
		[](std::unique_ptr<CQuadratic_Bezier>& element) noexcept -> void
	{
		// �I����������A�������
		if (element)
		{
			element->Finalize();
			element.reset();
			element = nullptr;
		}
	};

	// ���W�X�g���̏I������
	m_upRegistry->Finalize(fpRelease);
}