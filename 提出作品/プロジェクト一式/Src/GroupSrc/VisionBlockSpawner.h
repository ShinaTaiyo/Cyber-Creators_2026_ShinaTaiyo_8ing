//============================================================================
// 
// �Q�O�Q�T�N�U���P�Q���F����W�Q�I�u�W�F�N�g����������N���X���쐬 [VisionBlockSpawner.h]
// Author : �Ŗ����z
// 
//============================================================================

//****************************************************
// 2�d�C���N���[�h�h�~
//****************************************************
#ifndef _VISIONBLOCKSPAWNER_H_
#define _VISIONBLOCKSPAWNER_H_	// ��d�C���N���[�h�h�~

//****************************************************
// �C���N���[�h
//****************************************************
#include "object.h" // �I�u�W�F�N�g
#include "VisionBlocker.h" // ����W�Q�I�u�W�F�N�g

//****************************************************
// ���N���X
//****************************************************
class CVisionBlockSpawner : public CObject
{
public:
	// === ����֐� ===

	CVisionBlockSpawner();  // �R���X�g���N�^
	~CVisionBlockSpawner(); // �f�X�g���N�^

	// === ���C�t�T�C�N�������o�֐� ===

	HRESULT Init() override;	// �����ݒ�
	void	Uninit() override;	// �I������
	void	Update() override;	// �X�V����
	void	Draw() override;	// �`�揈��

	// === �����o�֐� ===

	// �������[�g
	const int& GetSpawnRate() const; // �擾
	void SetSpawnRate(int nRate);    // �ݒ�

	const int& GetCntFrame() const; // �t���[�����J�E���g�擾

	// �����̊�_�ƂȂ�ʒu
	const D3DXVECTOR3& GetSpawnPoint() const;   // �擾
	void SetSpawnPoint(D3DXVECTOR3 SpawnPoint); // �ݒ�

	// �����͈͍ő�
	const D3DXVECTOR3& GetMaxSpawnRange() const;     // �擾
	void SetMaxSpawnRage(D3DXVECTOR3 MaxSpawnRange); // �ݒ�

	// �����͈͍ŏ�
	const D3DXVECTOR3& GetMinSpawnRange() const;    // �擾
	void SetMinSpawnRage(D3DXVECTOR3 MinSpawnRange); // �ݒ�

	// �x�[�X�ƂȂ�ړ���
	const D3DXVECTOR3& GetBaseMove() const; // �擾
	void SetBaseMove(D3DXVECTOR3 BaseMove); // �ݒ�

	// �t���ύő呬�x
	const float& GetMaxLeafSpeed() const;  // �擾
	void SetMaxLeafSpeed(float fMaxSpeed); // �ݒ�

	// �t���ύŒᑬ�x
	const float& GetMinLeafSpeed() const;  // �擾
	void SetMinLeafSpeed(float fMinSpeed); // �ݒ�

	// �t���ς�h�炷�p�x���Z�ʍő�
	const float& GetMaxLeafSwayAddAngle() const;     // �擾
	void SetMaxLeafSwayAddAngle(float fMaxAddAngle); // �ݒ�

	// �t���ς�h�炷�p�x���Z�ʍŏ�
	const float& GetMinLeafSwayAddAngle() const;     // �擾
	void SetMinLeafSwayAddAngle(float fMinAddAngle); // �ݒ�

	// �t���ω��Z�p�x�͈�
	const float& GetLeafAddRotRange() const; // �擾
	void SetLeafAddRotRange(float fRange);   // �ݒ�

	// �t���ϑ̗�
	const int& GetLeafLife() const; // �擾
	void SetLeafLife(int nLife);    // �ݒ�

	// �t���ώ��
	const CVisionBlocker::TYPE& GetLeafType() const; // �擾
	void SetLeafType(CVisionBlocker::TYPE Type);     // �ݒ�

	// === �ÓI�����o�֐� ===

	// ��������
	static CVisionBlockSpawner* Create(
		D3DXVECTOR3 SpawnPoint, // ������_
		D3DXVECTOR3 BaseMove,   // �x�[�X�ړ���
		int nSpawnRate, // �������[�g
		D3DXVECTOR3 MaxSpawnRange, // �����͈͍ő�
		D3DXVECTOR3 MinSpawnRange, // �����͈͍ŏ�
		float fMaxLeafSpeed, // �t���ϑ��x�ő�
		float fMinLeafSpeed, // �t���ϑ��x�Œ�
		float fMaxLeafSwayAddAngle, // �t���ς�h�炷�p�x���Z�ʍő�
		float fMinLeafSwayAddAngle, // �t���ς�h�炷�p�x���Z�ʍŏ�
		float fLeafAddRotRange, // �t���ς̌������Z�͈�
		int nLeafLife,
		CVisionBlocker::TYPE LeafType // �t���ς̎��
	);

private:
	// === �����o�ϐ� ===

	int
		m_nSpawnRate, // �������[�g
		m_nCntFrame,  // �t���[�����J�E���g
		m_nLeafLife;  // �̗�
	float
		m_fMaxLeafSpeed,          // �t���ύő呬�x
		m_fMinLeafSpeed,          // �t���ύŒᑬ�x
		m_fMaxLeafSwayAddAngle,   // �t���ς�h�炷�p�x���Z�ʍő�
		m_fMinLeafSwayAddAngle,   // �t���ς�h�炷�p�x���Z�ʍŏ�
		m_fLeafAddRotRange;       // �t���ω��Z�p�x�͈�
	D3DXVECTOR3
		m_SpawnPoint,    // ��_
		m_MaxSpawnRange, // �����͈͍ő�
		m_MinSpawnRange, // �����͈͍ŏ�
		m_BaseMove;      // �x�[�X�ƂȂ�ړ���
	CVisionBlocker::TYPE m_LeafType; // ��������W�Q�I�u�W�F�N�g�^�C�v

	// === �����o�֐� ===

	D3DXVECTOR3 DecideSpawnPos();   // �����ʒu����
	float DecideLeafSpeed();        // ���x����
	float DecideLeafSwayAddAngle(); // �t���ς��h���p�x���Z�ʌ���
	D3DXVECTOR3 DecideLeafAddRot(); // �t���ω��Z�p�x����

};
#endif // _VISIONBLOCKSPAWNER_H_