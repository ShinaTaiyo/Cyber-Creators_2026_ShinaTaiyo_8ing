//============================================================================
// 
// �e�N�X�`���A�j���[�^�[�A�w�b�_�t�@�C�� [texture_animator.hpp]
// Author : ���c����
// 
//============================================================================

// �C���N���[�h�K�[�h
#pragma once

//****************************************************
// ���O���useful���`
//****************************************************
namespace useful
{
	//****************************************************
	// �e�N�X�`���A�j���[�^�[�N���X���`
	//****************************************************
	class CTextureAnimator
	{
		//****************************************************
		// �e�N�X�`���A�j���[�V�����p���̍\���̂��`
		//****************************************************
		struct TEXTURE_ANIMATION_DATA
		{
			// �f�t�H���g�R���X�g���N�^
			TEXTURE_ANIMATION_DATA() :
				nChangeRate(0),
				nFrameCounter(0),
				nNowPetternU(0),
				nNowPetternV(0),
				fTexSplitWidth(0.0f),
				fTexSplitHeight(0.0f)
			{}

			int
				nChangeRate,	// �؂�ւ����x
				nFrameCounter,	// �t���[���J�E���^�[
				nNowPetternU,	// U���̌��݂̃p�^�[����
				nNowPetternV;	// V���̌��݂̃p�^�[����

			// �e�N�X�`�������̐��@
			float
				fTexSplitWidth,		// ��
				fTexSplitHeight;	// �c
		};

	public:

		//****************************************************
		// function
		//****************************************************

		/**
		 * �e�N�X�`���A�j���[�V�������s���܂�
		 * �Ԃ�l�̓A�j���[�V�����̊�����ʒm���܂�
		 *  -> �ʏ�X�V����false
		 *  -> 1���������̂�true
		 *
		 * refVtx -> ���_�t�H�[�}�b�g�̃|�C���^�Q�ƁA�K�����b�N���ꂽ����̃|�C���^��n���Ă�������
		 */
		bool Animate(VERTEX_2D*& refVtx);
		bool Animate(VERTEX_3D*& refVtx);

		// �؂�ւ����x�̐ݒ�
		inline void SetChangeRate(int rate) { m_Data.nChangeRate = rate; }

		// �e�N�X�`�������̐��@�̐ݒ�
		inline void SetTexSplitWidth (float texSplitWidth)  { m_Data.fTexSplitWidth = texSplitWidth; }
		inline void SetTexSplitHeight(float texSplitHeight) { m_Data.fTexSplitHeight = texSplitHeight; }

	private:

		//****************************************************
		// funciton
		//****************************************************
		bool ChangeTexturePattern();	// �e�N�X�`���p�^�[���̐؂�ւ�

		//****************************************************
		// data
		//****************************************************
		TEXTURE_ANIMATION_DATA m_Data;	// �e�N�X�`���A�j���[�V�����p���
	};
}