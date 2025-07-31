//============================================================================
// 
// Vector�n��ԁA�w�b�_�t�@�C�� [vector_interpolator.hpp]
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
	// Vector�n��ԏ�������p�t���O�̗񋓌^���`
	//****************************************************
	enum class VectorInterpolationFlags : unsigned char
	{
		None             = 0,
		UseInterpolation = 1 << 0,	// ��ԏ������̂̎g�p
		Linear           = 1 << 1,	// ���`���
		Decay            = 1 << 2	// �w������
	};

	//****************************************************
	// Vector�n��ԃN���X���`
	//****************************************************
	class CVectorInterpolator
	{
		//****************************************************
		// Vector�n��ԏ����p���̍\���̂��`
		//****************************************************
		struct VectorInterpolationData
		{
			// Vector�n�����o�̐�
			static constexpr unsigned char NUM_VECTOR_MEMBER = 3;

			// �f�t�H���g�R���X�g���N�^
			VectorInterpolationData() :
				ScaleTarget(VEC3_INIT),
				RotTarget(VEC3_INIT),
				PosTarget(VEC3_INIT),
				fRate{ 0.0f, 0.0f, 0.0f },
				Flags{ VectorInterpolationFlags::None, VectorInterpolationFlags::None, VectorInterpolationFlags::None }
			{}

			// �ڕW�l
			Vec3
				ScaleTarget,	// �ڕW�g�k
				RotTarget,		// �ڕW����
				PosTarget;		// �ڕW�ʒu

			// ��Ԏ��ݒ�
			float
				fRate[NUM_VECTOR_MEMBER];	// ����
			VectorInterpolationFlags
				Flags[NUM_VECTOR_MEMBER];	// ��������p�t���O
		};

	public:

		//****************************************************
		// Vector�n�����o�̈����p�Q�ƃ��b�p�[
		//****************************************************
		struct VectorRefWrapper
		{
			// �R���X�g���N�^
			VectorRefWrapper(Vec3& scale, Vec3& rot, Vec3& pos) :
				rScale(scale),
				rRot(rot),
				rPos(pos)
			{}

			// �ڕW�l
			Vec3
				&rScale,	// �ڕW�g�k
				&rRot,		// �ڕW����
				&rPos;		// �ڕW�ʒu
		};

		//****************************************************
		// Vector�n�����o�̈����p�R�s�[���b�p�[
		//****************************************************
		struct VectorCopyWrapper
		{
			// �ڕW�l
			Vec3
				Scale,	// �ڕW�g�k
				Rot,	// �ڕW����
				Pos;	// �ڕW�ʒu
		};

		//****************************************************
		// function
		//****************************************************

		/**
		 * ���l�̕�ԏ����ł��A�R�s�[���������܂��񂪎Q�ƑO��ł�
		 * �Ԃ�l����
		 */
		void Interpolate(const VectorRefWrapper& wrapper);

		/**
		 * ���l�̕�ԏ����ł��A���̎��̂ő��삷�邽�߁A�Y�ꂸ�Ɍ��ʂ̑�������Ă�������
		 * �Ԃ�l�͈����Ŏ󂯎�������l�ɉ��Z���s��������
		 */
		VectorCopyWrapper Interpolate(const VectorCopyWrapper& wrapper);

		/**
		 * �t���O�̐ݒ�����܂����A�����Ő���������邽�߂̔�����s���܂�
		 * �Ԃ�l����
		 */
		void SetFlags(unsigned char idx, VectorInterpolationFlags flags);

		// �ڕW�k�ڂ̑���p
		inline		 Vec3& RefScaleTarget()					{ return m_Data.ScaleTarget; }
		inline const Vec3& GetScaleTarget() const			{ return m_Data.ScaleTarget; }
		inline		 void  SetScaleTarget(const Vec3& vec3) { m_Data.ScaleTarget = vec3; }

		// �ڕW�����̑���p
		inline		 Vec3& RefRotTarget()				  { return m_Data.RotTarget; }
		inline const Vec3& GetRotTarget() const			  { return m_Data.RotTarget; }
		inline		 void  SetRotTarget(const Vec3& vec3) { m_Data.RotTarget = vec3; }

		// �ڕW�ʒu�̑���p
		inline		 Vec3& RefPosTarget()				  { return m_Data.PosTarget; }
		inline const Vec3& GetPosTarget() const			  { return m_Data.PosTarget; }
		inline		 void  SetPosTarget(const Vec3& vec3) { m_Data.PosTarget = vec3; }

		// �����̑���p
		inline float& RefRate(unsigned char idx)			 { return m_Data.fRate[idx]; }
		inline float  GetRate(unsigned char idx)			 { return m_Data.fRate[idx]; }
		inline void	  SetRate(unsigned char idx, float rate) { m_Data.fRate[idx] = rate; }

		// ��������p�t���O�̑���p
		inline VectorInterpolationFlags& RefFlags(unsigned char idx) { return m_Data.Flags[idx]; }
		inline VectorInterpolationFlags	 GetFlags(unsigned char idx) { return m_Data.Flags[idx]; }

	private:

		//****************************************************
		// function
		//****************************************************
		void AdjustRateRange();	// �����͈̔͂𐧌�

		//****************************************************
		// data
		//****************************************************
		VectorInterpolationData m_Data;	// ���l��ԏ����p���
	};
}