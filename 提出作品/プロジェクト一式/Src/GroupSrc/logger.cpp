//============================================================================
// 
// ���K�[ [logger.cpp]
// Author : ���c����
// 
//============================================================================

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "logger.h"

//============================================================================
// 
// public�����o
// 
//============================================================================

//============================================================================
// �������O�ǉ� - �@
//============================================================================
void CLogger::AddSynchronization(const char* ptr)
{
	std::unique_ptr<LOG_SYNCHRONIZATION> upSync = std::make_unique<LOG_SYNCHRONIZATION>(ptr);
	m_vupLogData.push_back(std::move(upSync));
}

//============================================================================
// �������O�ǉ� - �A
//============================================================================
void CLogger::AddSynchronization(const std::string& str)
{
	std::unique_ptr<LOG_SYNCHRONIZATION> upSync = std::make_unique<LOG_SYNCHRONIZATION>(str);
	m_vupLogData.push_back(std::move(upSync));
}

//============================================================================
// �������O�ǉ� - �@
//============================================================================
void CLogger::AddDuration(const char* ptr, int duration)
{
	std::unique_ptr<LOG_DURATION> upSync = std::make_unique<LOG_DURATION>(ptr, duration * 60);
	m_vupLogData.push_back(std::move(upSync));
}

//============================================================================
// �������O�ǉ� - �A
//============================================================================
void CLogger::AddDuration(const std::string& str, int duration)
{
	std::unique_ptr<LOG_DURATION> upSync = std::make_unique<LOG_DURATION>(str, duration * 60);
	m_vupLogData.push_back(std::move(upSync));
}

//============================================================================
// �i�����O�ǉ� - �@
//============================================================================
void CLogger::AddPersistence(const char* ptr)
{
	const std::string& str = ptr;
	m_vLogPersistence.push_back(str);
}

//============================================================================
// �i�����O�ǉ� - �A
//============================================================================
void CLogger::AddPersistence(const std::string& str)
{
	m_vLogPersistence.push_back(str);
}

//============================================================================
// �x�����O�ǉ� - �@
//============================================================================
void CLogger::AddWarning(const char* ptr)
{
	m_LogWarning = ptr;
}

//============================================================================
// �x�����O�ǉ� - �A
//============================================================================
void CLogger::AddWarning(const std::string& str)
{
	m_LogWarning = str;
}

//============================================================================
// �v���J�n
//============================================================================
void CLogger::StartFreqCount()
{
	if (m_bFreqCount)
	{
		return;
	}

	m_bFreqCount = true;

	QueryPerformanceCounter(&m_StartFreq);
}

//============================================================================
// �v����~
//============================================================================
void CLogger::StopFreqCount()
{
	if (!m_bFreqCount)
	{
		return;
	}

	m_bFreqCount = false;

	QueryPerformanceCounter(&m_StopFreq);

	// �o�߂������Ԃ�����o��
	double dTime = static_cast<double>(m_StopFreq.QuadPart - m_StartFreq.QuadPart) / m_FreqBase.QuadPart;

	// �i�����O�ɒǉ�
	AddPersistence(std::to_string(dTime) + " : [s]");

	// �v���̊J�n�_�E�I���̏���������
	m_StartFreq = {};
	m_StopFreq = {};
}

//============================================================================
// ���b�v
//============================================================================
void CLogger::LapFreqCount()
{
	if (!m_bFreqCount)
	{
		return;
	}

	StopFreqCount();
	StartFreqCount();
}

