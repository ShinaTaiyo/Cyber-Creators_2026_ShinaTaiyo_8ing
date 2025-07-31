//============================================================================
// 
// Vector�n��� [vector_interpolator.cpp]
// Author : ���c����
// 
//============================================================================

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************
#include "vector_interpolator.hpp"

//****************************************************
// using�f�B���N�e�B�u
//****************************************************
using namespace useful;

//============================================================================
// ��ԏ��� - �Q��
//============================================================================
void useful::CVectorInterpolator::Interpolate(const VectorRefWrapper& wrapper)
{
	// Vector�n�����o��z��ɂ܂Ƃ߂�
	std::array<Vec3, VectorInterpolationData::NUM_VECTOR_MEMBER> aTmpMember =
	{
		m_Data.ScaleTarget,
		m_Data.RotTarget,
		m_Data.PosTarget
	};

	// ���������l�ɂ܂Ƃ߂�
	std::array<Vec3*, VectorInterpolationData::NUM_VECTOR_MEMBER> aTmpArg =
	{
		&wrapper.rScale,
		&wrapper.rRot,
		&wrapper.rPos
	};

	// �����͈̔͂𐧌�
	AdjustRateRange();

	// ���ꂼ��t���O�ݒ�ɉ����ĕ�Ԃ��s��
	for (unsigned char ucIdx = 0; ucIdx < VectorInterpolationData::NUM_VECTOR_MEMBER; ++ucIdx)
	{
		// ��ԏ������g�p���Ȃ�
		if (!AndCheck(m_Data.Flags[ucIdx] & VectorInterpolationFlags::UseInterpolation))
		{
			continue;
		}

		// ���`��Ԃ��s��
		if (AndCheck(m_Data.Flags[ucIdx] & VectorInterpolationFlags::Linear))
		{
			*aTmpArg[ucIdx] = Lerp(*aTmpArg[ucIdx], aTmpMember[ucIdx], m_Data.fRate[ucIdx]);
		}

		// �w���������s��
		if (AndCheck(m_Data.Flags[ucIdx] & VectorInterpolationFlags::Decay))
		{
			ExponentialDecay(*aTmpArg[ucIdx], aTmpMember[ucIdx], m_Data.fRate[ucIdx]);
		}
	}
}

//============================================================================
// ��ԏ��� - �R�s�[
//============================================================================
useful::CVectorInterpolator::VectorCopyWrapper useful::CVectorInterpolator::Interpolate(const VectorCopyWrapper& wrapper)
{
	// Vector�n�����o��z��ɂ܂Ƃ߂�
	std::array<Vec3, VectorInterpolationData::NUM_VECTOR_MEMBER> aTmpMember =
	{
		m_Data.ScaleTarget,
		m_Data.RotTarget,
		m_Data.PosTarget
	};

	// ���������l�ɂ܂Ƃ߂�
	decltype(aTmpMember) aTmpArg =
	{
		wrapper.Scale,
		wrapper.Rot,
		wrapper.Pos
	};

	// �����͈̔͂𐧌�
	AdjustRateRange();

	// ���ꂼ��̐ݒ�ɉ����ĕ�Ԃ��s��
	for (unsigned char ucIdx = 0; ucIdx < VectorInterpolationData::NUM_VECTOR_MEMBER; ++ucIdx)
	{
		// ��ԏ������g�p���Ȃ�
		if (!AndCheck(m_Data.Flags[ucIdx] & VectorInterpolationFlags::UseInterpolation))
		{
			continue;
		}

		// ���`��Ԃ��s��
		if (AndCheck(m_Data.Flags[ucIdx] & VectorInterpolationFlags::Linear))
		{
			aTmpArg[ucIdx] = Lerp(aTmpArg[ucIdx], aTmpMember[ucIdx], m_Data.fRate[ucIdx]);
		}

		// �w���������s��
		if (AndCheck(m_Data.Flags[ucIdx] & VectorInterpolationFlags::Decay))
		{
			ExponentialDecay(aTmpArg[ucIdx], aTmpMember[ucIdx], m_Data.fRate[ucIdx]);
		}
	}

	// ���Z���ʗp���b�p�[
	VectorCopyWrapper Result = { VEC3_INIT, VEC3_INIT, VEC3_INIT };

	Result.Scale = aTmpArg[0];
	Result.Rot = aTmpArg[1];
	Result.Pos = aTmpArg[2];

	return Result;
}

//============================================================================
// �t���O�̐ݒ�
//============================================================================
void useful::CVectorInterpolator::SetFlags(unsigned char idx, VectorInterpolationFlags flags)
{
	// �t���O�̌^�G�C���A�X���`
	using VIF = VectorInterpolationFlags;

	// ��U�A���̃t���O�Ǝw�肵���t���O���܂Ƃ߂ė��Ă�
	VIF NewFlags = m_Data.Flags[idx] | flags;

	// ���̃t���O�ƁA�V�����t���O�Ŏw�肵����ނ��������Ă��܂��ꍇ
	if (AndCheck(NewFlags & VIF::Linear) && AndCheck(NewFlags & VIF::Decay))
	{
		// �V�����w�肵���t���O��D�悵�ė��ĂĂ���
		if (AndCheck(flags & VIF::Linear))
		{
			NewFlags = NewFlags & ~VIF::Decay;
		}
		else if (AndCheck(flags & VIF::Decay))
		{
			NewFlags = NewFlags & ~VIF::Linear;
		}
	}

	// �t���O��ݒ�
	m_Data.Flags[idx] = NewFlags;
}

//============================================================================
// �����͈̔͂𐧌�
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