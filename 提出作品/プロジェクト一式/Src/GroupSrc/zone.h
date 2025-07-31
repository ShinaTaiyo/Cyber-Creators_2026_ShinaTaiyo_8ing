//============================================================================
// 
// �Q�O�Q�T�N�T���X���F�]�[���N���X���쐬 [zone.h]
// Author : �Ŗ����z
// 
//============================================================================

#ifndef _ZONE_H_
#define _ZONE_H_	// ��d�C���N���[�h�h�~

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "object_3D.h"      // 3D�|���S�����o������
#include "collision.h"      // �����蔻����
#include "bounding_box.h"   // �����`�����蔻��
#include "move_info.h"      // �ړ����
#include "Vec3LerpInfo.h" // �R�����x�N�g�����`��ԏ��
#include "SummonCommonData.h" // �����I�u�W�F�N�g���ʃf�[�^

//****************************************************
// �O���錾
//****************************************************

//****************************************************
// �]�[���N���X
//****************************************************
class CZone : public CObject_3D
{
public:
	enum class TYPE
	{
		SAFEZONE = 0,
		MAX
	};

	// <special function>
	CZone(bool bIsSummonManagerObj); // �R���X�g���N�^
	~CZone() override;	// �f�X�g���N�^

	// <function>
	HRESULT Init() override;	// �����ݒ�
	void	Uninit() override;	// �I������
	void	Update() override;	// �X�V����
	void	Draw() override;	// �`�揈��
	void InitFromJson(const JSON& Json); // �W�F�C�\������������

	// <static function>
	static CZone* Create(TYPE Type,D3DXVECTOR3 InitPos,D3DXVECTOR3 Size,bool bIsSummmonManagerObj);	// ����

	//< Getter>
	CCollisionInfo* GetCollisionInfo() const; // ������擾
	
    // <getter/setter>
	const bool& GetSummonManagerObj() const; // �����}�l�[�W���[�����������I�u�W�F�N�g���ǂ����擾

	// �����I�u�W�F�N�g���ʃf�[�^
	CSummonCommonData& GetSummonCommonData(); // �擾
	void SetSummonCommonData(CSummonCommonData SummonCommonData); // �ݒ�
private:

	//****************************************************
	// �ÓI�萔���`
	//****************************************************
	static const std::string s_aSafeZoneTextureName[static_cast<int>(TYPE::MAX)]; // �e�N�X�`����
	static constexpr float s_fAdjustScale = 0.5f; // �g�嗦�␳�l�i�G�f�B�^�g���킹�邽�߂ł��j

	// <Data>
	bool m_bIsSummonManagerObj; // �����}�l�[�W���[�����������I�u�W�F�N�g���ǂ���
	CCollisionInfo_BoundingBox * m_pCollisionInfo_Box; // ������
	CSummonCommonData m_SummonCommonData; // �����I�u�W�F�N�g���ʃf�[�^

	// === �����o�֐� ===

	void UpdateFromSummonCommonData(); // �����I�u�W�F�N�g���ʃf�[�^����X�V����
};

#endif // _PALYER_H_