//============================================================================
// ���O�o��
//============================================================================
void CLogger::DisplayLog()
{
	/* �T�C�Y�̃t���O�ɉ����ĕϓ� (�G�Ȏ���) */
	ImVec2 Size = { 0, 0 };
	!m_bSwitchSize ? Size = { 200, 400 } : Size = { 500, 400 };

	// �E�B���h�E�̃T�C�Y�ƈʒu��ݒ�
	ImGui::SetNextWindowSize(Size);
	ImGui::SetNextWindowPos({ 0, 0 }, ImGuiCond_FirstUseEver);

	if (ImGui::Begin("Debug Window", nullptr, ImGuiWindowFlags_MenuBar))
	{
		// ���j���[�o�[�̑���
		ControlMenuBar();

		// �^�u�`���ł̃��O�o��
		WindowStyleTab();

		// �E�B���h�E�̕\���I��
		ImGui::End();
	}
}

//============================================================================
// 
// private�����o
// 
//============================================================================

//============================================================================
// LOG_SYNCHRONIZATION�\���̂̃R���X�g���N�^ - �@
//============================================================================
CLogger::LOG_SYNCHRONIZATION::LOG_SYNCHRONIZATION(const char* ptr)
{
	this->m_Log = ptr;
}

//============================================================================
// LOG_SYNCHRONIZATION�\���̂̃R���X�g���N�^ - �A
//============================================================================
CLogger::LOG_SYNCHRONIZATION::LOG_SYNCHRONIZATION(const std::string& str)
{
	this->m_Log = str;
}

//============================================================================
// LOG_SYNCHRONIZATION�\���̂̃��O�o��
//============================================================================
bool CLogger::LOG_SYNCHRONIZATION::Log()
{
	ImGui::Text(m_Log.c_str());

	// Synchronization�ȃ��O�͑����������邽�ߕK��true��Ԃ�
	return true;
}

//============================================================================
// LOG_DURATION�\���̂̃R���X�g���N�^ - �@
//============================================================================
CLogger::LOG_DURATION::LOG_DURATION(const char* ptr, int duration) :
	m_nDuration(duration)
{
	this->m_Log = ptr;
}

//============================================================================
// LOG_DURATION�\���̂̃R���X�g���N�^ - �A
//============================================================================
CLogger::LOG_DURATION::LOG_DURATION(const std::string& str , int duration) :
	m_nDuration(duration)
{
	this->m_Log = str;
}

//============================================================================
// LOG_DURATION�\���̂̃��O�o��
//============================================================================
bool CLogger::LOG_DURATION::Log()
{
	ImGui::TextColored({ 255, 255, 0, 255 }, m_Log.c_str());

	// �o�͊��Ԃ��f�N�������g
	--m_nDuration;

	// Duration�ȃ��O�͊��Ԃ�0�ɂȂ�Ə������邽��true���A�o�݂͂̂̏ꍇ��false��Ԃ�
	return m_nDuration <= 0 ? true : false;
}

//============================================================================
// �f�t�H���g�R���X�g���N�^
//============================================================================
CLogger::CLogger() :
	m_vupLogData{},
	m_vLogPersistence{},
	m_LogWarning{},
	m_bSwitchSize(false),
	m_bAutoScroll(false),
	m_bFreqCount(false),
	m_StartFreq{},
	m_StopFreq{},
	m_FreqBase{}
{}

//============================================================================
// �f�X�g���N�^
//============================================================================
CLogger::~CLogger()
{
	// �I������
	Finalize();
}

//============================================================================
// �����ݒ�
//============================================================================
bool CLogger::Initialize()
{
	// �R���e�i�N���X�̃N���A
	m_vupLogData.clear();		// ���O�W�σf�[�^
	m_vLogPersistence.clear();	// �i�����O�W�σf�[�^
	m_LogWarning.clear();		// �x�����O

	// 1�b������̎��g�����擾
	QueryPerformanceFrequency(&m_FreqBase);

	return true;
}

