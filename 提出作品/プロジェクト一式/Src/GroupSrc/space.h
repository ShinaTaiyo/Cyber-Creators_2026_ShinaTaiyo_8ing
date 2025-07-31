//============================================================================
// 
// 空間、ヘッダファイル [space.h]
// Author : 福田歩希
// 
//============================================================================

#pragma once

#include "object_Texture.h"

class CSpace
{
	static constexpr unsigned short int NUM_SPACE = 2;

	struct STATE
	{
		virtual bool Execute(CSpace*& space);
		virtual void Change(std::unique_ptr<STATE>& r);

		static bool UpdateState(CSpace*& space);

		int m_nCount = 0;
	};

	struct INTRO : public STATE
	{
		static constexpr int CNT_MAX = 1;

		bool Execute(CSpace*& space) override;
		void Change(std::unique_ptr<STATE>& r);
	};

	struct STOP : public STATE
	{
		static constexpr short int CNT_MAX = 30;

		bool Execute(CSpace*& space) override;
		void Change(std::unique_ptr<STATE>& r);
	};

	struct OUTRO : public STATE
	{
		static constexpr short int CNT_MAX = 60;

		bool Execute(CSpace*& space) override;
		void Change(std::unique_ptr<STATE>& r);
	};

public:

	CSpace();
	~CSpace() = default;

	void Start();
	void Verify();
	void Control();
	bool Update();

private:

	CObject_Texture* m_pSpace[NUM_SPACE];
	std::unique_ptr<STATE> m_upState;
};