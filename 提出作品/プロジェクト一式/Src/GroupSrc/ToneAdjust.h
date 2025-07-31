//==============================================
// 
// �|�X�g�G�t�F�N�g[�F���ω�] <ToneAdjust.h>
// Auther �ؓc�А�
// 
//==============================================
#ifndef _TONEADJUST_H_
#define _TONEADJUST_H_  // ��d�C���N���[�h�h�~

//====================================
// �C���N���[�h
//====================================
#include "pch.h"  // �v���R���p�C���ς݃w�b�_�[
#include "object_2D.h"  // �I�u�W�F�N�g2D�̏����g��

// ���_�t�H�[�}�b�g(�F���ω��V�F�[�_�[��p)
#define FVF_SCREEN_VERTEX (D3DFVF_XYZRHW | D3DFVF_TEX1)

// �F���ω��V�F�[�_�[�N���X
class CToneAdjust :public CObject_2D
{
public:
	CToneAdjust();		//�R���X�g���N�^
	~CToneAdjust()override;		//�f�X�g���N�^
	HRESULT Init()override;	//�����ݒ�
	void Uninit()override;		//�I������
	void Update()override;		//�X�V����
	void Draw()override;		//�`�揈��

	static CToneAdjust* Create();  // ��������
	static void SetUseDraw(bool bUse); // �`�悷�邩�ǂ����ݒ�

private:
	bool InitToneAdjust(LPDIRECT3DDEVICE9 device, const char* fxFile);  // �V�F�[�_�[�̏�����
	void UninitToneAdjust();   // �V�F�[�_�[�̔j��
	void UpdateToneAdjust(float deltaTime);   // �V�F�[�_�[�̍X�V

	void RenderToneAdjust(LPDIRECT3DDEVICE9 device);  // �V�F�[�_�[�̕`��
	void CreateFullScreenQuad(LPDIRECT3DDEVICE9 device);  // �|���S���̕`��

	LPD3DXEFFECT m_pEffect;    // �G�t�F�N�g
	std::string m_fxFilePath;             // �ǂݍ���.fx�t�@�C���̃p�X

	// �e�N�X�`���̍\����
	struct SCREEN_VERTEX {
		float x, y, z, rhw;
		float u, v;
	};
	bool m_enableEffect;  // �G�t�F�N�g�̕\�����
	bool m_bActive;   // ImGui�̕\�����
	// �J���[�ێ��ϐ�
	float m_colorR;
	float m_colorG;
	float m_colorB;

	float m_Alpha;  // �����x

	// deltaTime �̎��Ԃ�����ϐ�
	float m_totalTime;
	bool m_bLevelBarEnabled;
	bool m_bChange;  // �F�̐؂�ւ�
	int m_nCountFrame;  // �J�E���g�p�t���[��

	//==== �t�F�[�h�֘A�̕ϐ� ======
	float m_fadeProgress;  // �t�F�[�h�̐i���
	float m_FadeDirection;
	bool m_FadeActive;   // �t�F�[�h�̐؂�ւ�

	// �e�}�N���̒�`
	static constexpr float FADE_SPEED = 0.008f;  // �t�F�[�h���̂̑��x
	static constexpr float FADE_SPEED_DOWN = 0.015f;  // �t�F�[�h���̂̑��x

	static bool s_bUseDraw; // �������g�p���邩�ǂ���

};

#endif