//============================================================================
// 
// ���C�g�A�w�b�_�t�@�C�� [light.h]
// Author : ���c����
// 
//============================================================================

#ifndef _LIGHT_H_
#define _LIGHT_H_	// ��d�C���N���[�h�h�~

//****************************************************
// ���C�g�N���X
//****************************************************
class CLight
{
public:

	CLight();	// �R���X�g���N�^
	~CLight();	// �f�X�g���N�^

	HRESULT Init();	// �����ݒ�
	void Update();	// �X�V����
	D3DXVECTOR3 GetLightDir();  // ���C�g�̎擾

private:

	void SetTempDirection();	// �����̃e���v�����Z�b�g
	void Create();				// ����
	void ChangeID();			// ����ԍ��ύX
	void Control();				// ����
	void Reset();				// ���Z�b�g
	void SetLight();			// ���C�g�Z�b�g

	static const int MAX_LIGHT = 3;	// ���C�g�̐�

	D3DLIGHT9 m_light[MAX_LIGHT];			// ���C�g���
	D3DXVECTOR3 m_tempDirection[MAX_LIGHT];	// ���C�g�̌���
	int m_nID;								// ���C�g����ԍ��p
};

#endif // _LIGHT_H_