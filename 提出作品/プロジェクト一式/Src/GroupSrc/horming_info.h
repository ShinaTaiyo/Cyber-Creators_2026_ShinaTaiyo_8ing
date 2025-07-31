//============================================================================
// 
// �Q�O�Q�T�N�T���Q�Q���F�z�[�~���O���N���X���쐬 [horming_info.h]
// Author : �Ŗ����z
// 
//============================================================================

#ifndef _horming_INFO_H_
#define _horming_INFO_H_	// ��d�C���N���[�h�h�~

//****************************************************
// �C���N���[�h
//****************************************************
#include "object.h"

//****************************************************
// �����}�l�[�W���[�X�e�[�g�N���X
//****************************************************
class CHorming_Info
{
public:
	// === �񋓌^ ===

	// �_�����
	enum class LOCKONTYPE
	{
		NONE = 0, // �Ȃ�
		PLAYER,   // �v���C���[
		MAX       // �ő�
	};

	// === ���ʊ֐� ===

	CHorming_Info();  // �R���X�g���N�^
	~CHorming_Info(); // �f�X�g���N�^

	// === ���C�t�T�C�N�������o�֐� ===

	const D3DXVECTOR3 & Update(D3DXVECTOR3& Pos,CObject * pObj); // �X�V����

	// === �����o�֐� ===

	// �ڕW�ʒu
	const D3DXVECTOR3& GetGoalPos() const; // �擾
	void SetGoalPos(D3DXVECTOR3 GoalPos);  // �ݒ�

	// �ړ���
	const D3DXVECTOR3& GetMove() const; // �擾
	void SetMove(D3DXVECTOR3 Move);     // �ݒ�

	// �␳�l
	const float& GetAdjust() const; // �擾
	void SetAdjust(float fAdjust);  // �ݒ�

	// ���x
	const float& GetSpeed() const;   // �擾
	void SetSpeed(float fSpeed);           // �ݒ�

	// ���̏������g�p���邩�ǂ���
	const bool& GetUseHorming() const; // �擾
	void SetUseHorming(bool bUse);     // �ݒ�

	// Yaw�l
	const float& GetYaw() const; // �擾
	void SetYaw(float fYaw);     // �ݒ�

	// Pitch�l
	const float& GetPitch() const; // �擾
	void SetPitch(float fPitch);   // �ݒ�

	// ���Z���x
	const float& GetAddSpeed() const;  // �擾
	void SetAddSpeed(float fAddSpeed); // �ݒ�

	// ���Z�␳�x
	const float& GetAddAdjust() const;   // �擾
	void SetAddAdjust(float fAddAdjust); // �ݒ�

	// �_���I�u�W�F�N�g���
	const LOCKONTYPE& GetLockOnType() const;   // �擾
	void SetLockOnType(LOCKONTYPE TargetType); // �ݒ�
private:

	// === �����o�ϐ� ===

	D3DXVECTOR3 
		m_GoalPos,   // �ڕW�ʒu
	    m_Move;      // �ړ���
	LOCKONTYPE m_LockOnType; // �_���I�u�W�F�N�g���
	float m_fAdjust,         // �␳�l
		m_fAddAdjust,        // ���Z�␳�l
		m_fSpeed,            // ���x
		m_fAddSpeed,         // �����x
		m_fYaw,              // Yaw�l
		m_fPitch;            // Pitch�l
	bool m_bUse;             // �z�[�~���O���g�p���邩�ǂ���

	// === �����o�֐� ===

	void CalcYaw(D3DXVECTOR3 & Pos,CObject * pObj);     // Yaw�v�Z
	void CalcPitch(D3DXVECTOR3 & Pos,CObject * pObj);   // Pitch�v�Z
	void LockOnPlayer(CObject* pObj); // �v���C���[��_��
	
};
#endif // _Summon_MANAGER_H_