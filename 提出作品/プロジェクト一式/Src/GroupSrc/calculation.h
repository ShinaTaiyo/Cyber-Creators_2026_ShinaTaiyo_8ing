//============================================================================
// 
// ２０２５年５月１９日、計算用関数 [calculation.h]
// Author : 福田歩希
// 
//============================================================================

#ifndef _CALCULATION_H_
#define _CALCULATION_H_	// 二重インクルード防止

//****************************************************
// 名前空間Calculationを定義
//****************************************************
namespace Calculation
{
	//****************************************************
	// usingディレクティブ
	//****************************************************
	using namespace useful;

	// 距離名前空間
	namespace Length
	{
		// === 関数 ===

	    float PointToPoint(D3DXVECTOR3 SelfPos, D3DXVECTOR3 OtherPos); // 点と点の距離を取る
		float PointToPoint_1D(float fSelfPos, float fOtherPos); // １次元の距離を取る
		float PointtoPoint_2D(D3DXVECTOR2 SelfPos, D3DXVECTOR3 OtherPos); // ２次元の距離を取る
	}

	// 角度名前空間
	namespace Rot
	{
		// === 関数 ===

		void Adjust(float & fSelfRot); // 角度を調整
		float Yaw(D3DXVECTOR3 SelfPos, D3DXVECTOR3 OtherPos);   // Yawを求める
		float Pitch(D3DXVECTOR3 SelfPos, D3DXVECTOR3 OtherPos); // Pitchを求める
		float AdjustAngleDifference(float fSelfRot, float fOtherRot); // 角度差を調整
	}

	// 数値
	namespace Value
	{
		// === 関数 ===

		bool IsAccessSafeForArray(int nNum,int nMax); // 配列外アクセスをしていないかチェックする
		int NumDigit(int nValue); // 桁数を計算する
		int LimitRange(int nValue, int nMin, int nMax); // 数値を指定範囲内に制限する
		D3DXVECTOR3 RandVector3(D3DXVECTOR3 Min, D3DXVECTOR3 Max); // ランダムなvector3を返す
	}

	namespace Convert
	{
		std::wstring StrToWStr(std::string Str);
		std::string Utf8ToSjis(std::string Str);
	}

	// 位置名前空間
	namespace Pos
	{
		// === 関数 ===

		// ワールド座標をスクリーン座標に変換
		D3DXVECTOR3 WorldToScreen(
			D3DXVECTOR3 worldPos, // ワールド座標
			D3DXMATRIX viewMatrix, // ビューマトリックス
			D3DXMATRIX projectionMatrix, // プロジェクションマトリックス
			float screenWidth, // スクリーン横幅
			float screenHeight // スクリーン高さ
		);

		// スクリーン座標をワールド座標に変換
		D3DXVECTOR3* ScreenToWorld(D3DXVECTOR3* pout,
			float Sx,  // スクリーンX座標
			float Sy,  // スクリーンY座標
			float fZ,  // 射影空間でのZ値（0～1）
			int Screen_w, // スクリーン横幅
			int Screen_h, // スクリーン高さ
			D3DXMATRIX* View, // ビューマトリックス
			D3DXMATRIX* Prj   // プロジェクションマトリックス
		);

	}
};
#endif