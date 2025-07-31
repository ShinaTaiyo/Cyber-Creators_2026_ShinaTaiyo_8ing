//============================================================================
// 
// �e�N�X�`���A�j���[�^�[ [texture_animator.cpp]
// Author : ���c����
// 
//============================================================================

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "texture_animator.hpp"

//****************************************************
// using�f�B���N�e�B�u
//****************************************************
using namespace useful;

//============================================================================
// �e�N�X�`���A�j���[�V����
//============================================================================
bool useful::CTextureAnimator::Animate(VERTEX_2D*& refVtx)
{
	// �e�N�X�`�������̐��@���R�s�[
	float
		fWidth = m_Data.fTexSplitHeight,
		fHeight = m_Data.fTexSplitHeight;

	// ���݂̃p�^�[�����R�s�[
	int
		nNowPatternU = m_Data.nNowPetternU,
		nNowPatternV = m_Data.nNowPetternV;

	// �e�N�X�`�����W�̐ݒ�
	refVtx[0].tex = { fWidth * nNowPatternU,       fHeight * nNowPatternV };
	refVtx[1].tex = { fWidth * (nNowPatternU + 1), fHeight * nNowPatternV };
	refVtx[2].tex = { fWidth * nNowPatternU,       fHeight * (nNowPatternV + 1) };
	refVtx[3].tex = { fWidth * (nNowPatternU + 1), fHeight * (nNowPatternV + 1) };

	// �e�N�X�`���p�^�[���̐؂�ւ�
	if (ChangeTexturePattern())
	{
		return true;
	}

	return false;
}

//============================================================================
// �e�N�X�`���A�j���[�V����
//============================================================================
bool useful::CTextureAnimator::Animate(VERTEX_3D*& refVtx)
{
	// �e�N�X�`�������̐��@���R�s�[
	float
		fWidth = m_Data.fTexSplitHeight,
		fHeight = m_Data.fTexSplitHeight;

	// ���݂̃p�^�[�����R�s�[
	int
		nNowPatternU = m_Data.nNowPetternU,
		nNowPatternV = m_Data.nNowPetternV;

	// �e�N�X�`�����W�̐ݒ�
	refVtx[0].tex = { fWidth * nNowPatternU,       fHeight * nNowPatternV };
	refVtx[1].tex = { fWidth * (nNowPatternU + 1), fHeight * nNowPatternV };
	refVtx[2].tex = { fWidth * nNowPatternU,       fHeight * (nNowPatternV + 1) };
	refVtx[3].tex = { fWidth * (nNowPatternU + 1), fHeight * (nNowPatternV + 1) };

	// �e�N�X�`���p�^�[���̐؂�ւ�
	if (ChangeTexturePattern())
	{
		return true;
	}

	return false;
}

//============================================================================
// �e�N�X�`���p�^�[���̐؂�ւ�
//============================================================================
bool useful::CTextureAnimator::ChangeTexturePattern()
{
	// �t���[���J�E���^�[�̃C���N�������g
	++m_Data.nFrameCounter;

	// �K��l�ɓ��B����܂Ő؂�ւ�����
	if (m_Data.nFrameCounter < m_Data.nChangeRate)
	{
		return false;
	}

	// �J�E���^�[�̓��Z�b�g
	m_Data.nFrameCounter = 0;

	// �@ U���̃p�^�[�������C���N�������g
	++m_Data.nNowPetternU;

	// �A �E�[ (1.0f) �ɓ��B������U���̂ݐ܂�Ԃ�
	if (m_Data.fTexSplitWidth * m_Data.nNowPetternU >= 1.0f)
	{
		// (U���p�^�[���̓��Z�b�g)
		m_Data.nNowPetternU = 0;

		// �B V���̃p�^�[�������C���N�������g
		++m_Data.nNowPetternV;
	}

	// �C ���[�ɓ��B�����痼�����Z�b�g
	if (m_Data.fTexSplitHeight * m_Data.nNowPetternV >= 1.0f)
	{
		m_Data.nNowPetternU = m_Data.nNowPetternV = 0;

		// �A�j���[�V������1��������ʒm����
		return true;
	}

	return false;
}