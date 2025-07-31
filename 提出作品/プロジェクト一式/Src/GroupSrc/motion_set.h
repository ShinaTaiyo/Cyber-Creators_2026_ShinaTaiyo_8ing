//============================================================================
// 
// ���[�V�����Z�b�g�A�w�b�_�t�@�C�� [motion_set.h]
// Author : ���c����
// 
//============================================================================

#ifndef _MOTION_SET_H_
#define _MOTION_SET_H_	// ��d�C���N���[�h�h�~

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "motion_set.h"
#include "object.h"

//****************************************************
// �O���錾
//****************************************************
class CObject_Parts;

//****************************************************
// ���[�V�����Z�b�g�N���X
//****************************************************
class CMotion_Set : public CObject
{
public:

	//****************************************************
	// ����ڕW�l�̍\���̂��`
	//****************************************************
	struct KeyDest
	{
		D3DXVECTOR3 ScaleTarget;	// �ڕW�k��
		D3DXVECTOR3 RotTarget;		// �ڕW����
		D3DXVECTOR3 PosTarget;		// �ڕW���W
	};

	//****************************************************
	// �L�[���̍\���̂��`
	//****************************************************
	struct Key
	{
		int			nMaxFrame;	// �L�[�̑��t���[����
		KeyDest*	apDest;		// �p�[�c���Ƃ̓���ڕW�l
	};

	//****************************************************
	// ���[�V�������̍\���̂��`
	//****************************************************
	struct Motion
	{
		bool bLoop;				// ���[�v�t���O
		WORD wMaxKey;			// ���[�V�����̑��L�[��
		std::vector<Key> vpKey;	// �L�[���
	};

	// <special function>
	CMotion_Set(LAYER Layer);	// �R���X�g���N�^
	~CMotion_Set() override;	// �f�X�g���N�^

	// <function>
	HRESULT	Init() override;	// �����ݒ�
	void	Uninit() override;	// �I������
	void	Update() override;	// �X�V����
	void	Draw() override;	// �`�揈��

	// <setter>
	void SetSyncRot(D3DXVECTOR3 Rot);	// �V���N��������ݒ�
	void SetSyncPos(D3DXVECTOR3 Pos);	// �V���N�����W��ݒ�

	// <getter/setter>
	D3DXVECTOR3 GetRot() const;		// �������擾
	void SetRot(D3DXVECTOR3 Rot);	// ������ݒ�
	
	// <getter/setter>
	D3DXVECTOR3 GetPos() const;		// ���W���擾
	void SetPos(D3DXVECTOR3 Pos);	// ���W��ݒ�

	// <getter/setter>
	WORD GetNowMotion() const;		// �Đ����̃��[�V�����ԍ����擾
	void SetNowMotion(WORD wIdx);	// �Đ����̃��[�V�����ԍ���ݒ�

	// <getter>
	bool GetStopState() const;			// �X�g�b�v�t���O���擾
	CObject_Parts* GetParentParts();	// �e�p�[�c���擾

	// <static function>
	static CMotion_Set* Create(LAYER Layer, TYPE Type, JSON Json);	// ����

protected:

	// <function>
	void SetMotion(LAYER Layer, JSON Json);	// ���[�V���������Z�b�g

private:

	// <function>
	void Animation();		// ����
	void CountFrame();		// �t���[�����J�E���g
	void CorrectTarget();	// �ڕW�l�ւ̕␳

	// <getter>
	CMotion_Set::Motion* const GetNowMotionPtr();	// ���݂̃��[�V�����̃|�C���^���擾
	CMotion_Set::Key* const GetNowKey();			// ���݂̃L�[�̃|�C���^���擾

	// <data>
	D3DXVECTOR3					m_Rot;			// ����
	D3DXVECTOR3					m_Pos;			// ���W
	bool						m_bStop;		// �X�g�b�v�t���O
	WORD						m_wNowMotion;	// �Đ����̃��[�V����
	WORD						m_wNowKey;		// �Đ����̃L�[
	WORD						m_wNowFrame;	// �Đ����̃t���[��
	std::vector<CObject_Parts*>	m_vpModelParts;	// ���f���p�[�c
	CObject_Parts*				m_pMostParent;	// �e�p�[�c
	WORD						m_wMaxParts;	// ���f���̑��p�[�c��
	WORD						m_wMaxMotion;	// �����[�V������
	std::vector<Motion>			m_vpMotion;		// ���[�V�������
};

#endif	// _MOTION_SET_