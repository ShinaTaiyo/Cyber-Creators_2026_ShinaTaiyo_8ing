//============================================================================
// 
// �Q�O�Q�T�N�T���P�Q���F�^�C�}�[�ɂ��V�[���J�ڎ��� [timer.h]
// Author : �Ŗ����z
// 
//============================================================================

#ifndef _TIMER_H_
#define _TIMER_H_	// ��d�C���N���[�h�h�~

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "object.h"
#include "number.h"
//****************************************************
// �^�C�}�[�N���X
//****************************************************
class CTimer : public CObject
{
public:
	// <special function>
	CTimer();		    // �R���X�g���N�^
	~CTimer() override;	// �f�X�g���N�^

	// <function>
	HRESULT Init() override;    // �����ݒ�
	void	Uninit() override;	// �I������
	void	Update() override;	// �X�V����
	void	Draw() override;   	// �`�揈��

	// <getter/setter>
	const D3DXVECTOR3& GetPos() const; // �ʒu�擾
	void SetPos(D3DXVECTOR3 Pos); // �ʒu�ݒ�

	// <getter/setter>
	const D3DXVECTOR3& GetNumberSize() const; // �����̃T�C�Y�擾
	void SetNumberSize(D3DXVECTOR3 Size);     // �����̃T�C�Y�ݒ�
	
	// <getter/setter>
	const int& GetNowTime() const; // ���݂̎��Ԏ擾
	void SetNowTime(int nTime);    // ���݂̎��Ԑݒ�

	// <getter>
	const bool& GetEnd() const; // �I���������ǂ����擾

	// <static function>
	static CTimer* Create(D3DXVECTOR3 InitPos,D3DXVECTOR3 InitSize);	// ����
private:
	//****************************************************
    // �ÓI�萔���`
    //****************************************************
	static constexpr int s_nMaxTime = 60; // �ő厞��
	// <Function>
	void NumberDisp();               // �����\���X�V

	// <Data>
	D3DXVECTOR3 m_Pos;                // �ʒu
	D3DXVECTOR3 m_NumberSize;         // �T�C�Y
	int m_nNowTime;                   // ���݂̎���
	int m_nNumDigit;                  // ����
	bool m_bEnd;                      // �I���t���O
	int m_nCntFrame;                  // �t���[�����J�E���g
	std::vector<CNumber*> m_vpNumber; // �������I�z�� 
};

#endif // _PALYER_H_