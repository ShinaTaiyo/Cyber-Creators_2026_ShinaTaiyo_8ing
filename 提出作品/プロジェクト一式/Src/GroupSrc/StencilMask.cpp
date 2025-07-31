//============================================================================
// 
// ２０２５年６月２６日：ステンシルバッファを実装する [StencilMask.cpp]
// Author : 椎名太陽
// 
//============================================================================

//****************************************************
// インクルードファイル
//****************************************************
#include "StencilMask.h"
#include "bounding_box.h"
#include "renderer.h"

// 入力用
#include "manager.h"

// テクスチャ取得用
#include "texture_manager.h"

//****************************************************
// usingディレクティブ
//****************************************************
using namespace useful;

//****************************************************
// 静的メンバ変数
//****************************************************
bool CStencilMask::s_bStencil = true; // ステンシルバッファを使用するかどうか

//============================================================================
// 
// publicメンバ
// 
//============================================================================

//============================================================================
// コンストラクタ
//============================================================================
CStencilMask::CStencilMask() :
	CObject_2D{CObject::LAYER::STENCIL}
{
	SetType(CObject::TYPE::STENCILMASK); // タイプを「ステンシルマスク」に設定
}

//============================================================================
// デストラクタ
//============================================================================
CStencilMask::~CStencilMask()
{
	// 何もなし
}

//============================================================================
// 初期設定
//============================================================================
HRESULT CStencilMask::Init()
{
	// オブジェクト2Dクラスの初期設定
	if (FAILED(CObject_2D::Init()))
	{
		assert(false && "数字の初期設定に失敗");
	}

	return S_OK;
}

//============================================================================
// 終了処理
//============================================================================
void CStencilMask::Uninit()
{
	// オブジェクト2Dクラスの終了処理
	CObject_2D::Uninit();
}

//============================================================================
// 更新処理
//============================================================================
void CStencilMask::Update()
{
	// オブジェクト2Dクラスの更新
	CObject_2D::Update();
}

//============================================================================
// 描画処理
//============================================================================
void CStencilMask::Draw()
{
	// ステンシルバッファ描画設定をする
	SetStencilState();

	// ステンシルバッファを使用するなら全体にマスク用ポリゴンを描画しますt
	if (s_bStencil)
	{
		CObject_2D::Draw();
	}
	// ステンシルバッファ描画設定を元に戻す
	ResetStencilState();
}

//============================================================================
// 生成
//============================================================================
CStencilMask* CStencilMask::Create(D3DXVECTOR3 InitPos)
{
	// インスタンスを生成
	CStencilMask* pStencilMask = DBG_NEW CStencilMask();

	// 生成失敗
	if (pStencilMask == nullptr)
	{
		assert(false);
	}

	// タイプを設定
	pStencilMask->SetType(TYPE::STENCILMASK);

	// サイズ設定
	pStencilMask->SetSize(D3DXVECTOR3(FSCREEN_WIDTH, FSCREEN_HEIGHT, 0.0f));

	// 色合い設定
	pStencilMask->SetCol(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));

	// === その他初期設定 ===

	pStencilMask->Init(); // 初期化処理

	pStencilMask->SetPos(InitPos);// 初期位置設定

	return pStencilMask;
}

