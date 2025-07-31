//============================================================================
// 
// �v���R���p�C���w�b�_�[ [pch.h]
// Author : ���c����
// 
//============================================================================

// �C���N���[�h�K�[�h
#pragma once

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************

// C�p
#include <assert.h>
#include <stdio.h>

// C++�p
#include <fstream>
#include <iomanip>
#include <iostream>
#include <list>
#include <sstream>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

// ���̑�
#include <windows.h>

// ����
#include "HayuLib.hpp"
#include "HayuLib.tpp"

// DirectX9
#include <d3dx9.h>					// DirectX9����p
#define DIRECTINPUT_VERSION	0x0800	// DirectInput�̃o�[�W����
#include <dinput.h>					// DirectInput����p
#include <xinput.h>					// �R���g���[���[����p
#include <xaudio2.h>				// �T�E���h����p

// JSON
#include <nlohmann/json.hpp>

// ImGui
#ifndef IMGUI_DEFINE_MATH_OPERATORS
#define IMGUI_DEFINE_MATH_OPERATORS
#endif // IMGUI_DEFINE_MATH_OPERATORS
#include <imgui.h>
#include <imgui_impl_dx9.h>
#include <imgui_impl_win32.h>

//****************************************************
// ���C�u�����̃����N
//****************************************************

// ����
#ifdef _DEBUG
#pragma comment(lib, "HayuLib_d.lib")
#else
#pragma comment(lib, "HayuLib_r.lib")
#endif // _DEBUG

#pragma comment(lib, "winmm.lib")
#pragma	comment(lib, "d3d9.lib")	// �`�揈���p
#pragma	comment(lib, "d3dx9.lib")	// d3d9.lib�̊g�����C�u����
#pragma	comment(lib, "dxguid.lib")	// DirectX�̃R�[�|�l���g�g�p�̂���
#pragma	comment(lib, "dinput8.lib")	// DirectInput����p
#pragma comment(lib, "xinput.lib")	// �R���g���[������p

//****************************************************
// �}�N����`
//****************************************************

// ���������[�N���o�p
#define _CRTDBG_MAP_ALLOC

#ifdef _DEBUG
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
// Replace _NORMAL_BLOCK with _CLIENT_BLOCK if you want the
// allocations to be of _CLIENT_BLOCK type
#else
#define DBG_NEW new
#endif

// ���_�t�H�[�}�b�g
#define FVF_VERTEX_2D	(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)
#define FVF_VERTEX_3D	(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)

//****************************************************
// �ÓI�萔���`
//****************************************************

// �X�N���[������
static const WORD	WSCREEN_WIDTH = 1280;
static const float	FSCREEN_WIDTH = 1280.0f;

// �X�N���[���c��
static const WORD	WSCREEN_HEIGHT = 720;
static const float	FSCREEN_HEIGHT = 720.0f;

//****************************************************
// using�f�B���N�e�B�u
//****************************************************
using JSON = nlohmann::json;

//****************************************************
// 2D�|���S�����_�t�H�[�}�b�g�̒�`
//****************************************************
struct VERTEX_2D
{
	D3DXVECTOR3 pos;	// ���_���W
	float		rhw;	// ���Z��
	D3DCOLOR	col;	// ���_�J���[
	D3DXVECTOR2 tex;	// �e�N�X�`�����W
};

//****************************************************
// 3D�|���S�����_�t�H�[�}�b�g�̒�`
//****************************************************
struct VERTEX_3D
{
	D3DXVECTOR3 pos;	// ���_���W
	D3DXVECTOR3 nor;	// �@���x�N�g��
	D3DCOLOR	col;	// ���_�J���[
	D3DXVECTOR2 tex;	// �e�N�X�`�����W
};

// (�������̃C���N���[�h���`���ς܂�����)
#include "useful.hpp"