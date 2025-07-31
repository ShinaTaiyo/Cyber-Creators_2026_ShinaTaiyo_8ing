//============================================================================
// 
// �h��Đ��A�w�b�_�t�@�C�� [swaying_player.h]
// Author : ���c����
// 
//============================================================================

// �C���N���[�h�K�[�h
#pragma once

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "quadratic_bezier.h"
#include "camera_manager.h"

//****************************************************
// �h��Đ��N���X���`
//****************************************************
class CSwayingPlayer
{
	//****************************************************
	// �ÓI�����o�萔��錾
	//****************************************************

	// �r���[�㏑���p�p�����[�^
	static const std::array<JSON, 2> s_aOverWriteViewParam;

	//****************************************************
	// �h���ԍ\���̂��`
	//****************************************************
	struct SWAY_STATE
	{
		// �f�X�g���N�^
		virtual ~SWAY_STATE() = default;

		// ��Ԃ̍X�V
		static bool UpdateState(CSwayingPlayer*& refSwayingPlayer);

		// ���s
		virtual bool Execute(CSwayingPlayer*& refSwayingPlayer);

		// �ύX
		virtual void Change(useful::up<SWAY_STATE>& refState);
	};

	//****************************************************
	// �h��C���g����ԍ\���̂��`
	//****************************************************
	struct SWAY_INTRO :public SWAY_STATE
	{
		// �f�t�H���g�R���X�g���N�^
		SWAY_INTRO() = default;

		// �f�X�g���N�^
		~SWAY_INTRO() override = default;

		bool Execute(CSwayingPlayer*& refSwayingPlayer) override;
		void Change(useful::up<SWAY_STATE>& refState) override;
	};

	//****************************************************
	// �h��Đ���ԍ\���̂��`
	//****************************************************
	struct SWAY_PLAY :public SWAY_STATE
	{
		// �ő�J�E���g��
		static constexpr int NUM_MAXCOUNT = 120 - 90;

		// �f�t�H���g�R���X�g���N�^
		SWAY_PLAY() :
			m_Bezier{}
		{}

		// �f�X�g���N�^
		~SWAY_PLAY() override
		{
			m_Bezier.Finalize();
		}

		bool Execute(CSwayingPlayer*& refSwayingPlayer) override;
		void Change(useful::up<SWAY_STATE>& refState) override;

		// �x�W�F�Ȑ�
		CQuadratic_Bezier m_Bezier;
	};

	//****************************************************
	// �h��A�E�g����ԍ\���̂��`
	//****************************************************
	struct SWAY_OUTRO :public SWAY_STATE
	{
		// �f�t�H���g�R���X�g���N�^
		SWAY_OUTRO() :
			m_Bezier{}
		{}

		// �f�X�g���N�^
		~SWAY_OUTRO() override
		{
			m_Bezier.Finalize();
			/* ���}���u */
			CCamera_Manager::GetInstance()->RefRegistry().BindAtKey("Game")->OverwriteView(nullptr);
			CCamera_Manager::GetInstance()->RefRegistry().BindAtKey("Game")->SetUseDecayState(false);
		};

		bool Execute(CSwayingPlayer*& refSwayingPlayer) override;
		void Change(useful::up<SWAY_STATE>& refState) override;

		// �x�W�F�Ȑ�
		CQuadratic_Bezier m_Bezier;
	};

public:
	
	//****************************************************
	// special function
	//****************************************************
	CSwayingPlayer();	// �f�t�H���g�R���X�g���N�^

	//****************************************************
	// function
	//****************************************************

	/**
	 * �U���𔭐������܂�
	 *
	 * coolTimeCnt -> �����N�[���^�C���J�E���g
	 */
	bool Sway(int coolTimeCnt);

private:

	//****************************************************
	// data
	//****************************************************
	useful::up<SWAY_STATE> m_upState;		// ���
	CCamera*			   m_pMainCamera;	// ���C���J�����̃|�C���^
	int					   m_nCoolTimeCnt;	// �N�[���^�C���J�E���g
};