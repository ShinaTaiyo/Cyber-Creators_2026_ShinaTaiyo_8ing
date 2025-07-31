//================================
//
//�u�C�v�̒~��(EffectAccumulation.h)
// Auther �ؓc�А�
// 
//================================
#ifndef _EFFECTACCUMULATION_H_
#define _EFFECTACCUMULATION_H_

//====================================
// �C���N���[�h
//====================================

#include "pch.h"  // �v���R���p�C���ς݃w�b�_�[
#include "object_2D.h"  // �I�u�W�F�N�g2D�̏����g��
#include "summon_manager.h"

//====================================
// �O���錾
//====================================


// ���_�t�H�[�}�b�g(�C�V�F�[�_�[��p)
#define FVF_SCREENVERTEX (D3DFVF_XYZRHW | D3DFVF_TEX1)

class CEffectAccumulation :public CObject_2D
{
public:
	CEffectAccumulation();	// �R���X�g���N�^
	~CEffectAccumulation() override;	// �f�X�g���N�^
	HRESULT Init() override;			// �����ݒ�
	void Uninit() override;			// �I������
	void Update() override;			// �X�V����
	void Draw() override;			// �`�揈��
	static CEffectAccumulation* Create();	// ����

private:
	LPDIRECT3DTEXTURE9 m_sceneRenderTarget;
	LPDIRECT3DTEXTURE9 m_fireTexture;
	LPD3DXEFFECT m_fireEffect;
	LPDIRECT3DSURFACE9 m_sceneSurface;
	LPDIRECT3DVERTEXBUFFER9 m_fullscreenVB; // �t���X�N���[���N�A�b�h�p
	float m_fireTime;
	std::string m_fxFilePath;             // �ǂݍ���.fx�t�@�C���̃p�X

	CSummon_Manager* m_pSuummonManager;

	struct DeformWave {
		float time = 0.0f;
		float duration = 0.05f; // �b
		float power = 0.03f;    // ���̔g�̋���
		float timeScale = 0.025f;  // ���Z�{��
		bool active = false;   // ���݃A�N�e�B�u��
	};
	DeformWave currentWave;

	// ���_�̍\����
	struct ScreenVertex {
		D3DXVECTOR4 pos;
		D3DXVECTOR2 uv;
	};


	// �J���[�ێ��ϐ�
	float m_colorR;
	float m_colorG;
	float m_colorB;
	// �~�Ɛ��̋���ϐ�
	float m_circlePowor;
	float m_sharpnessPowor;
	float m_totalDeformPower;
	float m_circleSize;

	bool m_enableEffect;  // �G�t�F�N�g�̕\�����
	bool m_bActive;
	bool m_bSwitch;



	int pressCount;
	float powerPerPress;   // �������Ƃ̑�����
	float maxPower;
	float baseValue;

	bool InitPostEffect(LPDIRECT3DDEVICE9 device, const char* fxFile);  // �V�F�[�_�[�Ɗe�����̏�����
	void ShutdownPostEffect();  // �ϐ��̔j��
	void UpdatePostEffect(float deltaTime);  // �V�F�[�_�[�̍X�V
	void RenderEffect(LPDIRECT3DDEVICE9 device);  // �V�F�[�_�[�̕`��

	float GetWaveStrength(float t);
	float EaseInSmooth(float t);



	static constexpr int TEX_SIZE_X = 1280;  // �e�N�X�`�����W X
	static constexpr int TEX_SIZE_Y = 720;   // �e�N�X�`�����W Y

	static constexpr float TEX_F_SIZE_X = 1280;  // �e�N�X�`�����W X
	static constexpr float TEX_F_SIZE_Y = 720;   // �e�N�X�`�����W Y

	static constexpr float MAX_SIZE = 5.5f;   // �G�t�F�N�g�̍ő�T�C�Y
	static constexpr float MAX_POWOR = 0.5f;   // �~�Ɛ��̕ύX��̍ő�̋���
	static constexpr float NORMAL_POWOR_CIRCLE = 0.3f;   // �~�̒ʏ�̋���
	static constexpr float NORMAL_POWOR_SHARPNESS = 0.1f;   // ���̒ʏ�̋���
	static constexpr float VALUE = 4.0f;   // ���オ�ς��Ƃ��̈��l

	static constexpr float MIN_SIZE = 2.0f;  // �G�t�F�N�g�T�C�Y�̍ŏ��l
	static constexpr float START_RATIO = 0.4f;   // �G�t�F�N�g�J�n�ʒu

};


#endif