//============================================================================
// �I������
//============================================================================
void CLogger::Finalize()
{
	// using�f�B���N�e�B�u
	using std::to_string;

	// �V�X�e���������擾
	SYSTEMTIME Time = {};
	GetLocalTime(&Time);

	// �����o�����O�p�̃p�X���쐬
	std::string Path = RELATIVE_PATH + to_string(Time.wYear) + "_" + to_string(Time.wMonth) + "_" + to_string(Time.wDay) + " " + to_string(Time.wHour) + "-" + to_string(Time.wMinute) + "-" + to_string(Time.wSecond) + EXTENSION;

	// �t�@�C���������o���W�J
	std::ofstream Ofs(Path, std::ios::out);

	// �����o���p�W�F�C�\���I�u�W�F�N�g���쐬
	JSON Export;

	if (Ofs.good())
	{
		unsigned short int wIdx = 0;

		for (const auto& it : m_vLogPersistence)
		{
			Export["Log"][wIdx] = it.c_str();
			++wIdx;
		}

		// �W�F�C�\���f�[�^���V���A���C�Y
		Ofs << Export.dump(1, '	');	// ������ -> �C���f���g��, ������ -> �C���f���g�`��
	}
}

//============================================================================
// ���j���[�o�[�̑���
//============================================================================
void CLogger::ControlMenuBar()
{
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu(u8"�T�C�Y"))
		{
			if (ImGui::MenuItem(u8"��", nullptr, !m_bSwitchSize))
			{
				m_bSwitchSize = false;
			}

			if (ImGui::MenuItem(u8"��", nullptr, m_bSwitchSize))
			{
				m_bSwitchSize = true;
			}

			ImGui::EndMenu();
		}
		ImGui::Separator();

		if (ImGui::BeginMenu(u8"���O"))
		{
			if (ImGui::MenuItem(u8"�ŐV���O�ւ̎����X�N���[��", nullptr, m_bAutoScroll))
			{
				m_bAutoScroll = !m_bAutoScroll;
			}

			if (ImGui::MenuItem(u8"�i�����O�����Z�b�g"))
			{
				m_vLogPersistence.clear();			
			}

			ImGui::EndMenu();
		}
		ImGui::Separator();

		ImGui::EndMenuBar();
	}
}

//============================================================================
// �E�B���h�E�`�� - �^�u
//============================================================================
void CLogger::WindowStyleTab()
{
	if (ImGui::BeginTabBar("StyleTab - Tab Bar"))
	{
		if (ImGui::BeginTabItem(u8"���݂̃��O"))
		{
			if (ImGui::BeginChild("StyleTab - Tab Bar - ChildA", { 0, 300 }, false, ImGuiWindowFlags_HorizontalScrollbar))
			{
				// ���s���O�o��
				DisplayLogCurrent();

				// �㉺�����X�N���[��
				if (m_bAutoScroll)
				{
					ImGui::SetScrollHereY(1.0f);
				}
			}
			ImGui::EndChild();	// https://github.com/ocornut/imgui/issues/6683

			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem(u8"�ߋ��̃��O"))
		{
			if (ImGui::BeginChild("StyleTab - Tab Bar - ChildB", { 0, 300 }, false, ImGuiWindowFlags_HorizontalScrollbar))
			{
				// �i�����O�o��
				DisplayLogPersistence();

				// �㉺�����X�N���[��
				if (m_bAutoScroll)
				{
					ImGui::SetScrollHereY(1.0f);
				}
			}
			ImGui::EndChild();	// https://github.com/ocornut/imgui/issues/6683

			ImGui::EndTabItem();
		}

		ImGui::EndTabBar();
	}
}

