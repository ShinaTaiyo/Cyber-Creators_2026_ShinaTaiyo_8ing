//============================================================================
// 
// ２０２５年５月１９日 [calculation.cpp]
// Author : 福田歩希
// 
////============================================================================

//****************************************************
// インクルードファイル
//****************************************************
#include "calculation.h" // 元クラス
#include "pch.h"         // プリコンパイル
#include "renderer.h"    // レンダラー
#include <string>
#include <locale>
#include <codecvt>

//****************************************************
// usingディレクティブ
//****************************************************
using namespace useful;

//****************************************************
// 距離名前空間関数
//****************************************************

//====================================================
// 点と点の距離を取る
//====================================================
float Calculation::Length::PointToPoint(D3DXVECTOR3 SelfPos, D3DXVECTOR3 OtherPos)
{
	// 計算
	float fLength = sqrtf(powf(OtherPos.x - SelfPos.x, 2) + powf(SelfPos.y - OtherPos.y, 2) + powf(SelfPos.z - OtherPos.z, 2));
	return fLength;
}

//====================================================
// １次元の点と点の距離を取る
//====================================================
float Calculation::Length::PointToPoint_1D(float fSelfPos, float fOtherPos)
{
	// 計算
	float fLength = sqrtf(powf(fOtherPos - fSelfPos, 2));
	return fLength;
}

//====================================================
// ２次元の点と点の距離を取る
//====================================================
float Calculation::Length::PointtoPoint_2D(D3DXVECTOR2 SelfPos, D3DXVECTOR3 OtherPos)
{
	// 計算
	float fLength = sqrtf(powf(OtherPos.x - SelfPos.x, 2) + powf(SelfPos.y - OtherPos.y, 2));
	return fLength;
}

//****************************************************
// 角度名前空間関数
//****************************************************

//====================================================
// 角度を調整する
//====================================================
void Calculation::Rot::Adjust(float & fSelfRot)
{
	if (fSelfRot > D3DX_PI)
	{
		fSelfRot -= D3DX_PI * 2;
	}
	else if (fSelfRot < -D3DX_PI)
	{
		fSelfRot += D3DX_PI * 2;
	}
}

//====================================================
// Yawを求める
//====================================================
float Calculation::Rot::Yaw(D3DXVECTOR3 SelfPos, D3DXVECTOR3 OtherPos)
{
	D3DXVECTOR3 Vec = OtherPos - SelfPos; // ベクトル
	return atan2f(Vec.x, Vec.z);
}

//====================================================
// Pitchを求める
//====================================================
float Calculation::Rot::Pitch(D3DXVECTOR3 SelfPos, D3DXVECTOR3 OtherPos)
{
	D3DXVECTOR3 Vec = OtherPos - SelfPos; // ベクトル
	return atan2f(Vec.y,sqrtf(powf(Vec.x,2) + powf(Vec.z,2)));
}

//====================================================
// 角度差を調整する
//====================================================
float Calculation::Rot::AdjustAngleDifference(float fSelfRot, float fOtherRot)
{
	// === 変数 ===

	float fRotDiff = fOtherRot - fSelfRot; // 角度差分

	// === 処理 ===

	// より近い角度差分へ調整
	if (fRotDiff > D3DX_PI)
	{
		fRotDiff -= D3DX_PI * 2;
	}
	else if (fRotDiff < -D3DX_PI)
	{
		fRotDiff += D3DX_PI * 2;
	}

	return fRotDiff;
}

//====================================================
// ワールド座標をスクリーン座標に変換する
//====================================================
D3DXVECTOR3 Calculation::Pos::WorldToScreen(D3DXVECTOR3 worldPos, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix, float screenWidth, float screenHeight)
{
	//ワールド座標をクリップ座標に変換
	D3DXVECTOR4 ClipSpacePos;
	D3DXMATRIX mtxTrans;

	mtxTrans = viewMatrix * projectionMatrix; // ビューポート、プロジェクション変換
	D3DXVec3Transform(&ClipSpacePos, &worldPos, &mtxTrans); // 上記の変換した時のワールド座標の位置を求める

	//透視除算（クリップ座標からNDC空間へ）（本来はGPUが勝手にやるが、データとしては出ないので自分で求める)
	if (ClipSpacePos.w != 0.0f)
	{//X、Y、Zは、カメラからの相対的な位置を表している（変換された位置)。
	 //Wは、カメラからの距離（深度情報）を表している
		ClipSpacePos.x /= ClipSpacePos.w;
		ClipSpacePos.y /= ClipSpacePos.w;
		ClipSpacePos.z /= ClipSpacePos.w;
	}

	//スクリーン座標へ変換
	D3DXVECTOR3 ScreenPos;
	ScreenPos.x = (ClipSpacePos.x * 0.5f + 0.5f) * screenWidth;           //真ん中を０にするため＋０．５ｆ、ー１なら左端、＋１なら右端となる
	ScreenPos.y = (1.0f - (ClipSpacePos.y * 0.5f + 0.5f)) * screenHeight; //真ん中を０にするため＋０．５ｆ、下が正となり、上が０となるので、１の時は０になる、ー１の時は１となる
	ScreenPos.z = ClipSpacePos.z;                                         //深度値（０～１）の範囲
	return ScreenPos;
}

