//============================================================================
// 
// (��)�v���C���[�A�w�b�_�t�@�C�� [player.h]
// Author : ���c����
// 
//============================================================================

#ifndef _PLAYER_H_
#define _PLAYER_H_	// ��d�C���N���[�h�h�~

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "object_X.h"  // �I�u�W�F�N�gX
#include "camera_manager.h"
#include "space.h"

//****************************************************
// �O���錾
//****************************************************
class CTarget; // �^�[�Q�b�g
class CMove_Info; // �ړ����
class CPlayerState; // �v���C���[���

//****************************************************
// �v���C���[�N���X
//****************************************************
class CPlayer : public CObject_X
{
public:
	// === �񋓌^ ===
	enum class TYPE
	{
		TUTORIAL = 0, // �`���[�g���A��
		GAME, // �Q�[��
		MAX
	};

	// === ����֐� ===

	CPlayer();				// �R���X�g���N�^
	~CPlayer() override;	// �f�X�g���N�^

	// === ���C�t�T�C�N�������o�֐� ===

	HRESULT Init() override;	// �����ݒ�
	void	Uninit() override;	// �I������
	void	Update() override;	// �X�V����
	void	Draw() override;	// �`�揈��

	// === �����o�֐� ===

	// �C
	const int& GetAirPower() const; // �擾
	void SetAirPower(int nPower); // �ݒ�

	const D3DXVECTOR3& GetMove() const; // �ړ��ʎ擾

	void SetDamage(int nFrame); // �_���[�W���o
	CTarget* GetTarget() const; // �^�[�Q�b�g�擾

	// �v���C���[���
	CPlayerState* GetState() const;     // �擾
	void SetState(CPlayerState* State); // �ݒ�

	// �^�C�v
	const TYPE& GetType() const; // �擾
	void SetType(TYPE Type);     // �ݒ�

	// === �ÓI�����o�֐� ===

	static CPlayer* Create(D3DXVECTOR3 InitPos);	// ����
	static const int& GetMaxAirPower(); // �C�̍ő�l���擾

	// �ڕW������ݒ�
	inline void SetRotTarget(const Vec3& rotTarget) { m_RotTarget = rotTarget; }

	void ExecuteUpSpace(); // �蓁���o����
private:
	// === �����o�֐� ===

	void Rotation();	// ��]
	void Damage(); // �_���[�W����
	void BlowVisionBlocker(); // ����W�Q�I�u�W�F�N�g�𐁂���΂�
	void AirPowerLimit(); // �C�̒l���� 
	void CollisionParticle(); // �p�[�e�B�N���Ƃ̓����蔻��

	// === �V�F�[�_�[�p�����o�֐� ===

	bool InitToonShader(LPDIRECT3DDEVICE9 device, const char* fxFile);
	void Draw_UsingEffect(LPD3DXEFFECT pEffect);

	// === �ÓI�����o�ϐ� ===

	static constexpr float
		MAX_VELOCITY = 0.5f,// �����x���
		JUMP_FORCE = 6.0f,  // �W�����v��
		s_fVisionBlockerBlowInertia = 0.01f, // ����W�Q�I�u�W�F�N�g�𐁂���΂��␳�l
		s_fVisionBlockerBlowLength = 100.0f, // ����W�Q�I�u�W�F�N�g�𐁂���΂�����
		s_fVisionBlockerBlowSpeed = 5.0f; // ����W�Q�I�u�W�F�N�g�𐁂���΂����x
	static constexpr int
		s_nMaxAirPower = 100,          // �C�̍ő�p���[
		s_nINITAIRPOWER = 0;           // �ŏ��̋C�̃|�C���g

	// === �v���C���[�p�����o�ϐ� ===

	int
		m_nAirPower,          // �C�̗�
		m_nDamageFrame;       // �_���[�W��Ԃ̃t���[����
	D3DXVECTOR3
		m_Move,       // �ړ���
		m_Velocity,   // �����x
		m_RotTarget;  // �ڕW����
	TYPE m_Type;      // �^�C�v

	std::unique_ptr<CSpace>			m_upSpace;		// ��Ԑ��䏈��������

	CTarget* m_pTarget; // �^�[�Q�b�g�ւ̃|�C���^

	CMove_Info* m_pMoveInfo; // �ړ����
	CPlayerState* m_pState; // �v���C���[���

	// === �V�F�[�_�[�p�����o�ϐ� ===

	LPD3DXEFFECT m_pToonEffect;
	float m_RimPowor;  // �s��
	float m_RimIntensity;  // ���邳
	float m_RimThickness;  // ����
	LPDIRECT3DTEXTURE9 m_pToonMap;
	std::string m_fxFilePath;             // �ǂݍ���.fx�t�@�C���̃p�X

	LPD3DXMESH m_pMesh;			//���b�V���ւ̃|�C���^

};

#endif // _PALYER_H_