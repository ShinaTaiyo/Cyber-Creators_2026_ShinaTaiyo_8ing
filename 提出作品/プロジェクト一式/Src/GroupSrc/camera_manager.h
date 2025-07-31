//============================================================================
// 
// �J�����}�l�[�W���[�A�w�b�_�t�@�C�� [camera_manager.h]
// Author : ���c����
// 
//============================================================================

// �C���N���[�h�K�[�h
#pragma once

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "singleton.hpp"

// �R���|�W�V����
#include "registry.hpp"
#include "camera.h"

//****************************************************
// �J�����}�l�[�W���[�N���X�̒�`
//****************************************************
class CCamera_Manager final : public CSingleton<CCamera_Manager>
{
	//****************************************************
	// �t�����h�錾
	//****************************************************
	friend struct std::default_delete<CCamera_Manager>;
	friend CCamera_Manager* CSingleton<CCamera_Manager>::GetInstance();

	//****************************************************
	// �^�G�C���A�X���`
	//****************************************************
	using upCamera = std::unique_ptr<CCamera>;

	//****************************************************
	// �ÓI�����o�ϐ��̒�`
	//****************************************************
	static constexpr const char* INITIALIZE_PATH = "Data\\JSON\\INITIALIZE\\Camera_Manager.json";

	//****************************************************
	// �g�����W�V�������\���̂̒�`
	//****************************************************
	struct TRANSITION
	{
		/**
		 * �����̎��s�ł��A�I�[�o�[���C�h�p�̂��ߗ�O���X���[���܂�
		 * 
		 * ���s�����������ꍇ true ��Ԃ��܂��A��Ԃ�؂�ւ��Ă�������
		 */
		virtual bool Execute(CCamera_Manager*& cameraManager);

		/**
		 * ��Ԃ�؂�ւ��܂��A�I�[�o�[���C�h�p�̂��ߗ�O���X���[���܂�
		 */
		virtual void Change(TRANSITION*& r);

		/**
		 * ���݂̃g�����W�V�����̎��s�y�я�Ԋm�F�𓯎��ɍs���܂�
		 */
		static void UpdateTransition(CCamera_Manager*& cameraManager);

		std::string m_NextKey;	// �J�ڗ\��̃J�����̃L�[
	};

	//****************************************************
	// �C���g���\���̂̒�`
	//****************************************************
	struct INTRO : public TRANSITION
	{
		/**
		 * �J�ڗ\��̃J�����̃L�[���󂯎�����
		 */
		inline INTRO(const std::string& key) { m_NextKey = key; }

		// ���s
		bool Execute(CCamera_Manager*& cameraManager) override;
		
		// �ύX
		void Change(TRANSITION*& r) override;
	};

	//****************************************************
	// �A�E�g���\���̂̒�`
	//****************************************************
	struct OUTRO : public TRANSITION
	{
		// ���s
		bool Execute(CCamera_Manager*& cameraManager) override;

		// �ύX
		void Change(TRANSITION*& r) override;
	};

public:

	//****************************************************
	// function
	//****************************************************

	/**
	 * �ۗL���Ă���S�ẴJ�����C���X�^���X�̍X�V�������s���܂�
	 */
	void Update();

	/**
	 * �C���X�^���X���m�Ńp�����[�^���q�����킹�ăg�����W�V���������܂�
	 * �w�肳�ꂽ�L�[�ɊY������C���X�^���X�����݂��Ȃ���Η�O���X���[���܂�
	 * 
	 * key - �J�ڗ\��̃J�����̃L�[
	 */
	void SetTransition(const std::string& key);

	// ���W�X�g���̎Q�Ƃ̎擾
	inline CRegistry<CCamera*>& RefRegistry() const { return *m_upRegistry; }

	// �I�𒆂̃J�����̎Q�Ƃ̎擾
	inline CCamera& RefSelectCamera() const { return *m_upRegistry->BindAtKey(m_SelectKey); }

	// �J�����̑I��
	inline void SetSelectKey(const std::string& key) { m_SelectKey = key; }

private:
	
	//****************************************************
	// special function
	//****************************************************
	CCamera_Manager();				// �f�t�H���g�R���X�g���N�^
	~CCamera_Manager() override;	// �f�X�g���N�^

	//****************************************************
	// function
	//****************************************************
	bool Initialize();	// ����������
	void Finalize();	// �I������

	//****************************************************
	// data
	//****************************************************
	std::unique_ptr<CRegistry<CCamera*>> m_upRegistry;	// �J�����p���W�X�g��

	std::string m_SelectKey;	// �I�𒆃J�����̃L�[
	TRANSITION* m_pTransition;	// �g�����W�V�������̃|�C���^
};