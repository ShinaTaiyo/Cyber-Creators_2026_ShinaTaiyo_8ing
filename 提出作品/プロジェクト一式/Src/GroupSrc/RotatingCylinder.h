//============================================================================
// 
// �Q�O�Q�T�N�U���P�O���F��]�~����AABB�̓����蔻����쐬 [RotatingCylinder.h]
// Author : �Ŗ����z
// 
//============================================================================

//****************************************************
// �Q�d�C���N���[�h�h�~
//****************************************************
#ifndef _ROTATINGCYLINDER_H_
#define _ROTATINGCYLINDER_H_ 

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "bounding_cylinder.h" // boudingCylinder

//****************************************************
// ��]�~�������蔻��N���X
//****************************************************
class CRotatingCylinder : public CCollisionInfo_BoundingCylinder
{
public:
	// === ����֐� ===

	CRotatingCylinder(); // �R���X�g���N�^
	~CRotatingCylinder() override; // �f�X�g���N�^

	// === �����o�֐� ===

	// ����
	void SetRot(D3DXVECTOR3 Rot);      // �ݒ�
	const D3DXVECTOR3& GetRot() const; // �擾
private:
	D3DXVECTOR3 m_Rot; // ����
};
#endif // _COLLISION_H_