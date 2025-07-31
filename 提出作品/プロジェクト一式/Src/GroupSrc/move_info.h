//============================================================================
// 
// �Q�O�Q�T�N�T���P�U���F�ړ����N���X���쐬 [move_info.h]
// Author : �Ŗ����z
// 
//============================================================================

#ifndef _MOVE_INFO_H_
#define _MOVE_INFO_H_	// ��d�C���N���[�h�h�~

//****************************************************
// �C���N���[�h
//****************************************************
#include "object.h"

//****************************************************
// �����}�l�[�W���[�X�e�[�g�N���X
//****************************************************
class CMove_Info
{
public:
	// <special function>
	CMove_Info();  // �R���X�g���N�^
	~CMove_Info(); // �f�X�g���N�^

	// <function>
	void Update(D3DXVECTOR3 & Pos,CObject * pObj); // �X�V�����i���̃N���X�̏�����ǂ�ňʒu�����X�V���܂��j�^�C���X�P�[�������f���������̂ŃI�u�W�F�N�g�̃|�C���^��������

	// <getter/setter>
	void SetMove(D3DXVECTOR3 Move);     // �ړ��ʐݒ�
	const D3DXVECTOR3& GetMove() const; // �ړ��ʎ擾

	// <getter/setter>
	void SetInertia(float fInertia); // �����ݒ�
	const float& GetInertia() const; // �����擾

	// <getter/setter>
	void SetUseMove(bool bUse);     // �ړ����邩�ǂ����ݒ�
	const bool& GetUseMove() const; // �ړ����邩�ǂ����擾

	// <getter/setter>
	void SetUseInertia(bool bUse);     // �������g�p���邩�ǂ����ݒ�
	const bool& GetUseInertia() const; // �������g�p���邩�ǂ����擾

	void ControlInfo(); // ��񑀍�

private:
	// <function>
	void CalcInertia(); // �����v�Z

	// <Data>
	D3DXVECTOR3 m_Move; // �ړ���
	float m_fInertia;   // ����
	bool 
		m_bUseInertia, // �������g�p���邩�ǂ���
	    m_bUseMove;    // �ړ��ʂ��g�p���邩�ǂ���
};
#endif // _Summon_MANAGER_H_