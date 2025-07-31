//============================================================================
// 
// �Q�O�Q�T�N�U���P�Q���F����W�Q���������� [VisionBlocker.h]
// Author : �Ŗ����z
// 
//============================================================================

//****************************************************
// �Q�d�C���N���[�h�h�~
//****************************************************
#ifndef _VISIONBLOCKER_H_
#define _VISIONBLOCKER_H_

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "object_X.h"     // �I�u�W�F�N�gX

//****************************************************
// �O���錾
//****************************************************
class CMove_Info; // �ړ����

//****************************************************
// ����W�Q�N���X
//****************************************************
class CVisionBlocker : public CObject_X
{
public:
	// === �񋓌^ ===

	// ���
	enum class TYPE
	{
		LEAF00 = 0, // �t���ςO�O
		LEAF01,     // �t���ςO�P
		LEAF02,     // �t���ςO�Q
		MAX
	};

	// === ����֐� ===

	CVisionBlocker(); // �R���X�g���N�^
	~CVisionBlocker() override;	// �f�X�g���N�^

	// === ���C�t�T�C�N�������o�֐� ===

	HRESULT Init() override;	// �����ݒ�
	void	Uninit() override;	// �I������
	void	Update() override;	// �X�V����
	void	Draw() override;	// �`�揈��

	// === �����o�֐� ===

	// ���
	const TYPE& GetType() const; // �擾
	void SetType(TYPE Type);     // �ݒ�

	// �t���ς��h���p�x
	const float& GetLeafSwayAngle() const; // �擾
	void SetLeafSwayAngle(float fAngle);   // �ݒ�

	// �t���ς�h�炷���߂ɉ��Z����p�x
	const float& GetLeafSwayAddAngle() const;  // �擾
	void SetLeafSwayAddAngle(float fAddAngle); // �ݒ�

	// �t���ς�h�炷���x
	const float& GetLeafSwaySpeed() const;   // �擾
	void SetLeafSwaySpeed(float fSwaySpeed); // �ݒ�

	// ���Z�p�x
	const D3DXVECTOR3& GetAddRot() const; // �擾
	void SetAddRot(D3DXVECTOR3 AddRot);   // �ݒ�

	// ������΂��ړ���
	const D3DXVECTOR3& GetBlowMove() const; // �擾
	void SetBlowMove(D3DXVECTOR3 Move);     // �ݒ�

	// ������΂�����
	const float& GetBlowInertia() const; // �擾
	void SetBlowInertia(float fInertia); // �ݒ�

	CMove_Info* GetMoveInfo() const; // �ړ����擾

	// �̗�
	const int& GetLife() const; // �擾
	void SetLife(int nLife);    // �ݒ�

	// === �ÓI�����o�֐� ===

	static CVisionBlocker* Create(TYPE Type, D3DXVECTOR3 InitPos); // ����
private:
	// === �ÓI�����o�ϐ� ===

	static const std::string s_aVisionBlockerModelType[static_cast<int>(TYPE::MAX)]; // ����W�Q���f���^�C�v

	// === �����o�ϐ� ===

	CMove_Info* m_pMove_Info; // �ړ����
	float
		m_fLeafSwayAngle,    // �t���ς��h���p�x
		m_fLeafSwayAddAngle, // �t���ς�h�炷���߂ɉ��Z����p�x
		m_fLeafSwaySpeed;    // �t���ς�h�炷���x
	D3DXVECTOR3
		m_AddRot,   // ���Z�p�x
		m_BlowMove; // ������΂��ړ���
	int m_nLife; // �̗�
	float m_fBlowInertia; // ������΂�����
	TYPE m_Type; // ����W�Q�^�C�v

	// === �����o�֐� ===

	void Blow(); // ������΂�����
};

#endif // _PALYER_H_