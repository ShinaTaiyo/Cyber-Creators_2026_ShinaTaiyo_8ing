//============================================================================
// 
// ユースフル、ヘッダファイル [useful.hpp]
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
	// using宣言
	//****************************************************
	using std::unique_ptr;
	using std::make_unique;
	using std::to_string;

	//****************************************************
	// 型エイリアスを定義
	//****************************************************
	template <typename T> using up = unique_ptr<T>;
	using XCol = D3DXCOLOR;
	using Mtx = D3DXMATRIX;
	using Vec2 = D3DXVECTOR2;
	using Vec3 = D3DXVECTOR3;

	//****************************************************
	// 静的定数を定義
	//****************************************************
	static const XCol XCOL_INIT = XCol(1.0f, 1.0f, 1.0f, 1.0f);	// D3DXCOLOR初期化用
	static const Vec2 VEC2_INIT = Vec2(0.0f, 0.0f);				// D3DXVECTOR2初期化用
	static const Vec3 VEC3_INIT = Vec3(0.0f, 0.0f, 0.0f);		// D3DXVECTOR3初期化用
	static const Vec3 SCALE_INIT = Vec3(1.0f, 1.0f, 1.0f);		// Scale初期化用

	/* コンバーター専用 */
	static constexpr const char* FRONT_PATH_FOR_CONVERTER = "Data\\MODEL";

	//****************************************************
	// レイ情報用構造体を定義
	//****************************************************
	struct RAY
	{
		// デフォルトコンストラクタ
		RAY() :
			Origin(VEC3_INIT),
			Direction(VEC3_INIT)
		{}

		Vec3 Origin;	// レイの始点
		Vec3 Direction;	// レイの方向
	};

	//****************************************************
	// インライン関数テンプレートを定義
	//****************************************************

	// '~' 演算子のオーバーロード
	template <typename T, typename U = unsigned char> inline T operator~(T l)
	{
		return static_cast<T>(~static_cast<U>(l));
	}

	// '&' 演算子のオーバーロード
	template <typename T, typename U = unsigned char> inline T operator&(T l, T r)
	{
		return static_cast<T>(static_cast<U>(l) & static_cast<U>(r));
	}

	// '|' 演算子のオーバーロード
	template <typename T, typename U = unsigned char> inline T operator|(T l, T r)
	{
		return static_cast<T>(static_cast<U>(l) | static_cast<U>(r));
	}

	/**
	 * 論理積の比較文の省略形です、列挙子に'None'の定義が前提です
	 * 
	 * flags -> 'None'と比較したいフラグ
	 */
	template <typename T> inline bool AndCheck(T flags)
	{
		return flags != T::None;
	}

	/**
	 * 汎用合成のみ、例外処理などは一切行いません
	 * 返り値無し
	 *
	 * up      -> 合成先のポインタの参照
	 * factory -> ファクトリメソッド
	 */
	template <typename T, typename U> inline void Compose(std::unique_ptr<T>& up, U&& factory)
	{
		up = std::forward<U>(factory)();
	}

	/**
	 * 線形補間
	 * 返り値は毎回の計算結果
	 *
	 * base   -> 現在値
	 * target -> 目標値
	 * rate   -> 減衰の強度
	 */
	template <typename T> inline T Lerp(const T& base, const T& target, float rate)
	{
		return base + (target - base) * rate;
	}

	/**
	 * 指数減衰
	 * 返り値無し
	 *
	 * base   -> 現在値
	 * target -> 目標値
	 * coef   -> 減衰の強度
	 */
	template <typename T> inline void ExponentialDecay(T& base, T target, float coef)
	{
		base += (target - base) * coef;
	}

	//****************************************************
	// 関数テンプレートを定義
	//****************************************************

	/**
	 * 汎用生成、前提として 'Inirialzie()' と 'Release()' をメンバに持つクラス向け
	 * 返り値はインスタンスのポインタ
	 */
	template <typename T> T* Create();

	/**
	 * 汎用ユニークポインタ生成、前提として 'Inirialzie()' と 'Release()' をメンバに持つクラス向け
	 * テンプレート引数に指定された型のデフォルトコンストラクタで生成します
	 * 返り値はインスタンスのユニークポインタ
	 */
	template <typename T> up<T> CreateUniquePtr();

	/**
	 * 汎用解放、前提として 'Release()' をメンバに持つクラス向け
	 * 返り値無し
	 *
	 * p -> 解放したいインスタンスを参照
	 */
	template <typename T> void Release(T*& p);

	/**
	 * unique_ptr型の中身を、テンプレート引数に指定された型のデフォルトコンストラクタでmake_uniqueし交換する
	 * 返り値無し
	 *
	 * uptr -> メモリ解放対象、および新規インスタンスの格納先のunique_ptr
	 */
	template <typename T, typename U> void ChangeUniquePtr(up<T>& uptr);

	/**
	 * オブジェクトの向きをYaw回転
	 * 返り値無し
	 *
	 * base -> 'GetRot()' と 'SetRot()' をメンバに持つオブジェクト限定
	 * coef -> 加算する回転量、速さや回転方向を指定
	 */
	template <typename T> void RotateYawAxis(T& base, float coef);

	/**
	 * オブジェクトの目標向きをYaw回転
	 * 返り値無し
	 *
	 * base -> 'GetRotTarget()' と 'SetRotTarget()' をメンバに持つオブジェクト限定
	 * coef -> 加算する回転量、速さや回転方向を指定
	 */
	template <typename T> void RotateTargetYawAxis(T& base, float coef);

	//****************************************************
	// プロトタイプ宣言
	//****************************************************

	/**
	 * 頂点フォーマットの持つテクスチャマッピング情報を初期化します
	 * 返り値無し
	 *
	 * refVtx -> 頂点フォーマットのポインタ参照、必ずロックされた直後のポインタを渡してください
	 */
	void InitializeTextureMappingInVertex(VERTEX_2D*& refVtx);
	void InitializeTextureMappingInVertex(VERTEX_3D*& refVtx);

	/**
	 * ヨー角、ピッチ角の制限補正を行います
	 * 返り値無し
	 *
	 * vec3 -> 制限したい回転ベクトル
	 */
	//void AdjustDirectionYawPitch(Vec3& vec3);

	/**
	 * ワールド行列の計算、引数でゼロ指定するとその要素を無視します
	 * 返り値無し
	 *
	 * mtx   -> 行列を参照
	 * scale -> 拡縮
	 * rot   -> 向き
	 * pos   -> 座標
	 */
	void UpdateMtxWorld(Mtx& mtx, const Vec3& scale, const Vec3& rot, const Vec3& pos);

	/**
	 * 旧型式の召喚グループを使用可能なものに変換します
	 * 返り値無し
	 *
	 * json -> 変換したい召喚グループのオブジェクト単位の参照
	 */
	void OldModelPathConverterForSummonPettern(JSON& json);

	//****************************************************
	// 名前空間MISを定義
	//****************************************************
	namespace MIS
	{
		/* ImGuiを使用する際の自分向けショートカットです */

		/**
		 * ImGuiのWindowを生成、必ず'ImGui::End()'とセットで使用すること
		 * 返り値はウィンドウ生成の可否
		 */
		bool MyImGuiShortcut_BeginWindow(const char* name);
		bool MyImGuiShortcut_BeginWindow(const char* name, const ImVec2& rect);
		bool MyImGuiShortcut_BeginWindow(const char* name, const ImVec2& rect, const ImVec2& pos);
		bool MyImGuiShortcut_BeginWindow(const char* name, const ImVec2& rect, const ImVec2& pos, ImGuiWindowFlags flags);

		/**
		 * ボタンのウィジェットを、該当行の残りの幅に均等に敷き詰めるためのサイズを算出します
		 * 必ず右詰めになります、これらの左に説明を記載する際は先行してテキストを配置すること
		 * 返り値はボタン1つのサイズ
		 */
		ImVec2 CalcButtonSpacing(unsigned short num);
		ImVec2 CalcButtonSpacing(unsigned short num, float height);

		/**
		 * Vector系構造体のドラッグ操作
		 * 返り値無し
		 */
		void MyImGuiShortcut_DragVector(const char* label, Vec2& vec2, float speed);
		void MyImGuiShortcut_DragVector(const char* label, Vec3& vec3, float speed);

		/**
		 * Vector系構造体のリセット操作
		 */
		void MyImGuiShortcut_ResetVector(const char* label, Vec2& vec2);
		void MyImGuiShortcut_ResetVector(const char* label, Vec2& vec2, float initVal);
		void MyImGuiShortcut_ResetVector(const char* label, Vec3& vec3);
		void MyImGuiShortcut_ResetVector(const char* label, Vec3& vec3, float initVal);
	}
}

//****************************************************
// インクルードファイル
//****************************************************

// この名前空間に含む追加情報
#include "vector_interpolator.hpp"
#include "texture_animator.hpp"

// テンプレート定義ファイル
#include "useful.tpp"