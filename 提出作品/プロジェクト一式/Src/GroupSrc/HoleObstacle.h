//============================================================================
// 
// �Q�O�Q�T�N�U���P�P���F�����󂢂���Q�������� [HoleObstacle.h]
// Author : �Ŗ����z
// 
//============================================================================

//****************************************************
// �Q�d�C���N���[�h�h�~
//****************************************************
#ifndef _HOLEOBSTACLE_H_
#define _HOLEOBSTACLE_H_

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "obstacle.h" // ��Q��

//****************************************************
// �O���錾
//****************************************************
class CHole; // ��

//****************************************************
// ������
//****************************************************
class CHoleObstacle : public CObstacle
{
public:
	// === ����֐� ===

	CHoleObstacle(bool bIsSummonManagerObj); // �R���X�g���N�^
	~CHoleObstacle() override;	              // �f�X�g���N�^

	// === ���C�t�T�C�N�������o�֐� ===

	HRESULT Init() override;	// �����ݒ�
	void	Uninit() override;	// �I������
	void	Update() override;	// �X�V����
	void	Draw() override;	// �`�揈��

	// === �����o�֐� ===

	void InitFromJson(const JSON& Json) override; // �W�F�C�\������������
	CHole* GetHole() const; // �����擾

	// === �ÓI�����o�֐� ===

	static CHoleObstacle* Create(const std::string& modelpath, CObstacle::OBSTACLETYPE Type, D3DXVECTOR3 InitPos, bool bIsSummonManagerObj); // ����

private:
	// === �����o�ϐ� ===

	CHole* m_pHole; // ���ւ̃|�C���^

};

#endif // _PALYER_H_