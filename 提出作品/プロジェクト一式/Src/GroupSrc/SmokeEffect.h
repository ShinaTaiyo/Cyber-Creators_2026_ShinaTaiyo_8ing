//==============================================
// 
// �|�X�g�G�t�F�N�g[��] <SmokeEffect.h>
// Auther �ؓc�А�
// 
//==============================================
#ifndef _SMOKEEFFECT_H_
#define _SMOKEEFFECT_H_


#include "pch.h"
#include "object_2D.h"

// ���̒��_�t�H�[�}�b�g
#define SMOKE_FVF (D3DFVF_XYZRHW | D3DFVF_TEX1)


class CSmokeEffect :public CObject_2D
{
public:
	// === ����֐� ===

	CSmokeEffect();		// �R���X�g���N�^
	~CSmokeEffect()override;		// �f�X�g���N�^

	// === ���C�t�T�C�N�������o�֐� ===

	HRESULT Init()override;	// �����ݒ�
	void Uninit()override;		// �I������
	void Update()override;		// �X�V����
	void Draw()override;		// �`�揈��

	// === �ÓI�����o�֐� ===

	static CSmokeEffect* Create();
	static void SetUseDraw(bool bUse); // �`�悷�邩�ǂ����ݒ�
private:
	// === �ÓI�����o�ϐ� ===
	static bool s_bUseDraw; // �������g�p���邩�ǂ���

	bool InitSmokeEffect(LPDIRECT3DDEVICE9 device, const char* fxFile);
	void UpdateSmokeEffect(float deltaTime);
	void RenderSmokeEffect(LPDIRECT3DDEVICE9 device);
	void InitScreenQuad(LPDIRECT3DDEVICE9 device);

	struct Vertex {
		float x, y, z, rhw;
		float u, v;
	};
	LPD3DXEFFECT m_pEffect;    // �G�t�F�N�g
	std::string m_fxFilePath;             // �ǂݍ���.fx�t�@�C���̃p�X

	LPDIRECT3DTEXTURE9 m_noiseTex;
	LPDIRECT3DVERTEXBUFFER9 m_quadVB;

	float m_time;
	float m_Opacity;
	static constexpr int SCR_WIDTH = 1280;
	static constexpr int SCR_HEIGHT = 720;

	// ImGui
	bool m_enableEffect;  // �G�t�F�N�g�̕\�����
	bool m_bActive;
};

#endif