//============================================================================
// 
// Vector系補間 [vector_interpolator.cpp]
// Author : 福田歩希
// 
//============================================================================

//****************************************************
// インクルードファイル
//****************************************************
#include "vector_interpolator.hpp"

//****************************************************
// usingディレクティブ
//****************************************************
using namespace useful;

//============================================================================
// 補間処理 - 参照
//============================================================================
void useful::CVectorInterpolator::Interpolate(const VectorRefWrapper& wrapper)
{
	// Vector系メンバを配列にまとめる
	std::array<Vec3, VectorInterpolationData::NUM_VECTOR_MEMBER> aTmpMember =
	{
		m_Data.ScaleTarget,
		m_Data.RotTarget,
		m_Data.PosTarget
	};

	// 引数も同様にまとめる
	std::array<Vec3*, VectorInterpolationData::NUM_VECTOR_MEMBER> aTmpArg =
	{
		&wrapper.rScale,
		&wrapper.rRot,
		&wrapper.rPos
	};

	// 割合の範囲を制限
	AdjustRateRange();

	// それぞれフラグ設定に応じて補間を行う
	for (unsigned char ucIdx = 0; ucIdx < VectorInterpolationData::NUM_VECTOR_MEMBER; ++ucIdx)
	{
		// 補間処理を使用しない
		if (!AndCheck(m_Data.Flags[ucIdx] & VectorInterpolationFlags::UseInterpolation))
		{
			continue;
		}

		// 線形補間を行う
		if (AndCheck(m_Data.Flags[ucIdx] & VectorInterpolationFlags::Linear))
		{
			*aTmpArg[ucIdx] = Lerp(*aTmpArg[ucIdx], aTmpMember[ucIdx], m_Data.fRate[ucIdx]);
		}

		// 指数減衰を行う
		if (AndCheck(m_Data.Flags[ucIdx] & VectorInterpolationFlags::Decay))
		{
			ExponentialDecay(*aTmpArg[ucIdx], aTmpMember[ucIdx], m_Data.fRate[ucIdx]);
		}
	}
}

//============================================================================
// 補間処理 - コピー
//============================================================================
useful::CVectorInterpolator::VectorCopyWrapper useful::CVectorInterpolator::Interpolate(const VectorCopyWrapper& wrapper)
{
	// Vector系メンバを配列にまとめる
	std::array<Vec3, VectorInterpolationData::NUM_VECTOR_MEMBER> aTmpMember =
	{
		m_Data.ScaleTarget,
		m_Data.RotTarget,
		m_Data.PosTarget
	};

	// 引数も同様にまとめる
	decltype(aTmpMember) aTmpArg =
	{
		wrapper.Scale,
		wrapper.Rot,
		wrapper.Pos
	};

	// 割合の範囲を制限
	AdjustRateRange();

	// それぞれの設定に応じて補間を行う
	for (unsigned char ucIdx = 0; ucIdx < VectorInterpolationData::NUM_VECTOR_MEMBER; ++ucIdx)
	{
		// 補間処理を使用しない
		if (!AndCheck(m_Data.Flags[ucIdx] & VectorInterpolationFlags::UseInterpolation))
		{
			continue;
		}

		// 線形補間を行う
		if (AndCheck(m_Data.Flags[ucIdx] & VectorInterpolationFlags::Linear))
		{
			aTmpArg[ucIdx] = Lerp(aTmpArg[ucIdx], aTmpMember[ucIdx], m_Data.fRate[ucIdx]);
		}

		// 指数減衰を行う
		if (AndCheck(m_Data.Flags[ucIdx] & VectorInterpolationFlags::Decay))
		{
			ExponentialDecay(aTmpArg[ucIdx], aTmpMember[ucIdx], m_Data.fRate[ucIdx]);
		}
	}

	// 演算結果用ラッパー
	VectorCopyWrapper Result = { VEC3_INIT, VEC3_INIT, VEC3_INIT };

	Result.Scale = aTmpArg[0];
	Result.Rot = aTmpArg[1];
	Result.Pos = aTmpArg[2];

	return Result;
}

//============================================================================
// フラグの設定
//============================================================================
void useful::CVectorInterpolator::SetFlags(unsigned char idx, VectorInterpolationFlags flags)
{
	// フラグの型エイリアスを定義
	using VIF = VectorInterpolationFlags;

	// 一旦、元のフラグと指定したフラグをまとめて立てる
	VIF NewFlags = m_Data.Flags[idx] | flags;

	// 元のフラグと、新しいフラグで指定した種類が競合してしまう場合
	if (AndCheck(NewFlags & VIF::Linear) && AndCheck(NewFlags & VIF::Decay))
	{
		// 新しく指定したフラグを優先して立てておく
		if (AndCheck(flags & VIF::Linear))
		{
			NewFlags = NewFlags & ~VIF::Decay;
		}
		else if (AndCheck(flags & VIF::Decay))
		{
			NewFlags = NewFlags & ~VIF::Linear;
		}
	}

	// フラグを設定
	m_Data.Flags[idx] = NewFlags;
}

//============================================================================
// 割合の範囲を制限
//============================================================================
void useful::CVectorInterpolator::AdjustRateRange()
{
	for (unsigned char ucIdx = 0; ucIdx < VectorInterpolationData::NUM_VECTOR_MEMBER; ++ucIdx)
	{
		if (m_Data.fRate[ucIdx] > 1.0f)
		{
			m_Data.fRate[ucIdx] = 1.0f;
		}

		if (m_Data.fRate[ucIdx] < 0.0f)
		{
			m_Data.fRate[ucIdx] = 0.0f;
		}
	}
}