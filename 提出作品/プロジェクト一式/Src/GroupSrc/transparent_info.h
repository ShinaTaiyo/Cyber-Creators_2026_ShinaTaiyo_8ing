//============================================================================
// 
// �Q�O�Q�T�N�T���P�T���F�������N���X���쐬 [transparent_info.h]
// Author : �Ŗ����z
// 
//============================================================================

#ifndef _TRANSPARENT_INFO_H_
#define _TRANSPARENT_INFO_H_ // ��d�C���N���[�h�h�~

//****************************************************
// �������N���X
//****************************************************
class CTransparent_Info
{
public:
	// <special function>
	CTransparent_Info(int nMaxValue,int nValue,bool bUse,int nSpeed);  // �R���X�g���N�^
	~CTransparent_Info(); // �f�X�g���N�^

	// <function>
	void Update(D3DXCOLOR & Col); // �X�V����

	// <getter/setter>
	const int& GetValue() const; // �l���擾
	void SetValue(int nValue);   // �l��ݒ�

	// <getter/setter>
	const int& GetMaxValue() const;  // �ő�l���擾
	void SetMaxValue(int nMaxValue); // �ő�l��ݒ�

	// <getter/setter>
	const int& GetSpeed() const; // �l�������x�擾
	void SetSpeed(int nSpeed);   // �l�������x�ݒ�

	// <getter/setter>
	const bool& GetUse() const; // �g�p���邩�ǂ����擾
	void SetUse(bool bUse);     // �g�p���邩�ǂ����ݒ�

private:
	// <Data>
	int 
		m_nValue,    // �����ɂ���l���J�E���g����
	    m_nMaxValue, // �����ɂ���ő�l
	    m_nSpeed; // �l�������x
	bool m_bUse;  // �����ɂ��邩�ǂ���
};
#endif // _Summon_MANAGER_H_