//============================================================================
//	他のオブジェクトがステンシルバッファの設定を行う（ステンシルテストがオンの場合、全てマスクされる）
//============================================================================
void CStencilMask::SetOtherStencilState()
{
	// ステンシルバッファを使用するなら設定する
	if (!s_bStencil)
	{
		return;
	}

	// デバイスを取得
	LPDIRECT3DDEVICE9 pDev = CRenderer::GetDeviece();

	// 戦略：まず描画部分は全て0x05でステンシル値を設定、その次に大きなポリゴン（0x03で設定し、
	//→３より大きければ描画されるので、モデルがあるとこだけポリゴンが描画されます。
	// Zバッファへの書き込みは普通に行います。ステンシル用ポリゴンそのものじゃないので奥行必要

	// ステンシルテストを有効
	pDev->SetRenderState(D3DRS_STENCILENABLE, TRUE);

	//※ステンシルテストは
	//・ステンシルマスク
	//・ステンシルバッファの値
	//・ステンシル参照値
	// この３要素を設定し、それらを比較関数で判定を行います。

	// ステンシル参照値の設定
	pDev->SetRenderState(D3DRS_STENCILREF, 0x05); // ステンシル参照値を0x05にする。

	// マスクの設定

	// ステンシルマスクの設定。（どのビットを書き換えるか制限）
	// 想像の仕方：布をかぶせ一部に穴が開いており、そこしか比較しない

	pDev->SetRenderState(D3DRS_STENCILMASK, 0xffffffff);

	// ステンシルマスクの設定。（どのビットで比較するか」を制限
	pDev->SetRenderState(D3DRS_STENCILWRITEMASK, 0xffffffff);

	// どんな条件が成立した時にステンシルバッファに書き込むのか？　の設定
	pDev->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_ALWAYS); // 必ずステンシルバッファに描きこませる、という設定。

	// 必ずステンシルテストに合格するのでこの設定はいらない
	{
		// 書き込む場合、どんな値を書き込むのか？の設定。
		pDev->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_INCR); // ステンシルテストに不合格の時・・・何もしない。

		// ステンシルテストに合格、深度テストに不合格の時・・・何もしない
		//→モデルが隠れた部分は深度テスト不合格になるのでこれで大丈夫！
		pDev->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_INCR);

	}

	// ステンシルテストと深度テストに合格の時・・・値をインクリメントします
	pDev->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_INCR);
}

//============================================================================
// マスクされたくないオブジェクトの描画処理で呼びます。
//============================================================================
void CStencilMask::SetOtherStencilAvoid()
{
	// ステンシルバッファを使用するなら設定する
	if (!s_bStencil)
	{
		return;
	}

	// デバイスを取得
	LPDIRECT3DDEVICE9 pDev = CRenderer::GetDeviece();

	// ステンシルテストを無効にする
	pDev->SetRenderState(D3DRS_STENCILENABLE,TRUE);

	// ステンシル参照値の設定
	pDev->SetRenderState(D3DRS_STENCILREF, 0x00); // ステンシル参照値を0x05にする。

	// マスクの設定

	// ステンシルマスクの設定。（どのビットを書き換えるか制限）
	// 想像の仕方：布をかぶせ一部に穴が開いており、そこしか比較しない

	pDev->SetRenderState(D3DRS_STENCILMASK, 0xffffffff);

	// ステンシルマスクの設定。（どのビットで比較するか」を制限
	pDev->SetRenderState(D3DRS_STENCILWRITEMASK, 0xffffffff);

	// どんな条件が成立した時にステンシルバッファに書き込むのか？　の設定
	pDev->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_ALWAYS); // 無条件でステンシルテストに合格

	// ステンシルテストに不合格の場合・・・ステンシル値を０にする
	pDev->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_REPLACE);

	//ステンシルテストに合格し、Zテストには不合格なら・・・ステンシル値を０にする
	pDev->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_REPLACE);

	// ステンシルテストと深度テストに合格の時・・・ステンシル値を０にする
	pDev->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_REPLACE);
}

//============================================================================
// 他のオブジェクトがステンシルバッファの設定を行う
//============================================================================
void CStencilMask::SetOtherResetStencilState()
{
	// ステンシルバッファを使用するなら設定する
	if (!s_bStencil)
	{
		return;
	}

	// デバイスを取得
	LPDIRECT3DDEVICE9 pDev = CRenderer::GetDeviece();

	// ステンシルテストを無効にする
	pDev->SetRenderState(D3DRS_STENCILENABLE, FALSE);

	// ステンシル参照値の設定
	pDev->SetRenderState(D3DRS_STENCILREF, 0x03); // ステンシル参照値を0x03にする。

	// ステンシルマスク設定（さっきと同じ)
	pDev->SetRenderState(D3DRS_STENCILMASK, 0xffffffff);

	// ステンシルマスクの設定。（どのビットで比較するか」を制限
	pDev->SetRenderState(D3DRS_STENCILWRITEMASK, 0xffffffff);

	// どんな条件が成立した時にステンシルバッファに書き込むのか？　の設定
	pDev->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_LESSEQUAL); // 必ずステンシルバッファに描きこませる、という設定。
}

