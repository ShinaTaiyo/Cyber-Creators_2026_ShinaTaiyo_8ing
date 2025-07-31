//============================================================================
//
// �񎟃x�W�F�Ȑ��A�w�b�_�t�@�C�� [quadratic_bezier.h]
// Author : ���c����
//
//============================================================================

// �C���N���[�h�K�[�h
#pragma once

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "object_X.h"

//****************************************************
// �񎟃x�W�F�Ȑ��N���X�̒�`
//****************************************************
class CQuadratic_Bezier
{
	//****************************************************
	// �^�G�C���A�X�̒�`
	//****************************************************
	using LINE_VTX = LPDIRECT3DVERTEXBUFFER9;

	//****************************************************
	// �ÓI�����o�萔�̒�` (����J)
	//****************************************************
	static constexpr unsigned int NUM_TRACE = 3;	// �ړ��_�̐�
	static constexpr float DEFAULT_SPEED = 0.005f;	// �f�t�H���g�ړ����x

public:

	//****************************************************
	// �ÓI�����o�萔�̒�` (���J)
	//****************************************************
	static constexpr int NUM_CONTROL = 3;	// ����_�̐�

	//****************************************************
	// special function
	//****************************************************
	CQuadratic_Bezier();	// �f�t�H���g�R���X�g���N�^
	~CQuadratic_Bezier();	// �f�X�g���N�^

	//****************************************************
	// function
	//****************************************************

	/**
	 * ����_�Ԃ����Ԓ�����`�悷�邽�߂̒��_�o�b�t�@�̐����ƁA����_ / �ړ��_�̃��f���������s���܂�
	 */
	bool Initialize();

	/**
	 * ���_�o�b�t�@�̔j���ƁA�����ڗp���f���ւ̔j���\����s���܂�
	 */
	void Finalize();

	/**
	 * �ړ��_�̍��W�X�V���s���܂�
	 * �I�[�ɓ��B�����ۂɁAtrue��Ԃ��܂�
	 */
	bool Update();

	/**
	 * ����_�Ԃ����Ԓ����̕\���ƁA����_ / �ړ��_�̃��f���̍��W�ݒ���s���܂�
	 * ����ړ����Ɍ���A�O�Ճ��f���̐������s���܂�
	 */
	void Draw();

	// �I�������C���f�b�N�X�̐���_�̍��W��ݒ�
	inline void SetPosToControlPointForSelectedIdx(const D3DXVECTOR3& pos, int idx) { m_ControlPoint[idx] = pos; }

	// �񎟋O�Տ�ɂ���ړ��_�̃|�C���^���擾
	[[ deprecated ]] inline CObject_X* GetPtrTracePointOnTrajectoryLine() const { return m_pTracePointModel[2]; }

	// �񎟋O�Տ�ɂ���ړ��_�̍��W���擾
	inline const D3DXVECTOR3& GetPosTracePointOnTrajectoryLine() const { return m_TracePoint[2]; }

	// �i�s���x��ݒ�
	inline void SetSpeed(float speed) { m_fSpeed = speed; }

	// ���]�g�p�t���O�̏�Ԃ̎擾 / �ݒ�
	inline bool GetUseReverse() const { return m_bUseReverse; }
	inline void SetUseReverse(bool state) { m_bUseReverse = state; }

private:

	//****************************************************
	// function
	//****************************************************
	HRESULT CreateVtxBuff();			// ���_�o�b�t�@�𐶐�
	void	SetReleaseAllTrajectory();	// �񎟋O�՗p�̑S�Ẵ��f���ɔj���\��
	bool	UpdateRate();				// �i�s�x�̍X�V
	void	UpdateVtxPos();				// ���_���W�̍X�V

	//****************************************************
	// data
	//****************************************************
	D3DXVECTOR3	m_ControlPoint[NUM_CONTROL];	// ����_���W
	D3DXVECTOR3	m_TracePoint[NUM_TRACE];		// �ړ��_���W
	float		m_fRate;						// �i�s�x
	float		m_fSpeed;						// �i�s���x
	bool		m_bUseReverse;					// ���]�g�p�t���O
	bool		m_bIsReverse;					// ���]�t���O
	bool		m_bIsEndFirstTime;				// ����ړ��I���t���O
	bool		m_bDispModel;					// ���f���\���t���O

	// �����ڗp
	D3DXMATRIX m_MtxWorld;							// ���[���h�s��
	LINE_VTX   m_pLineVtx;							// ���_�o�b�t�@
	CObject_X* m_pControlPointModel[NUM_CONTROL];	// ����_���f��
	CObject_X* m_pTracePointModel[NUM_TRACE];		// �ړ��_���f��
	std::vector<CObject_X*> m_vTrajectoryModel;		// �񎟋O�Ճ��f��
};