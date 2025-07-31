//============================================================================
// 
// �J�����}�l�[�W���[ [camera_manager.cpp]
// Author : ���c����
// 
//============================================================================

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "camera_manager.h"

// �f�o�C�X�擾�p
#include "renderer.h"

// ���O�o�͗p
#include "logger.h"

//****************************************************
// using�f�B���N�e�B�u
//****************************************************
using namespace useful;

//============================================================================
// ���s - ���
//============================================================================
bool CCamera_Manager::TRANSITION::Execute(CCamera_Manager*& cameraManager)
{
	throw std::runtime_error("'TRANSITION::Execute' was Failed : " + static_cast<std::string>(typeid(cameraManager).name()));
}

//============================================================================
// �ύX - ���
//============================================================================
void CCamera_Manager::TRANSITION::Change(TRANSITION*& r)
{
	throw std::runtime_error("'TRANSITION::Change' was Failed : " + static_cast<std::string>(typeid(r).name()));
}

//============================================================================
// �g�����W�V�����̍X�V����
//============================================================================
void CCamera_Manager::TRANSITION::UpdateTransition(CCamera_Manager*& cameraManager)
{
	// �g�����W�V�������̃C���X�^���X�̎Q��
	TRANSITION*& r = cameraManager->m_pTransition;

	if (!r)
	{
		return;
	}

	if (r->Execute(cameraManager))
	{
		r->Change(r);
	}
}

//============================================================================
// ���s - �C���g��
//============================================================================
bool CCamera_Manager::INTRO::Execute(CCamera_Manager*& cameraManager)
{
	// �}�l�[�W���[�̃��W�X�g���̃R���e�i���Q��
	std::map<std::string, CCamera*>& rResource = cameraManager->m_upRegistry->RefDynamicResource();

	// ���ݑI������Ă���J�����̃L�[���R�s�[
	const std::string& SelectKey = cameraManager->m_SelectKey;

	// ���݂̃L�[�ɑΉ�����C���X�^���X���ۗL����Ă��Ȃ�
	if (rResource.find(SelectKey) == rResource.end())
	{
		throw std::runtime_error("'SetTransition' was Failed at FindOld : " + SelectKey);
	}

	// �J�ڗ\��̃L�[�ɑΉ�����C���X�^���X���ۗL����Ă��Ȃ�
	if (rResource.find(m_NextKey) == rResource.end())
	{
		throw std::runtime_error("'SetTransition' was Failed at FindNew : " + m_NextKey);
	}

	// ���݂̃J�����ƁA�J�ڗ\��̃J�����̎Q��
	CCamera
		*& rOld = rResource.at(SelectKey),
		*& rNew = rResource.at(m_NextKey);

	// �@ - ���݂̃J�����̃p�����[�^��
	rOld->SetRotTarget(rNew->GetRot());
	rOld->SetPosTarget(rNew->GetPos());
	rOld->SetDistanceTarget(rNew->GetDistance());
	rOld->SetUpAdjustTarget(rNew->GetUpAdjust());

	// 臒l
	const float fThreshold = 0.1f;

	// ������x�p�����[�^���ڕW�l�ɔ������ꍇ��ԕύX
	if (rOld->GetRot().x - rNew->GetRot().x <= fThreshold)
	{
		return true;
	}

	return false;
}

//============================================================================
// �ύX - �C���g��
//============================================================================
void CCamera_Manager::INTRO::Change(TRANSITION*& r)
{
	if (r)
	{
		delete r;
		r = DBG_NEW OUTRO();
	}
}

//============================================================================
// ���s - �A�E�g��
//============================================================================
bool CCamera_Manager::OUTRO::Execute(CCamera_Manager*& cameraManager)
{
	// ��]
	RotateTargetYawAxis(cameraManager->RefSelectCamera(), 0.01f);

	return false;
}

//============================================================================
// �ύX - �A�E�g��
//============================================================================
void CCamera_Manager::OUTRO::Change(TRANSITION*& r)
{
	if (r)
	{
		delete r;
		r = nullptr;
	}
}

