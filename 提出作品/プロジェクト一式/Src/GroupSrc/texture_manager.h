//============================================================================
// 
// �e�N�X�`���}�l�[�W���[�A�w�b�_�t�@�C�� [texture_manager.h]
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
#include "registry.tpp"

//****************************************************
// �e�N�X�`���}�l�[�W���[�N���X�̒�`
//****************************************************
class CTexture_Manager final : public CSingleton<CTexture_Manager>
{
	//****************************************************
	// �t�����h�錾
	//****************************************************
	friend struct std::default_delete<CTexture_Manager>;
	friend CTexture_Manager* CSingleton<CTexture_Manager>::GetInstance();

	//****************************************************
	// �ÓI�����o�ϐ��̒�`
	//****************************************************
	static constexpr const char* INITIALIZE_PATH = "Data\\JSON\\INITIALIZE\\Texture_Manager.json";

public:

	//****************************************************
	// function
	//****************************************************

	// ���W�X�g���̎Q�Ƃ̎擾
	inline CRegistry<LPDIRECT3DTEXTURE9>& RefRegistry() const { return *m_upRegistry; }

private:

	//****************************************************
	// special function
	//****************************************************
	CTexture_Manager();				// �f�t�H���g�R���X�g���N�^
	~CTexture_Manager() override;	// �f�X�g���N�^

	//****************************************************
	// function
	//****************************************************
	bool Initialize();	// ����������
	void Finalize();	// �I������

	//****************************************************
	// data
	//****************************************************
	std::unique_ptr<CRegistry<LPDIRECT3DTEXTURE9>> m_upRegistry;	// �e�N�X�`���p���W�X�g��
};