//============================================================================
// 他のオブジェクトがステンシルバッファの設定を行う
//============================================================================
void CStencilMask::SetUseStencil(bool bUse)
{
	s_bStencil = bUse;
}

//============================================================================
// ステンシルバッファの設定を行う
//============================================================================
void CStencilMask::SetStencilState()
{
	// ステンシルバッファを使用するなら設定する
	if (!s_bStencil)
	{
		return;
	}

	// デバイスを取得
	LPDIRECT3DDEVICE9 pDev = CRenderer::GetDeviece();

	// 普通に奥行き使います。マスクの色を決めたいので
	pDev->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

	// Zバッファに登録しない（今までの深度情報と比べるが、自身は登録しない）
	pDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	// ステンシルテストを有効
	pDev->SetRenderState(D3DRS_STENCILENABLE, TRUE);

	//※ステンシルテストは
	//・ステンシルマスク
	//・ステンシルバッファの値
	//・ステンシル参照値
	// この３要素を設定し、それらを比較関数で判定を行います。

	// ステンシル参照値の設定
	pDev->SetRenderState(D3DRS_STENCILREF, 0x0A); // ステンシル参照値を0x03にする。

	// ステンシルマスクの設定。（どのビットを書き換えるか制限）
	// 想像の仕方：布をかぶせ一部に穴が開いており、そこしか比較しない

	pDev->SetRenderState(D3DRS_STENCILMASK, 0xffffffff);

	// ステンシルマスクの設定。（どのビットで比較するか」を制限
	//※マスク値を0xffffffffにすると比較の時の計算が簡略化されるみたい
	pDev->SetRenderState(D3DRS_STENCILWRITEMASK, 0xffffffff);

	// どんな条件が成立した時にステンシルバッファに書き込むのか？　の設定（１０＜１１）
	pDev->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_LESS); // "１０"がステンシル値以下なら描画

	// ステンシルテストに不合格の場合・・・何もしない
	pDev->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);

	//ステンシルテストに合格し、Zテストには不合格なら何もしない
	pDev->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);

	// ステンシルテストと深度テストに合格の時・・・何もしない。
	pDev->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_KEEP); 
}

//============================================================================
// ステンシルバッファの設定を元に戻す
//============================================================================
void CStencilMask::ResetStencilState()
{
	// ステンシルバッファを使用するなら設定する
	if (!s_bStencil)
	{
		return;
	}

	// デバイスを取得
	LPDIRECT3DDEVICE9 pDev = CRenderer::GetDeviece();

	// Zバッファへの書き込みを元に戻す
	pDev->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

	// Zバッファに登録する
	pDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	// ステンシルテストを無効にする
	pDev->SetRenderState(D3DRS_STENCILENABLE, FALSE);

	// ステンシル参照値の設定
	pDev->SetRenderState(D3DRS_STENCILREF, 0x0A); // ステンシル参照値を１０にする。

	// ステンシルマスク設定（さっきと同じ)
	pDev->SetRenderState(D3DRS_STENCILMASK, 0xffffffff);

	// ステンシルマスクの設定。（どのビットで比較するか」を制限
	pDev->SetRenderState(D3DRS_STENCILWRITEMASK, 0xffffffff);

	// どんな条件が成立した時にステンシルバッファに書き込むのか？　の設定
	pDev->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_LESSEQUAL); // 必ずステンシルバッファに描きこませる、という設定。
}