//============================================================================
// 
// public�����o
// 
//============================================================================

//============================================================================
// �X�V����
//============================================================================
void CCamera_Manager::Update()
{
	// �ۗL����S�J�����̍X�V����
	for (auto& it : m_upRegistry->RefDynamicResource())
	{
		it.second->Update();
	}

	// �J�����}�l�[�W���[�̃|�C���^
	CCamera_Manager* pCameraManager = this;

	// �g�����W�V�����̍X�V
	TRANSITION::UpdateTransition(pCameraManager);
}

//============================================================================
// �g�����W�V�����̃Z�b�g
//============================================================================
void CCamera_Manager::SetTransition(const std::string& key)
{
#if 0
	// ���łɃg�����W�V�������Ȃ�A��U�����G�ɏ�Ԃ�V�K����
	if (m_pTransition)
	{
		//return;
		delete m_pTransition;
		m_pTransition = nullptr;
	}

	// �I�𒆂̃L�[��n���C���g���𐶐�
	m_pTransition = DBG_NEW INTRO(key);
#else
	// ���W�X�g���̃R���e�i���Q��
	std::map<std::string, CCamera*>& rResource = m_upRegistry->RefDynamicResource();

	// ���݂̃L�[�ɑΉ�����C���X�^���X���ۗL����Ă��Ȃ�
	if (rResource.find(m_SelectKey) == rResource.end())
	{
		throw std::runtime_error("'SetTransition' was Failed at FindOld : " + m_SelectKey);
	}

	// �J�ڗ\��̃L�[�ɑΉ�����C���X�^���X���ۗL����Ă��Ȃ�
	if (rResource.find(key) == rResource.end())
	{
		throw std::runtime_error("'SetTransition' was Failed at FindNew : " + key);
	}

	// ���݂̃J�����ƁA�J�ڗ\��̃J�����̎Q��
	CCamera
		*& rOld = rResource.at(m_SelectKey),
		*& rNew = rResource.at(key);

	// �J�ڗ\��̃J�����̏����l���A���݂̃p�����[�^�ɂ���
	rNew->SetRot(rOld->GetRot());
	rNew->SetPos(rOld->GetPos());
	rNew->SetDistance(rOld->GetDistance());
	rNew->SetUpAdjust(rOld->GetUpAdjust());

	// �I�����Ă���J�����̃L�[��J�ڗ\��̂��̂ɕύX
	m_SelectKey = key;
#endif
}

//============================================================================
// 
// private�����o
// 
//============================================================================

//============================================================================
// �f�t�H���g�R���X�g���N�^
//============================================================================
CCamera_Manager::CCamera_Manager() :
	m_upRegistry(std::make_unique<CRegistry<CCamera*>>()),
	m_SelectKey{},
	m_pTransition(nullptr)
{}

//============================================================================
// �f�X�g���N�^
//============================================================================
CCamera_Manager::~CCamera_Manager()
{
	// �I������
	Finalize();
}

//============================================================================
// ����������
//============================================================================
bool CCamera_Manager::Initialize()
{
	// ���������p�̃����_�I�u�W�F�N�g�쐬
	std::function<CCamera*(const std::string&)> fpCreate =
		[](const std::string& path) -> CCamera*
	{
		// �K�v�Ȓǉ������擾
		const JSON& Json = hws::SafeOpenJsonFile(path);

		// �J�����̃C���X�^���X�𐶐�
		CCamera* pElement = DBG_NEW CCamera(Json);
		
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
// �I������
//============================================================================
void CCamera_Manager::Finalize()
{
	// ��������p�̃����_�I�u�W�F�N�g�쐬
	std::function<void(CCamera*&)> fpRelease =
		[](CCamera*& element) noexcept -> void
	{
		// �I�����������A���\�[�X�̉���̂�
		if (element)
		{
			delete element;
			element = nullptr;
		}
	};

	// ���W�X�g���̏I������
	m_upRegistry->Finalize(fpRelease);

	// �g�����W�V�������̉��
	if (m_pTransition)
	{
		delete m_pTransition;
		m_pTransition = nullptr;
	}
}