//============================================================================
// 
// Vector系補間、ヘッダファイル [vector_interpolator.hpp]
// Author : 福田歩希
// 
//============================================================================

// インクルードガード
#pragma once

//****************************************************
// 名前空間usefulを定義
//****************************************************
namespace useful
{
	//****************************************************
	// Vector系補間処理分岐用フラグの列挙型を定義
	//****************************************************
	enum class VectorInterpolationFlags : unsigned char
	{
		None             = 0,
		UseInterpolation = 1 << 0,	// 補間処理自体の使用
		Linear           = 1 << 1,	// 線形補間
		Decay            = 1 << 2	// 指数減衰
	};

	//****************************************************
	// Vector系補間クラスを定義
	//****************************************************
	class CVectorInterpolator
	{
		//****************************************************
		// Vector系補間処理用情報の構造体を定義
		//****************************************************
		struct VectorInterpolationData
		{
			// Vector系メンバの数
			static constexpr unsigned char NUM_VECTOR_MEMBER = 3;

			// デフォルトコンストラクタ
			VectorInterpolationData() :
				ScaleTarget(VEC3_INIT),
				RotTarget(VEC3_INIT),
				PosTarget(VEC3_INIT),
				fRate{ 0.0f, 0.0f, 0.0f },
				Flags{ VectorInterpolationFlags::None, VectorInterpolationFlags::None, VectorInterpolationFlags::None }
			{}

			// 目標値
			Vec3
				ScaleTarget,	// 目標拡縮
				RotTarget,		// 目標向き
				PosTarget;		// 目標位置

			// 補間時設定
			float
				fRate[NUM_VECTOR_MEMBER];	// 割合
			VectorInterpolationFlags
				Flags[NUM_VECTOR_MEMBER];	// 処理分岐用フラグ
		};

	public:

		//****************************************************
		// Vector系メンバの引数用参照ラッパー
		//****************************************************
		struct VectorRefWrapper
		{
			// コンストラクタ
			VectorRefWrapper(Vec3& scale, Vec3& rot, Vec3& pos) :
				rScale(scale),
				rRot(rot),
				rPos(pos)
			{}

			// 目標値
			Vec3
				&rScale,	// 目標拡縮
				&rRot,		// 目標向き
				&rPos;		// 目標位置
		};

		//****************************************************
		// Vector系メンバの引数用コピーラッパー
		//****************************************************
		struct VectorCopyWrapper
		{
			// 目標値
			Vec3
				Scale,	// 目標拡縮
				Rot,	// 目標向き
				Pos;	// 目標位置
		};

		//****************************************************
		// function
		//****************************************************

		/**
		 * 数値の補間処理です、コピーが発生しませんが参照前提です
		 * 返り値無し
		 */
		void Interpolate(const VectorRefWrapper& wrapper);

		/**
		 * 数値の補間処理です、仮の実体で操作するため、忘れずに結果の代入をしてください
		 * 返り値は引数で受け取った数値に演算を行った結果
		 */
		VectorCopyWrapper Interpolate(const VectorCopyWrapper& wrapper);

		/**
		 * フラグの設定をしますが、内部で整合性を取るための判定を行います
		 * 返り値無し
		 */
		void SetFlags(unsigned char idx, VectorInterpolationFlags flags);

		// 目標縮尺の操作用
		inline		 Vec3& RefScaleTarget()					{ return m_Data.ScaleTarget; }
		inline const Vec3& GetScaleTarget() const			{ return m_Data.ScaleTarget; }
		inline		 void  SetScaleTarget(const Vec3& vec3) { m_Data.ScaleTarget = vec3; }

		// 目標向きの操作用
		inline		 Vec3& RefRotTarget()				  { return m_Data.RotTarget; }
		inline const Vec3& GetRotTarget() const			  { return m_Data.RotTarget; }
		inline		 void  SetRotTarget(const Vec3& vec3) { m_Data.RotTarget = vec3; }

		// 目標位置の操作用
		inline		 Vec3& RefPosTarget()				  { return m_Data.PosTarget; }
		inline const Vec3& GetPosTarget() const			  { return m_Data.PosTarget; }
		inline		 void  SetPosTarget(const Vec3& vec3) { m_Data.PosTarget = vec3; }

		// 割合の操作用
		inline float& RefRate(unsigned char idx)			 { return m_Data.fRate[idx]; }
		inline float  GetRate(unsigned char idx)			 { return m_Data.fRate[idx]; }
		inline void	  SetRate(unsigned char idx, float rate) { m_Data.fRate[idx] = rate; }

		// 処理分岐用フラグの操作用
		inline VectorInterpolationFlags& RefFlags(unsigned char idx) { return m_Data.Flags[idx]; }
		inline VectorInterpolationFlags	 GetFlags(unsigned char idx) { return m_Data.Flags[idx]; }

	private:

		//****************************************************
		// function
		//****************************************************
		void AdjustRateRange();	// 割合の範囲を制限

		//****************************************************
		// data
		//****************************************************
		VectorInterpolationData m_Data;	// 数値補間処理用情報
	};
}