//============================================================================
// ���s���O�o��
//============================================================================
bool CLogger::DisplayLogCurrent()
{
	// Current�ȃ��O��1���������false��Ԃ�
	if (m_vupLogData.empty())
	{
		return false;
	}

	// �e�[�u���̕\����
	float fWidth = 0.0f;

	// ��s���đS�e�L�X�g���̍ő�T�C�Y���擾
	for (const auto& it : m_vupLogData)
	{
		fWidth = max(fWidth, ImGui::CalcTextSize(it->m_Log.c_str()).x);
	}

	// �q�E�B���h�E���̃e�[�u����K�v�ɉ��������X�N���[�������邽�߁A�����̎w����ő�e�L�X�g�T�C�Y�ɂ���
	if (ImGui::BeginTable("Current Table", 1, ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders, { fWidth + 10, 0 }))
	{
		// �W�σf�[�^�̑S�ẴC���X�^���X�Ń��O�o�͂��s��
		for (auto it = m_vupLogData.begin(); it != m_vupLogData.end(); )
		{
			ImGui::TableNextRow();
			ImGui::TableSetColumnIndex(0);

			// ���O�o�͂ƂƂ��ɁA�����t���O���󂯎��
			if (it->get()->Log())
			{
				// ���̃C���X�^���X���폜���A���̗v�f�̃C�e���[�^���󂯎��
				it = m_vupLogData.erase(it);

				// �C�e���[�^���C���N�������g�����ɑ��s
				continue;
			}

			// �������s��Ȃ������ꍇ�A���̂܂܃C�e���[�^���C���N�������g
			++it;
		}

		ImGui::EndTable();
	}

	return true;
}

//============================================================================
// �i�����O�o��
//============================================================================
bool CLogger::DisplayLogPersistence()
{
	// Persistence�ȃ��O��1���������false��Ԃ�
	if (m_vLogPersistence.empty())
	{
		return false;
	}

	// �e�[�u���̕\����
	float fWidth = 0.0f;

	// ��s���đS�e�L�X�g���̍ő�T�C�Y���擾
	for (const auto& it : m_vLogPersistence)
	{
		fWidth = max(fWidth, ImGui::CalcTextSize(it.c_str()).x);
	}

	// �q�E�B���h�E���̃e�[�u����K�v�ɉ��������X�N���[�������邽�߁A�����̎w����ő�e�L�X�g�T�C�Y�ɂ���
	if (ImGui::BeginTable("Persistence Table", 1, ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders, { fWidth + 10, 0 }))
	{
		//// �e�L�X�g�t�B���^�[���쐬
		//static ImGuiTextFilter Filter;
		//Filter.Draw();

		// �������s��Ȃ����߁A�S�Ă̗v�f�����O�o�͂���
		for (const auto& it : m_vLogPersistence)
		{
			ImGui::TableNextRow();
			ImGui::TableSetColumnIndex(0);

			// ���̗v�f�e�L�X�g���R�s�[
			/* �R�s�[���Ȃ��Ă�string�^�̃R���e�i�����C�e���[�^�g�� */
			/* �Ȃ�string�^�̃C�e���[�^���ăG�������g�w��o���Ȃ��c�H */
			//const char* Lines[] = { it.c_str() };

			//for (unsigned short wCntLoop = 0; wCntLoop < it.size(); ++wCntLoop)
			//{
			//	if (Filter.PassFilter(Lines[wCntLoop]))
			//	{
					//ImGui::TextColored({ 0, 255, 255, 255 }, "%s", Lines[wCntLoop]);
			//	}
			//}
			ImGui::TextColored({ 0, 255, 255, 255 }, it.c_str());
		}

		ImGui::EndTable();
	}

	return true;
}

//============================================================================
// �x�����O�o��
//============================================================================
bool CLogger::DisplayLogWarning()
{
	// ���[�_���E�B���h�E�̏ꏊ�𒆉��ɌŒ�
	ImGui::SetNextWindowSize({ -1, -1 });
	ImGuiIO& io = ImGui::GetIO();
	ImVec2 center = ImVec2(io.DisplaySize.x * 0.5f, io.DisplaySize.y * 0.5f);
	ImGui::SetNextWindowPos(center, ImGuiCond_Always, { 0.5, 0.5 });

	ImGui::OpenPopup("Warning");

	if (ImGui::BeginPopupModal("Warning", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::TextColored({ 128, 0, 0, 255 }, m_LogWarning.c_str());

		if (ImGui::Button(u8"�m�F"))
		{
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}

	return false;
}