//============================================================================
// 
// �x�W�F�Ȑ��}�l�[�W���[�A�w�b�_�t�@�C�� [bezier_manager.h]
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
#include "quadratic_bezier.h"

//****************************************************
// �x�W�F�Ȑ��}�l�[�W���[�N���X�̒�`
//****************************************************
class CBezier_Manager final : public CSingleton<CBezier_Manager>
{
	//****************************************************
	// �t�����h�錾
	//****************************************************
	friend struct std::default_delete<CBezier_Manager>;
	friend CBezier_Manager* CSingleton<CBezier_Manager>::GetInstance();
	friend void CSingleton<CBezier_Manager>::ExplicitRelease();

	//****************************************************
	// �ÓI�����o�ϐ��̒�`
	//****************************************************
	static constexpr const char* INITIALIZE_PATH = "Data\\JSON\\INITIALIZE\\Bezier_Manager.json";
	static std::string RELOAD_PATH;
	
public:

	//****************************************************
	// function
	//****************************************************

	/* ���}���u */
	static inline void SetReloadPath(const char* path) { RELOAD_PATH = path; };

	/* ���}���u */
	void ExplicitInitialize();

	/**
	 * �ۗL���Ă���S�Ẵx�W�F�Ȑ��C���X�^���X�̍X�V�������s���܂�
	 * �Ԃ�l�͍X�V�̊����󋵁A���o�[�X�t���O�s�g�p�̋Ȑ��̍X�V���������Ă����true��Ԃ��܂�
	 */
	bool Update();

	/**
	 * �ۗL���Ă���S�Ẵx�W�F�Ȑ��C���X�^���X�̕`�揈�����s���܂�
	 * �Ԃ�l����
	 */
	void Draw();

	// �X�V���̃x�W�F�Ȑ��̓񎟋O�Տ�̈ړ��_�̍��W���擾
	inline const D3DXVECTOR3& GetPosTracePointOnCurrentBeizier() { return m_CurrentPos; };

	// ���W�X�g���̎Q�Ƃ̎擾
	inline CRegistry<std::unique_ptr<CQuadratic_Bezier>>& RefRegistry() const { return *m_upRegistry; };

#if 0
	/* GetTracePointOnTrajectoryLine�̃��b�v */
	inline CObject_X* GetTracePointOnTrajectoryLine() const
	{
		// �v�f�̃C���f�b�N�X
		int nElementIdx = 0;

		// �S�C���X�^���X�̍X�V����
		for (auto& it : m_Register->RefDynamicResource())
		{
			// ���̗v�f���Ώۂ̃C���f�b�N�X�łȂ��ꍇ����
			if (m_nOnIdx != nElementIdx)
			{
				// �v�f�̃C���f�b�N�X���C���N�������g
				++nElementIdx;

				continue;
			}

			return it.second->GetTracePointOnTrajectoryLine();
		}
		
		return nullptr;
	}
#endif

private:

	//****************************************************
	// special function
	//****************************************************
	CBezier_Manager();				// �f�t�H���g�R���X�g���N�^
	~CBezier_Manager() override;	// �f�X�g���N�^

	//****************************************************
	// function
	//****************************************************
	bool Initialize();	// ����������
	void Finalize();	// �I������

	//****************************************************
	// data
	//****************************************************
	std::unique_ptr<CRegistry<std::unique_ptr<CQuadratic_Bezier>>> m_upRegistry;	// �x�W�F�Ȑ��p���W�X�g��

	int m_nIdxCurrentBeizier;	// �X�V���̃x�W�F�Ȑ��̃C���f�b�N�X
	int m_nMaxSizeRegistry;		// �v�f�̍ő吔
	D3DXVECTOR3 m_CurrentPos;	/* �X�V���̍��W */
};