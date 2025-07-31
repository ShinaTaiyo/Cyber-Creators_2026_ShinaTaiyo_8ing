//============================================================================
// 
// X���f���}�l�[�W���[�A�w�b�_�t�@�C�� [X_manager.h]
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

//****************************************************
// X���f���}�l�[�W���[�N���X�̒�`
//****************************************************
class CX_Manager final : public CSingleton<CX_Manager>
{
	//****************************************************
	// �t�����h�錾
	//****************************************************
	friend struct std::default_delete<CX_Manager>;
	friend CX_Manager* CSingleton<CX_Manager>::GetInstance();

	//****************************************************
	// �ÓI�����o�ϐ��̒�`
	//****************************************************
	static constexpr const char* INITIALIZE_PATH = "Data\\JSON\\INITIALIZE\\X_Manager.json";

public:

	//****************************************************
	// ���f�����̍\���̂̒�`
	//****************************************************
	struct MODEL
	{
		LPD3DXMESH			pMesh;		// ���b�V�����
		D3DXVECTOR3			Size;		// �T�C�Y
		D3DXVECTOR3         VtxMax;     // �ő咸�_
		D3DXVECTOR3         VtxMin;     // �ŏ����_
		LPD3DXBUFFER		pBuffMat;	// �}�e���A�����
		DWORD				dwNumMat;	// �}�e���A����
		D3DXCOLOR*			apColMat;	// �}�e���A���F	
		LPDIRECT3DTEXTURE9* apTex;		// �e�N�X�`��
	};

	//****************************************************
	// function
	//****************************************************

	// ���W�X�g���̎Q�Ƃ̎擾
	inline CRegistry<MODEL*>& RefRegistry() const { return *m_upRegistry; }

private:

	//****************************************************
	// special function
	//****************************************************
	CX_Manager();			// �f�t�H���g�R���X�g���N�^
	~CX_Manager() override;	// �f�X�g���N�^

	//****************************************************
	// function
	//****************************************************
	bool Initialize();	// ����������
	void Finalize();	// �I������
	
	D3DXVECTOR3 LoadSize(const std::string& path, D3DXVECTOR3& VtxMax, D3DXVECTOR3& VtxMin);	// �T�C�Y�ǂݍ���

	//****************************************************
	// data
	//****************************************************
	std::unique_ptr<CRegistry<MODEL*>> m_upRegistry;	// X���f���p���W�X�g��

	/* �}�e���A���e�N�X�`���̃L���b�V�� */
	std::map<std::string, LPDIRECT3DTEXTURE9> m_mCache;
};