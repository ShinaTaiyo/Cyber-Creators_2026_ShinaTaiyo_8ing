//============================================================================
// 
// �Q�O�Q�T�N�T���W���F�^�[�Q�b�g�N���X���쐬 [Target.h]
// Author : �Ŗ����z
// 
//============================================================================

#ifndef _TARGET_H_
#define _TARGET_H_	// ��d�C���N���[�h�h�~

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "object_3D.h" // �I�u�W�F�N�g3D 

//****************************************************
// �O���錾
//****************************************************
class CTransparent_Info; // �����x���

//****************************************************
// �v���C���[�N���X
//****************************************************
class CTarget : public CObject_3D
{
public:

	// <special function>
	CTarget();				// �R���X�g���N�^
	~CTarget() override;	// �f�X�g���N�^

	// <function>
	HRESULT Init() override;	// �����ݒ�
	void	Uninit() override;	// �I������
	void	Update() override;	// �X�V����
	void	Draw() override;	// �`�揈��

	// <getter/setter>
	const bool& GetUseControl() const; // ���삷�邩�ǂ����擾
	void SetUseControl(bool bControl); // ���삷�邩�ǂ����ݒ�

	// <getter>
	CTransparent_Info* GetTransparent_Info() const; // �����x�����擾

	// <setter>
	void SetPosTarget(D3DXVECTOR3 Pos); // �ړI�n��ݒ�

	// �ړ������邩�ǂ���
	const bool& GetUseMove() const; // �擾
	void SetUseMove(bool bUse);     // �ݒ�

	// �J�����̈ʒu�𒲐����邩�ǂ���
	const bool& GetUseAdjustCameraPos() const; // �擾
	void SetUseAdjustCameraPos(bool bUse); // �ݒ�

	// <static function>
	static CTarget* Create(D3DXVECTOR3 InitPos);	// ����

private:
	//****************************************************
	// �ÓI�萔���`
	//****************************************************
	static constexpr float MOVESPEED = 5.0f; // �ړ����x
	static constexpr float
		s_fMoveAdjust = 0.2f, // �ړI�n�ֈړ�������␳��
		s_fMoveLimitMinX = -235.0f,
		s_fMoveLimitMaxX = 235.0f,
		s_fMoveLimitMinY = -140.0f,
		s_fMoveLimitMaxY = 140.0f;

	// === �ϐ� ===

	bool m_bControl; // ���삷�邩�ǂ���
	bool m_bUseMove; // �ړ������邩�ǂ���
	bool m_bUseAdjustCameraPos; // �J�����̈ʒu�𒲐����邩�ǂ���
	D3DXVECTOR3 m_Move;    // �ړ���
	D3DXVECTOR3 m_PosOld;  // 1f�O�̈ʒu
	D3DXVECTOR3 m_PosTarget; // �_���ʒu
	D3DXVECTOR3 m_PosTargetOld; // 1f�O�̑_���ʒu
	CTransparent_Info* m_pTransParent_Info; // �����x���

	// === �����o�֐� ===

	void Control();  // ���삷��
	void LimitRange();   // ��ʊO�ɂ͂ݏo���Ȃ��悤�ɂ���
	void AdjustCameraPos(); // �J�����̈ʒu�𒲐�����
};

#endif // _PALYER_H_