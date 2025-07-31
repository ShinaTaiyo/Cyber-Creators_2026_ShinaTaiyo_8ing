//============================================================================
// 
// X�I�u�W�F�N�g�A�w�b�_�t�@�C�� [object_X.h]
// Author : ���c����
// 
//============================================================================

#ifndef _OBJECT_X_H_
#define _OBJECT_X_H_	// ��d�C���N���[�h�h�~

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "object.h"
// X���f�����p
#include "X_manager.h"

//****************************************************
// �O���錾
//****************************************************
class CRender_Collision;

//****************************************************
// X�I�u�W�F�N�g�N���X
//****************************************************
class CObject_X : public CObject
{
public:

	// <special function>
	CObject_X(LAYER Priority = LAYER::BG);	// �`��D��x�w��R���X�g���N�^
	~CObject_X() override;					// �f�X�g���N�^

	// <function>
	HRESULT	Init() override;	// �����ݒ�
	void	Uninit() override;	// �I������
	void	Update() override;	// �X�V����
	void	Draw() override;	// �`�揈��

	// <getter/setter>
	CX_Manager::MODEL* GetModel();				// ���f���擾
	void BindModel(CX_Manager::MODEL* pModel);	// ���f������

	// <getter/setter>
	const D3DXVECTOR3& GetRot() const;	// �����擾
	void SetRot(D3DXVECTOR3 Rot);		// �����ݒ�

	// <getter/setter>
	const D3DXVECTOR3& GetPos() const;	// ���W�擾
	void SetPos(D3DXVECTOR3 Pos);		// ���W�ݒ�

	// <getter/setter>
	const D3DXVECTOR3& GetScale() const;	// �k�ڎ擾
	void SetScale(D3DXVECTOR3 Scale);		// �k�ڐݒ�

	// <getter/setter>
	const D3DXVECTOR3& GetVtxMax() const; // �ő咸�_�擾
	void SetVtxMax(D3DXVECTOR3 VtxMax);   // �ő咸�_�ݒ�


	// <getter/setter>
	const D3DXVECTOR3& GetVtxMin() const; // �ŏ����_�擾
	void SetVtxMin(D3DXVECTOR3 VtxMin);   // �ŏ����_�ݒ�

	// <getter/setter>
	const D3DXVECTOR3& GetBaseVtxMax() const;   // ���̍ő咸�_�擾
	void SetBaseVtxMax(D3DXVECTOR3 BaseVtxMax); // ���̍ő咸�_�ݒ�

	// <getter/setter>
	const D3DXVECTOR3& GetBaseVtxMin() const;   // ���̍ŏ����_�擾
	void SetBaseVtxMin(D3DXVECTOR3 BaseVtxMin); // ���̍ŏ����_�ݒ�

	// <getter/setter>
	const D3DXCOLOR& GetCol() const;	// �F���擾
	void SetCol(D3DXCOLOR Col);			// �F��ݒ�

	// <setter>
	void SetSize(D3DXVECTOR3 Size);     // �T�C�Y�ݒ�

	// <getter/setter>
	const bool& GetUseCol() const;	// �F���f���擾
	void SetUseCol(bool Col);		// �F���f��ݒ�

	// <virtual getter>
	virtual D3DXVECTOR3 GetSize() const;	// �T�C�Y���擾
	virtual float GetRadius() const;		// ���a���擾
	virtual float GetHeight() const;		// �������擾

	// <getter/setter>
	const D3DXMATRIX GetMtxWorld() const; // ���[���h�}�g���b�N�X���擾
	void SetMtxWorld(D3DXMATRIX Mtx);     // ���[���h�}�g���b�N�X��ݒ�

	// <static function>
	static CObject_X* Create(const std::string& key);					// ����
	static CObject_X* Create(LAYER Priority, const std::string& key);	// ����
	static CObject_X* Create(const JSON& Json);							// ����

	static std::unique_ptr<CObject_X> upCreate(const JSON& Json);	// ����

	// Vector�n��ԃ|�C���^���Q��
	inline up<useful::CVectorInterpolator>& RefVectorInterpolator() { return m_upVectorInterpolator; }

private:

	/// <summary> �A���t�@�l�������p </summary>
	static constexpr float ALPHA_INIT = 1.0f;

	// <data>
	CX_Manager::MODEL*	m_pModel;	              // ���f�����
	D3DXVECTOR3			m_Rot;		              // ����
	D3DXVECTOR3			m_Pos;		              // ���W
	D3DXVECTOR3			m_Scale;	              // �k��
	D3DXVECTOR3         m_Size;                   // �T�C�Y
	D3DXVECTOR3         m_VtxMax;                 // �ő咸�_
	D3DXVECTOR3         m_BaseVtxMax;             // ���̍ő咸�_
	D3DXVECTOR3         m_VtxMin;                 // �ŏ����_
	D3DXVECTOR3         m_BaseVtxMin;             // ���̍ŏ����_
	D3DXCOLOR			m_Col;		              // �F
	bool				m_bUseCol;	              // �F���f
	D3DXMATRIX			m_MtxWorld;	              // ���[���h�s��

	// <function>
	void AdjustVtx();        // ���_����
	void AdjustRotAngle();	 // �����𒲐�����
    virtual void UpdateMtxWorld();		// ���[���h�s��ݒ�

	// Vector�n��ԍ�����
	useful::up<CVectorInterpolator> m_upVectorInterpolator;
 };

#endif // _OBJECT_X_H_