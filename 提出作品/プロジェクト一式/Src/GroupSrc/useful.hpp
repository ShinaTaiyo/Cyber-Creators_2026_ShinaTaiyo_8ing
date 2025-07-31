//============================================================================
// 
// ���[�X�t���A�w�b�_�t�@�C�� [useful.hpp]
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
	// using�錾
	//****************************************************
	using std::unique_ptr;
	using std::make_unique;
	using std::to_string;

	//****************************************************
	// �^�G�C���A�X���`
	//****************************************************
	template <typename T> using up = unique_ptr<T>;
	using XCol = D3DXCOLOR;
	using Mtx = D3DXMATRIX;
	using Vec2 = D3DXVECTOR2;
	using Vec3 = D3DXVECTOR3;

	//****************************************************
	// �ÓI�萔���`
	//****************************************************
	static const XCol XCOL_INIT = XCol(1.0f, 1.0f, 1.0f, 1.0f);	// D3DXCOLOR�������p
	static const Vec2 VEC2_INIT = Vec2(0.0f, 0.0f);				// D3DXVECTOR2�������p
	static const Vec3 VEC3_INIT = Vec3(0.0f, 0.0f, 0.0f);		// D3DXVECTOR3�������p
	static const Vec3 SCALE_INIT = Vec3(1.0f, 1.0f, 1.0f);		// Scale�������p

	/* �R���o�[�^�[��p */
	static constexpr const char* FRONT_PATH_FOR_CONVERTER = "Data\\MODEL";

	//****************************************************
	// ���C���p�\���̂��`
	//****************************************************
	struct RAY
	{
		// �f�t�H���g�R���X�g���N�^
		RAY() :
			Origin(VEC3_INIT),
			Direction(VEC3_INIT)
		{}

		Vec3 Origin;	// ���C�̎n�_
		Vec3 Direction;	// ���C�̕���
	};

	//****************************************************
	// �C�����C���֐��e���v���[�g���`
	//****************************************************

	// '~' ���Z�q�̃I�[�o�[���[�h
	template <typename T, typename U = unsigned char> inline T operator~(T l)
	{
		return static_cast<T>(~static_cast<U>(l));
	}

	// '&' ���Z�q�̃I�[�o�[���[�h
	template <typename T, typename U = unsigned char> inline T operator&(T l, T r)
	{
		return static_cast<T>(static_cast<U>(l) & static_cast<U>(r));
	}

	// '|' ���Z�q�̃I�[�o�[���[�h
	template <typename T, typename U = unsigned char> inline T operator|(T l, T r)
	{
		return static_cast<T>(static_cast<U>(l) | static_cast<U>(r));
	}

	/**
	 * �_���ς̔�r���̏ȗ��`�ł��A�񋓎q��'None'�̒�`���O��ł�
	 * 
	 * flags -> 'None'�Ɣ�r�������t���O
	 */
	template <typename T> inline bool AndCheck(T flags)
	{
		return flags != T::None;
	}

	/**
	 * �ėp�����̂݁A��O�����Ȃǂ͈�؍s���܂���
	 * �Ԃ�l����
	 *
	 * up      -> ������̃|�C���^�̎Q��
	 * factory -> �t�@�N�g�����\�b�h
	 */
	template <typename T, typename U> inline void Compose(std::unique_ptr<T>& up, U&& factory)
	{
		up = std::forward<U>(factory)();
	}

	/**
	 * ���`���
	 * �Ԃ�l�͖���̌v�Z����
	 *
	 * base   -> ���ݒl
	 * target -> �ڕW�l
	 * rate   -> �����̋��x
	 */
	template <typename T> inline T Lerp(const T& base, const T& target, float rate)
	{
		return base + (target - base) * rate;
	}

	/**
	 * �w������
	 * �Ԃ�l����
	 *
	 * base   -> ���ݒl
	 * target -> �ڕW�l
	 * coef   -> �����̋��x
	 */
	template <typename T> inline void ExponentialDecay(T& base, T target, float coef)
	{
		base += (target - base) * coef;
	}

	//****************************************************
	// �֐��e���v���[�g���`
	//****************************************************

	/**
	 * �ėp�����A�O��Ƃ��� 'Inirialzie()' �� 'Release()' �������o�Ɏ��N���X����
	 * �Ԃ�l�̓C���X�^���X�̃|�C���^
	 */
	template <typename T> T* Create();

	/**
	 * �ėp���j�[�N�|�C���^�����A�O��Ƃ��� 'Inirialzie()' �� 'Release()' �������o�Ɏ��N���X����
	 * �e���v���[�g�����Ɏw�肳�ꂽ�^�̃f�t�H���g�R���X�g���N�^�Ő������܂�
	 * �Ԃ�l�̓C���X�^���X�̃��j�[�N�|�C���^
	 */
	template <typename T> up<T> CreateUniquePtr();

	/**
	 * �ėp����A�O��Ƃ��� 'Release()' �������o�Ɏ��N���X����
	 * �Ԃ�l����
	 *
	 * p -> ����������C���X�^���X���Q��
	 */
	template <typename T> void Release(T*& p);

	/**
	 * unique_ptr�^�̒��g���A�e���v���[�g�����Ɏw�肳�ꂽ�^�̃f�t�H���g�R���X�g���N�^��make_unique����������
	 * �Ԃ�l����
	 *
	 * uptr -> ����������ΏہA����ѐV�K�C���X�^���X�̊i�[���unique_ptr
	 */
	template <typename T, typename U> void ChangeUniquePtr(up<T>& uptr);

	/**
	 * �I�u�W�F�N�g�̌�����Yaw��]
	 * �Ԃ�l����
	 *
	 * base -> 'GetRot()' �� 'SetRot()' �������o�Ɏ��I�u�W�F�N�g����
	 * coef -> ���Z�����]�ʁA�������]�������w��
	 */
	template <typename T> void RotateYawAxis(T& base, float coef);

	/**
	 * �I�u�W�F�N�g�̖ڕW������Yaw��]
	 * �Ԃ�l����
	 *
	 * base -> 'GetRotTarget()' �� 'SetRotTarget()' �������o�Ɏ��I�u�W�F�N�g����
	 * coef -> ���Z�����]�ʁA�������]�������w��
	 */
	template <typename T> void RotateTargetYawAxis(T& base, float coef);

	//****************************************************
	// �v���g�^�C�v�錾
	//****************************************************

	/**
	 * ���_�t�H�[�}�b�g�̎��e�N�X�`���}�b�s���O�������������܂�
	 * �Ԃ�l����
	 *
	 * refVtx -> ���_�t�H�[�}�b�g�̃|�C���^�Q�ƁA�K�����b�N���ꂽ����̃|�C���^��n���Ă�������
	 */
	void InitializeTextureMappingInVertex(VERTEX_2D*& refVtx);
	void InitializeTextureMappingInVertex(VERTEX_3D*& refVtx);

	/**
	 * ���[�p�A�s�b�`�p�̐����␳���s���܂�
	 * �Ԃ�l����
	 *
	 * vec3 -> ������������]�x�N�g��
	 */
	//void AdjustDirectionYawPitch(Vec3& vec3);

	/**
	 * ���[���h�s��̌v�Z�A�����Ń[���w�肷��Ƃ��̗v�f�𖳎����܂�
	 * �Ԃ�l����
	 *
	 * mtx   -> �s����Q��
	 * scale -> �g�k
	 * rot   -> ����
	 * pos   -> ���W
	 */
	void UpdateMtxWorld(Mtx& mtx, const Vec3& scale, const Vec3& rot, const Vec3& pos);

	/**
	 * ���^���̏����O���[�v���g�p�\�Ȃ��̂ɕϊ����܂�
	 * �Ԃ�l����
	 *
	 * json -> �ϊ������������O���[�v�̃I�u�W�F�N�g�P�ʂ̎Q��
	 */
	void OldModelPathConverterForSummonPettern(JSON& json);

	//****************************************************
	// ���O���MIS���`
	//****************************************************
	namespace MIS
	{
		/* ImGui���g�p����ۂ̎��������V���[�g�J�b�g�ł� */

		/**
		 * ImGui��Window�𐶐��A�K��'ImGui::End()'�ƃZ�b�g�Ŏg�p���邱��
		 * �Ԃ�l�̓E�B���h�E�����̉�
		 */
		bool MyImGuiShortcut_BeginWindow(const char* name);
		bool MyImGuiShortcut_BeginWindow(const char* name, const ImVec2& rect);
		bool MyImGuiShortcut_BeginWindow(const char* name, const ImVec2& rect, const ImVec2& pos);
		bool MyImGuiShortcut_BeginWindow(const char* name, const ImVec2& rect, const ImVec2& pos, ImGuiWindowFlags flags);

		/**
		 * �{�^���̃E�B�W�F�b�g���A�Y���s�̎c��̕��ɋϓ��ɕ~���l�߂邽�߂̃T�C�Y���Z�o���܂�
		 * �K���E�l�߂ɂȂ�܂��A�����̍��ɐ������L�ڂ���ۂ͐�s���ăe�L�X�g��z�u���邱��
		 * �Ԃ�l�̓{�^��1�̃T�C�Y
		 */
		ImVec2 CalcButtonSpacing(unsigned short num);
		ImVec2 CalcButtonSpacing(unsigned short num, float height);

		/**
		 * Vector�n�\���̂̃h���b�O����
		 * �Ԃ�l����
		 */
		void MyImGuiShortcut_DragVector(const char* label, Vec2& vec2, float speed);
		void MyImGuiShortcut_DragVector(const char* label, Vec3& vec3, float speed);

		/**
		 * Vector�n�\���̂̃��Z�b�g����
		 */
		void MyImGuiShortcut_ResetVector(const char* label, Vec2& vec2);
		void MyImGuiShortcut_ResetVector(const char* label, Vec2& vec2, float initVal);
		void MyImGuiShortcut_ResetVector(const char* label, Vec3& vec3);
		void MyImGuiShortcut_ResetVector(const char* label, Vec3& vec3, float initVal);
	}
}

//****************************************************
// �C���N���[�h�t�@�C��
//****************************************************

// ���̖��O��ԂɊ܂ޒǉ����
#include "vector_interpolator.hpp"
#include "texture_animator.hpp"

// �e���v���[�g��`�t�@�C��
#include "useful.tpp"