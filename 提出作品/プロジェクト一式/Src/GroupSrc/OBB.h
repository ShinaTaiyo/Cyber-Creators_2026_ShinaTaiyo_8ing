//============================================================================
// 
// �Q�O�Q�T�N�U���X���FOBB������������� [OBB.h]
// Author : �Ŗ����z
// 
//============================================================================

#ifndef _OBB_H_
#define _OBB_H_	// ��d�C���N���[�h�h�~

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "collision.h" //CollisionStrategy���N���X������̂�

//****************************************************
// OBB����N���X
//****************************************************
class COBB : public CCollisionInfo
{
public:
	// === ����֐� ===

	COBB(); // �R���X�g���N�^
	~COBB() override; // �f�X�g���N�^

	// === �����o�֐� ===

	// �ő咸�_
	void SetVtxMax(D3DXVECTOR3 VtxMax);   // �ݒ�
	const D3DXVECTOR3& GetVtxMax() const; // �擾
										  
	// �ŏ����_				  
	void SetVtxMin(D3DXVECTOR3 VtxMin);   // �ݒ�
	const D3DXVECTOR3& GetVtxMin() const; // �擾

	// ���� 
	void SetRot(D3DXVECTOR3 Rot);      // �ݒ�
	const D3DXVECTOR3& GetRot() const; // �擾
private:
	// === �����o�ϐ� ===

	D3DXVECTOR3
		m_VtxMax, // �ő咸�_
		m_VtxMin, // �ŏ����_
		m_Rot;    // ����

};
#endif // _COLLISION_H_