//===================================================================================================================
// スクリーン座標をワールド座標に変換
//===================================================================================================================
D3DXVECTOR3* Calculation::Pos::ScreenToWorld(D3DXVECTOR3* pout, float Sx, float Sy, float fZ, int Screen_w, int Screen_h, D3DXMATRIX* View, D3DXMATRIX* Prj)
{
	// 各行列の逆行列を算出（ビュー、プロジェクションマトリックスの逆行列をかけるのは、カメラの位置に2DのUIが出ていると定義できるから)
	// 逆行列とは、値に値-1をかけ、掛け合わされる前に戻すこと
	D3DXMATRIX InvView, InvPrj, VP, InvViewport; // 計算用マトリックス

	//ビューマトリックスとの逆光列をかけてワールド座標を求める（ビューマトリックスの向きや位置に応じて変換されているので）
	D3DXMatrixInverse(&InvView, NULL, View);

	// プロジェクションマトリックスとの逆行列（見え方（平行投影、視野角など）を変えているので、逆行列を掛け合わせ、もとに戻す必要がある）
	D3DXMatrixInverse(&InvPrj, NULL, Prj);
	D3DXMatrixIdentity(&VP);

	// スケーリングの値を変えている。スクリーン座標の中心を画面中央にする（NDC座標を使用して変換するので、スクリーン座標は０～１２８０、なお、
	//NDC座標はー１～１だから、変換する際に合わせる必要がある(１２８０　＝　６４０）、（０　＝　ー６４０)
	VP._11 = Screen_w / 2.0f; VP._22 = -Screen_h / 2.0f; // スケーリング成分（Xそのまま、Yは上を正に）
	VP._41 = Screen_w / 2.0f; VP._42 = Screen_h / 2.0f;  // 平行移動成分（中心を原点に合わせる)
	D3DXMatrixInverse(&InvViewport, NULL, &VP); // NDC座標に合わせる

	//自分
	D3DXVECTOR3 MyPos = D3DXVECTOR3(Sx, Sy, fZ);//スクリーン座標と深度値

	// 逆変換
	D3DXMATRIX tmp = InvViewport * InvPrj * InvView;// ワールド座標を求める（InvViewport「スクリーン座標→NDC座標」、InvPrj「NDC座標→ビュー空間」、InvView「ビュー空間→ワールド座標」)
													//この行列に深度値とスクリーン座標を合わせることで、深度値に合わせた座標変換が可能になる)
	D3DXVec3TransformCoord(pout, &MyPos, &tmp); // 座標変換後の位置を求める
	return pout;
}

//****************************************************
// 数値名前空間関数
//****************************************************

//====================================================
// 配列外アクセスをしていないかチェックする
//====================================================
bool Calculation::Value::IsAccessSafeForArray(int nNum, int nMax)
{
	if (nNum < 0 || nNum >= nMax)
	{
		return false;
	}
	return true;
}

//====================================================
// 桁数を計算する
//====================================================
int Calculation::Value::NumDigit(int nValue)
{
	int nNumDivision = 0; // 桁数

	// 桁数計算処理
	while (nValue > 0)
	{
		nValue /= 10;    // 桁数を減らす
		nNumDivision++;  // 桁数カウント増加
	}

	return nNumDivision;
}

//====================================================
// 数値を指定範囲内に制限する
//====================================================
int Calculation::Value::LimitRange(int nValue, int nMin, int nMax)
{
	// 最低値を下回ったらその値に固定する
	if (nValue <= nMin)
	{
		nValue = nMin;
	}
	// 最大値を上回ったらその値に固定する
	if (nValue >= nMax)
	{
		nValue = nMax;
	}

	return nValue;
}

//====================================================
// ランダムなvector3を返す
//====================================================
D3DXVECTOR3 Calculation::Value::RandVector3(D3DXVECTOR3 Min, D3DXVECTOR3 Max)
{
	float fX = Min.x + static_cast<float>(rand()) / RAND_MAX * (Max.x - Min.x);
	float fY = Min.y + static_cast<float>(rand()) / RAND_MAX * (Max.y - Min.y);
	float fZ = Min.z + static_cast<float>(rand()) / RAND_MAX * (Max.z - Min.z);
	return D3DXVECTOR3(fX,fY,fZ);
}

//****************************************************
// 変換名前空間
//****************************************************

//====================================================
// StringをwStringに変換する
//====================================================
std::wstring Calculation::Convert::StrToWStr(std::string Str)
{
	int len = MultiByteToWideChar(CP_UTF8, 0, Str.c_str(), -1, nullptr, 0);
	std::wstring wstr(len, L'\0');
	MultiByteToWideChar(CP_UTF8, 0, Str.c_str(), -1, &wstr[0], len);
	// null 文字も入るので消す
	if (!wstr.empty() && wstr.back() == L'\0') {
		wstr.pop_back();
	}
	return wstr;
}

//====================================================
// StringをSjisに変換する
//====================================================
std::string Calculation::Convert::Utf8ToSjis(std::string Str)
{
	// UTF-8 → UTF-16（中間）
	int wlen = MultiByteToWideChar(CP_UTF8, 0, Str.c_str(), -1, NULL, 0);
	std::wstring wstr(wlen, L'\0');
	MultiByteToWideChar(CP_UTF8, 0, Str.c_str(), -1, &wstr[0], wlen);

	// UTF-16 → Shift-JIS
	int slen = WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, NULL, 0, NULL, NULL);
	std::string sjisStr(slen, '\0');
	WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, &sjisStr[0], slen, NULL, NULL);

	return sjisStr;
}
