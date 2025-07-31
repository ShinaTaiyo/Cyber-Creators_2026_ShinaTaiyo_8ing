//============================================================================
// 
// �Q�O�Q�T�N�T���Q�Q���F�p�[�e�B�N�����쐬 [particle.h]
// Author : �Ŗ����z
// 
//============================================================================

#ifndef _PARTICLE_H_
#define _PARTICLE_H_	// ��d�C���N���[�h�h�~

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "object_Billboard.h" // �r���{�[�h

//****************************************************
// �O���錾
//****************************************************
class CMove_Info;    // �ړ����
class CHorming_Info; // �z�[�~���O���

//****************************************************
// �p�[�e�B�N���N���X
//****************************************************
class CParticle : public CObject_Billboard
{
public:

	// === ���ʊ֐� ===

	CParticle();	       // �R���X�g���N�^
	~CParticle() override; // �f�X�g���N�^

	// === ���C�t�T�C�N�������o�֐� ===

	HRESULT Init() override;	// �����ݒ�
	void	Uninit() override;	// �I������
	void	Update() override;	// �X�V����
	void	Draw() override;	// �`�揈��

	// === �֐� ===

	CMove_Info* GetMoveInfo() const; // �ړ������擾����
	CHorming_Info* GetHormingInfo() const; // �z�[�~���O�����擾����

	// �ő�T�C�Y
	const D3DXVECTOR3& GetMaxSize() const; // �擾
	void SetMaxSize(D3DXVECTOR3 MaxSize);     // �ݒ�

	// �̗�
	const int& GetLife() const; // �擾
	void SetLife(int nLife);    // �ݒ�

	// �ő�̗�
	const int& GetMaxLife() const; // �擾
	void SetMaxLife(int nMaxLife); // �ݒ�

	const int& GetCntFrame() const; // �t���[�������擾����

	// === �ÓI�����o�֐� ===

	static CParticle* Create(D3DXVECTOR3 InitPos,D3DXVECTOR3 InitSize,D3DXCOLOR Col,int nLife,int nMaxLife); // ����
private:

	// === �����o�ϐ� ===

	int m_nLife,                    // �̗�
		m_nMaxLife,                 // �ő�̗�
		m_nCntFrame;                // �t���[�������J�E���g����
	float m_fHormingYaw,            // �z�[�~���O����ۂ�Yaw�l
	      m_fHormingPitch;          // �z�[�~���O����ۂ�Pitch�l
	bool m_bHorming;                // �z�[�~���O���邩�ǂ���
	D3DXVECTOR3 m_HormingGoalPos,   // �z�[�~���O����ۂ̖ړI�l
	            m_MaxSize;          // �ő�T�C�Y
	CMove_Info* m_pMove_Info;       // �ړ����
	CHorming_Info* m_pHorming_Info; // �z�[�~���O���
};

#endif // _OBJECT_BILLBOARD_H_