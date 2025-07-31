//============================================================================
// 
// �e�N�X�`���I�u�W�F�N�g�A�w�b�_�t�@�C�� [object_Texture.h]
// Author : ���c����
// 
//============================================================================

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "object.h"

//****************************************************
// �e�L�X�g���b�V���N���X
//****************************************************
class CObject_Texture : public CObject
{
	//****************************************************
	// �ÓI�����o�萔�̒�`
	//****************************************************
	static constexpr WORD NUM_VTX = 4;
	static constexpr WORD NUM_PRIM = 2;

public:

	//****************************************************
	// special funtion
	//****************************************************
	CObject_Texture(LAYER Priority = LAYER::FRONT);	// �R���X�g���N�^
	~CObject_Texture() override;					// �f�X�g���N�^

	//****************************************************
	// function
	//****************************************************
	HRESULT	Init() override;	// �����ݒ�
	void	Uninit() override;	// �I������
	void	Update() override;	// �X�V����
	void	Draw() override;	// �`�揈��

	/**
	 * �e�N�X�`���I�u�W�F�N�g�𐶐����A�C���X�^���X�̃|�C���^��Ԃ��܂�
	 * 
	 * json -> �uOBJECT�v�t�H���_���̐����f�[�^��n���Ă�������
	 */
	static CObject_Texture* Create(const JSON& json);

	// �e�N�X�`���̎擾
	inline LPDIRECT3DTEXTURE9& GetTexture () { return m_pTexture; }

	// �T�[�t�F�C�X���擾
	inline LPDIRECT3DSURFACE9& GetSurface() { return m_pSurface; }

	// �e�N�X�`�����[�u�̎擾 / �ݒ�
	inline const D3DXVECTOR2& GetTextureMove() const { return m_TextureMove; }
	inline void SetTextureMove(const D3DXVECTOR2& textureMove) { m_TextureMove = textureMove; }

	// ���b�V���T�C�Y�̎擾 / �ݒ�
	inline const D3DXVECTOR2& GetMeshSize() const { return m_MeshSize; }
	inline void SetMeshSize(const D3DXVECTOR2& size) { m_MeshSize = size; }

	// �ڕW���b�V���T�C�Y�̎擾 / �ݒ�
	inline const D3DXVECTOR2& GetMeshSizeTarget() const { return m_MeshSizeTarget; }
	inline void SetMeshSizeTarget(const D3DXVECTOR2& sizeTarget) { m_MeshSizeTarget = sizeTarget; }

	// �����̎擾 / �ݒ�
	inline const D3DXVECTOR3& GetRot() const { return m_Rot; }
	inline void SetRot(const D3DXVECTOR3& rot) { m_Rot = rot; }

	// �ڕW�����̎擾 / �ݒ�
	inline const D3DXVECTOR3& GetRotTarget() const { return m_RotTarget; }
	inline void SetRotTarget(const D3DXVECTOR3& rotTarget) { m_RotTarget = rotTarget; }

	// ���W�̎擾 / �ݒ�
	inline const D3DXVECTOR3& GetPos() const { return m_Pos; }
	inline void SetPos(const D3DXVECTOR3& pos) { m_Pos = pos; }

	// �ڕW�����̎擾 / �ݒ�
	inline const D3DXVECTOR3& GetPosTarget() const { return m_PosTarget; }
	inline void SetPosTarget(const D3DXVECTOR3& posTarget) { m_PosTarget = posTarget; }

	// �F�̎擾 / �ݒ�
	inline const D3DXCOLOR& GetCol() const { return m_Col; }
	inline void SetCol(const D3DXCOLOR& col) { m_Col = col; }

	// �ڕW�F�̎擾 / �ݒ�
	inline const D3DXCOLOR& GetColTarget() const { return m_ColTarget; }
	inline void SetColTarget(const D3DXCOLOR& colTarget) { m_ColTarget = colTarget; }

private:

	//****************************************************
	// funtion
	//****************************************************
	HRESULT CreateVtxBuff();	// ���_�o�b�t�@�̐���
	HRESULT CreateTex();		// �e�N�X�`���̐���
	void	CorrectToTarget();	// �ڕW�l�ւ̕��

	//****************************************************
	// data
	//****************************************************
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;			// ���_�o�b�t�@�̃|�C���^
	LPDIRECT3DTEXTURE9		m_pTexture;			// �e�N�X�`���̃|�C���^
	LPDIRECT3DSURFACE9		m_pSurface;			// �T�[�t�F�C�X�̃|�C���^
	D3DXVECTOR2				m_TextureSize;		// �e�N�X�`���T�C�Y
	D3DXVECTOR2				m_TextureMove;		// �e�N�X�`�����[�u
	D3DXVECTOR2				m_MeshSize;			// ���b�V���T�C�Y
	D3DXVECTOR2				m_MeshSizeTarget;	// �ڕW���b�V���T�C�Y
	float					m_fDecayRate;		// ��������
	D3DXVECTOR3				m_Rot;				// ����
	D3DXVECTOR3				m_RotTarget;		// �ڕW����
	D3DXVECTOR3				m_Pos;				// ���W
	D3DXVECTOR3				m_PosTarget;		// �ڕW���W
	D3DXCOLOR				m_Col;				// �F
	D3DXCOLOR				m_ColTarget;		// �ڕW�F
	float					m_fLength;			// �W�J�p�Ίp��
	float					m_fAngle;			// �Ίp���p�p�x
	D3DXMATRIX				m_MtxWorld;			// ���[���h�s��
};