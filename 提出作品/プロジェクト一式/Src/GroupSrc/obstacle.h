//============================================================================
// 
// �Q�O�Q�T�N�U���X���FOBB��������� [obstacle.h]
// Author : �Ŗ����z
// 
//============================================================================

#ifndef _OBSTACLE_H_
#define _OBSTACLE_H_	// ��d�C���N���[�h�h�~

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "object_X.h"     // �I�u�W�F�N�gX
#include "Vec3LerpInfo.h" // �R�����x�N�g�����`��ԏ��
#include "SummonCommonData.h" // �����I�u�W�F�N�g���ʃf�[�^

//****************************************************
// �O���錾
//****************************************************
class COBB;       // OBB���
class CMove_Info; // �ړ����
class CCollisionInfo; // ������
class CCollisionInfo_BoundingBox; // ��������

//****************************************************
// �v���C���[�N���X
//****************************************************
class CObstacle : public CObject_X
{
public:
	// === �񋓌^ ===

	// ��Q���^�C�v
	enum class OBSTACLETYPE
	{
		TYPE00_BLOCK = 0, // �^�C�v�O�F�u���b�N
		MAX
	};

	// === ����֐� ===

	CObstacle(bool bIsSummonManagerObj); // �R���X�g���N�^
	~CObstacle() override;	// �f�X�g���N�^

	// === ���C�t�T�C�N�������o�֐� ===

	HRESULT Init() override;	// �����ݒ�
	void	Uninit() override;	// �I������
	void	Update() override;	// �X�V����
	void	Draw() override;	// �`�揈��

	// === �����o�֐� ===

	COBB* GetOBB() const; // OBB�����擾
	virtual void InitFromJson(const JSON& Json); // �W�F�C�\������������

	// �^�C�v
	const OBSTACLETYPE& GetObstacleType() const; // �擾
	void SetObstacleType(OBSTACLETYPE Type);     // �ݒ�

	// �j��\���ǂ���
	const bool& GetCanbeDestroy() const; // �j��\���ǂ����擾
	void SetCanbeDestroy(bool bDestroy); // �j��\���ǂ����ݒ�

	// �������Z��
	void SetAddRot(D3DXVECTOR3 AddRot);   // �ݒ�
	const D3DXVECTOR3& GetAddRot() const; // �擾

	// ���������Z�������邩�ǂ���
	const bool& GetUseAddRot() const; // �擾
	void SetUseAddRot(bool bUse); // �ݒ�

	// �����I�u�W�F�N�g���ʃf�[�^
	CSummonCommonData& GetSummonCommonData(); // �擾
	void SetSummonCommonData(CSummonCommonData SummonCommonData); // �ݒ�

	const bool& GetSummonManagerObj() const; // �����}�l�[�W���[�����������I�u�W�F�N�g���ǂ����擾

	// === �ÓI�����o�֐� ===

	static CObstacle* Create(const std::string& modelpath, OBSTACLETYPE Type,D3DXVECTOR3 InitPos,bool bIsSummonManagerObj); // ����

private:

	// === �����o�ϐ� ===

	OBSTACLETYPE m_Type; // ���
	bool 
		m_bCanbeDestroy,       // �j��\���ǂ���
		m_bUseAddRot,          // ���������Z�������邩�ǂ���
	    m_bIsSummonManagerObj; // �����}�l�[�W���[�����������I�u�W�F�N�g���ǂ���
	CCollisionInfo_BoundingBox* m_pCollisionInfo_Box; // ������
	COBB* m_pOBB; // OBB������
	D3DXVECTOR3 m_AddRot; // ���Z�������
	CSummonCommonData m_SummonCommonData; // �����I�u�W�F�N�g���ʃf�[�^

	// === �����o�֐� ===

	void ControlInfo_Pos();  // �ʒu�ҏW
	void UpdateCollisionInfo(); // ��������X�V����
	void UpdateFromSummonCommonData(); // �����I�u�W�F�N�g���ʃf�[�^����X�V����
	void Rotating(); // ��]����
	/* �� �g�U�p��������ɒǉ����炵�܂� */
public:

	//****************************************************
	// �g�U�����N���X���`
	//****************************************************
	class CSpreader
	{
	public:

		// �f�t�H���g�R���X�g���N�^ (�C�j�V�����C�U�[�p)
		CSpreader() :
			m_fRate(0.0f),
			m_Dir(VEC3_INIT)
		{}

		// �g�U
		void Spread(CObstacle& r);

		// �i�s���x���Z�b�g
		inline void SetRate(float rate) { m_fRate = rate; }

		// ������ѕ������Z�b�g
		inline void SetDir(const Vec3& dir) { m_Dir = dir; }

	private:
		
		float m_fRate;	// �i�s���x
		Vec3  m_Dir;	// ������ѕ���
	};

	// �g�U����������
	template <typename T> inline void ComposeSpreader(T&& fpFactory) { m_upSpreader = std::forward<T>(fpFactory)(); }

private:

	// �g�U�����ǉ��p
	useful::up<CSpreader> m_upSpreader;
};

#endif // _PALYER_H_