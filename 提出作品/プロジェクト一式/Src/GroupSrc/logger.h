//============================================================================
// 
// ���K�[�A�w�b�_�t�@�C�� [logger.h]
// Author : ���c����
// 
//============================================================================

// �C���N���[�h�K�[�h
#pragma once

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "singleton.hpp"

//****************************************************
// ���K�[�N���X�̒�`
//****************************************************
class CLogger final : public CSingleton<CLogger>
{
	//****************************************************
	// �t�����h�錾
	//****************************************************
	friend struct std::default_delete<CLogger>;
	friend CLogger* CSingleton<CLogger>::GetInstance();

public:

	//****************************************************
	// function
	//****************************************************

	/**
	 * ���t���[���X�V & �o�́A������������郍�O��ǉ����܂�
	 * 
	 * ptr, str - �o�͂������e�L�X�g
	 */
	void AddSynchronization(const char* ptr);			// - �@
	void AddSynchronization(const std::string& str);	// - �A

	/**
	 * �w����Ԃ̊Ԃ̂ݏo�͂���郍�O��ǉ����܂�
	 *
	 * ptr, str - �o�͂������e�L�X�g
	 * duration - �o�͊��ԁA�b���Ŏw�肵�Ă�������
	 */
	void AddDuration(const char* ptr, int duration);		// - �@
	void AddDuration(const std::string& str, int duration);	// - �A

	/**
	 * �v���Z�X�̏I���܂ŕێ�����郍�O��ǉ����܂�
	 * 
	 * ptr, str - �o�͂������e�L�X�g
	 */
	void AddPersistence(const char* ptr);			// - �@
	void AddPersistence(const std::string& str);	// - �A
	
	/**
	 * ���[�_���E�B���h�E�ɂ�菈�����~������x���p���O��ǉ����܂�
	 * 
	 * ptr, str - �o�͂������e�L�X�g
	 */
	[[ deprecated ]] void AddWarning(const char* ptr);			// - �@
	[[ deprecated ]] void AddWarning(const std::string& str);	// - �A

	/**
	 * ���g���̌v�����J�n���܂�
	 */
	void StartFreqCount();

	/**
	 * ���g���̌v�����~���܂�
	 */
	void StopFreqCount();

	/**
	 * �v�������b�v���܂�
	 */
	void LapFreqCount();

	/**
	 * �W�ς����f�[�^���o�͂��܂��A���̊֐��͍X�V���Ŗ��t���[���Ăяo���܂�
	 */
	void DisplayLog();

private:

	//****************************************************
	// �ÓI�����o�萔�̒�` (����J)
	//****************************************************

	// JSON�I�u�W�F�N�g���G�N�X�|�[�g���邽�߂̃p�X
	static constexpr const char* RELATIVE_PATH = "Data\\JSON\\Debug\\Persistence_Log ";
	static constexpr const char* EXTENSION = ".json";

	//****************************************************
	// LOG_BASE�\���̂��`
	//****************************************************
	struct LOG_BASE
	{
		// <special function>
		LOG_BASE() = default;			// �f�t�H���g�R���X�g���N�^
		virtual ~LOG_BASE() = default;	// �f�X�g���N�^

		// <function>
		virtual bool Log() = 0;	// ���O�o��

		// <data>
		std::string m_Log;	// ���O
	};

	//****************************************************
	// LOG_SYNCHRONIZATION�\���̂��`
	//****************************************************
	struct LOG_SYNCHRONIZATION final : public LOG_BASE
	{
		// <special function>
		LOG_SYNCHRONIZATION(const char* ptr);			// �R���X�g���N�^ - �@
		LOG_SYNCHRONIZATION(const std::string& str);	// �R���X�g���N�^ - �A
		~LOG_SYNCHRONIZATION() override = default;		// �f�X�g���N�^

		// <function>
		bool Log() override;	// ���O�o��
	};

	//****************************************************
	// LOG_DURATION�\���̂��`
	//****************************************************
	struct LOG_DURATION final : public LOG_BASE
	{
		// <special function>
		LOG_DURATION(const char* ptr, int duration);		// �R���X�g���N�^ - �@
		LOG_DURATION(const std::string& str, int duration);	// �R���X�g���N�^ - �A
		~LOG_DURATION() override = default;					// �f�X�g���N�^

		// <function>
		bool Log() override;	// ���O�o��

		// <data>
		int m_nDuration;	// �o�͊���
	};

	//****************************************************
	// special function
	//****************************************************
	CLogger();				// �f�t�H���g�R���X�g���N�^
	~CLogger() override;	// �f�X�g���N�^

	//****************************************************
	// function
	//****************************************************
	bool Initialize();				// ����������
	void Finalize();				// �I������
	void ControlMenuBar();			// ���j���[�o�[����
	void WindowStyleTab();			// �E�B���h�E�`�� - �^�u 
	bool DisplayLogCurrent();		// ���s���O�o��
	bool DisplayLogPersistence();	// �i�����O�o��
	bool DisplayLogWarning();		// �x�����O�o��

	//****************************************************
	// data
	//****************************************************
	std::vector<std::unique_ptr<LOG_BASE>> m_vupLogData;		// ���O�W�σf�[�^
	std::vector<std::string>			   m_vLogPersistence;	// �i�����O�W�σf�[�^
	std::string							   m_LogWarning;		// �x�����O

	bool m_bSwitchSize;	// �T�C�Y�؂�ւ�
	bool m_bAutoScroll;	// �����X�N���[���؂�ւ�
	bool m_bFreqCount;	// ���g���J�E���g

	LARGE_INTEGER m_StartFreq;	// �v���J�n�_
	LARGE_INTEGER m_StopFreq;	// �v���I���_
	LARGE_INTEGER m_FreqBase;	// �x�[�X���g��
};