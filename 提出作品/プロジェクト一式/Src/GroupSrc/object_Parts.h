//============================================================================
// 
// �p�[�c�I�u�W�F�N�g�A�w�b�_�t�@�C�� [object_Parts.h]
// Author : ���c����
// 
//============================================================================

#ifndef _OBJECT_PARTS_H_
#define _OBJECT_PARTS_H_	// ��d�C���N���[�h�h�~

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "object.h"

// X���f�����p
#include "X_manager.h"

//****************************************************
// �p�[�c�I�u�W�F�N�g�N���X
//****************************************************
class CObject_Parts : public CObject
{
public:

	// <special function>
	CObject_Parts(LAYER Priority);	// �`��D��x�w��R���X�g���N�^
	~CObject_Parts() override;		// �f�X�g���N�^

	// <function>
	HRESULT	Init() override;	// �����ݒ�
	void	Uninit() override;	// �I������
	void	Update() override;	// �X�V����
	void	Draw() override;	// �`�揈��

	// <setter>
	void SetSyncRot(D3DXVECTOR3 Rot);	// �V���N�������̐ݒ�
	void SetSyncPos(D3DXVECTOR3 Pos);	// �V���N�����W�̐ݒ�
	
	// <getter/setter>
	const CObject_Parts *const GetParent() const;	// �e�p�[�c�̃|�C���^�擾
	void SetParent(CObject_Parts* pParent);			// �e�p�[�c�̃|�C���^�ݒ�

	// <getter/setter>
	CX_Manager::MODEL* GetModel();				// ���f���擾
	void BindModel(CX_Manager::MODEL* pModel);	// ���f������

	// <getter/setter>
	const D3DXVECTOR3& GetScaleOffset() const;	// �k�ڃI�t�Z�b�g�擾
	void SetScaleOffset(D3DXVECTOR3 Scale);		// �k�ڃI�t�Z�b�g�ݒ�

	// <getter/setter>
	const D3DXVECTOR3& GetScale() const;	// �k�ڎ擾
	void SetScale(D3DXVECTOR3 Scale);		// �k�ڐݒ�

	// <getter/setter>
	const D3DXVECTOR3& GetRotOffset() const;	// �����I�t�Z�b�g�擾
	void SetRotOffset(D3DXVECTOR3 Rot);			// �����I�t�Z�b�g�ݒ�

	// <getter/setter>
	const D3DXVECTOR3& GetRot() const;	// �����擾
	void SetRot(D3DXVECTOR3 Rot);		// �����ݒ�

	// <getter/setter>
	const D3DXVECTOR3& GetPosOffset() const;	// ���W�I�t�Z�b�g�擾
	void SetPosOffset(D3DXVECTOR3 Pos);			// ���W�I�t�Z�b�g�ݒ�

	// <getter/setter>
	const D3DXVECTOR3& GetPos() const;	// ���W�擾
	void SetPos(D3DXVECTOR3 Pos);		// ���W�ݒ�

	// <getter/setter>
	const D3DXCOLOR& GetCol() const;	// �F���擾
	void SetCol(D3DXCOLOR Col);			// �F��ݒ�

	// <getter/setter>
	const bool& GetUseCol() const;	// �F���f���擾
	void SetUseCol(bool Col);		// �F���f��ݒ�

	// <getter>
	const D3DXMATRIX& GetMtxWorld() const;	// ���[���h�s����擾
	void SetMtxWorld(D3DXMATRIX Mtx);		// ���[���h�s���ݒ�

	// <virtual getter>
	virtual D3DXVECTOR3 GetSize() const;	// �T�C�Y���擾
	virtual float GetRadius() const;		// ���a���擾
	virtual float GetHeight() const;		// �������擾

	// <static function>
	static CObject_Parts* Create(CObject::LAYER Layer, const std::string& key, CObject_Parts* pParent);	// ����

private:

	// <function>
	void AdjustRotAngle();	// �����𒲐�����
	void SetMtxWorld();		// ���[���h�s��ݒ�

	// <data>
	D3DXVECTOR3			m_RotSync;		// �V���N������
	D3DXVECTOR3			m_PosSync;		// �V���N�����W
	CObject_Parts*		m_pParent;		// �e�p�[�c�̃|�C���^
	CX_Manager::MODEL*	m_pModel;		// ���f�����
	D3DXVECTOR3			m_ScaleOffset;	// �k�ڃI�t�Z�b�g
	D3DXVECTOR3			m_Scale;		// �k��
	D3DXVECTOR3			m_RotOffset;	// �����I�t�Z�b�g
	D3DXVECTOR3			m_Rot;			// ����
	D3DXVECTOR3			m_PosOffset;	// ���W�I�t�Z�b�g
	D3DXVECTOR3			m_Pos;			// ���W
	D3DXCOLOR			m_Col;			// �F
	bool				m_bUseCol;		// �F���f
	D3DXMATRIX			m_MtxWorld;		// ���[���h�s��
};

#endif // _OBJECT_PARTS_H_