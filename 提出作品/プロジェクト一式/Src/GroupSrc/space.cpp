//============================================================================
// 
// ãÛä‘ [space.cpp]
// Author : ïüìcï‡äÛ
// 
//============================================================================

#include "space.h"
#include "sound.h"

namespace
{
	static constexpr float
		Y_INITIALIZE = 18.0f,
		Z_INITIALIZE = 313.0f;

	float
		Y_Axis_Offset = Y_INITIALIZE,
		Z_Axis_Distance = Z_INITIALIZE;
}

using namespace useful;

bool CSpace::STATE::Execute(CSpace*& space)
{
	throw std::runtime_error("'CSpace::STATE::Execute()' was Failed : " + static_cast<std::string>(typeid(space).name()));
}

void CSpace::STATE::Change(std::unique_ptr<STATE>& r)
{
	throw std::runtime_error("'CSpace::STATE::Change()' was Failed : " + static_cast<std::string>(typeid(r).name()));
}

bool CSpace::STATE::UpdateState(CSpace*& space)
{
	up<STATE>& r = space->m_upState;

	if (!r)
	{
		return false;
	}

	++r->m_nCount;

	if (r->Execute(space))
	{
		r->Change(r);
	}

	return true;
}

bool CSpace::INTRO::Execute(CSpace*& space)
{
	space->m_pSpace[0]->SetPos(
		{ 0.0f, Y_Axis_Offset, -Z_Axis_Distance });

	space->m_pSpace[0]->SetPosTarget(
		{ 0.0f, Y_Axis_Offset, -Z_Axis_Distance });

	space->m_pSpace[1]->SetPos(
		{ 0.0f, -Y_Axis_Offset, -Z_Axis_Distance });

	space->m_pSpace[1]->SetPosTarget(
		{ 0.0f, -Y_Axis_Offset, -Z_Axis_Distance });

	if (m_nCount >= INTRO::CNT_MAX)
	{
		return true;
	}

	return false;
}

void CSpace::INTRO::Change(std::unique_ptr<STATE>& r)
{
	CSound::GetInstance()->Play(CSound::LABEL::GAKIN);
	ChangeUniquePtr<STATE, STOP>(r);
}

bool CSpace::STOP::Execute(CSpace*& space)
{
	if (m_nCount <= 10)
	{

	}
	if (m_nCount > 10 && m_nCount <= 20)
	{
		space->m_pSpace[0]->SetPosTarget(
			{ 0.0f, 3.0f + Y_Axis_Offset, -Z_Axis_Distance });

		space->m_pSpace[0]->SetPosTarget(
			{ 0.0f, 3.0f + Y_Axis_Offset, -Z_Axis_Distance });

		space->m_pSpace[1]->SetPosTarget(
			{ 0.0f, -3.0f + -Y_Axis_Offset, -Z_Axis_Distance });

		space->m_pSpace[1]->SetPosTarget(
			{ 0.0f, -3.0f + -Y_Axis_Offset, -Z_Axis_Distance });
	}
	else if (m_nCount >= STOP::CNT_MAX)
	{
		return true;
	}

	return false;
}

void CSpace::STOP::Change(std::unique_ptr<STATE>& r)
{
	ChangeUniquePtr<STATE, OUTRO>(r);
}

bool CSpace::OUTRO::Execute(CSpace*& space)
{
	space->m_pSpace[0]->SetPosTarget(
		{ 200.0f, Y_Axis_Offset, -Z_Axis_Distance });

	space->m_pSpace[1]->SetPosTarget(
		{ -200.0f, -Y_Axis_Offset, -Z_Axis_Distance });

	if (m_nCount >= OUTRO::CNT_MAX)
	{
		return true;
	}

	return false;
}

void CSpace::OUTRO::Change(std::unique_ptr<STATE>& r)
{
	// Ç®ÇµÇ‹Ç¢
	if (r)
	{
		r.reset();
	}
}

CSpace::CSpace() :
	m_pSpace{ nullptr, nullptr },
	m_upState(nullptr)
{
	if (!m_pSpace[0])
	{
		const JSON& ParamTexture0 = hws::SafeOpenJsonFile("Data\\JSON\\OBJECT\\TEXTURE\\Test0.json");
		m_pSpace[0] = CObject_Texture::Create(ParamTexture0);
	}

	if (!m_pSpace[1])
	{
		const JSON& ParamTexture1 = hws::SafeOpenJsonFile("Data\\JSON\\OBJECT\\TEXTURE\\Test1.json");
		m_pSpace[1] = CObject_Texture::Create(ParamTexture1);
	}

	Verify();
}

void CSpace::Start()
{
	if (m_upState)
	{
		m_upState.reset();
	}

	m_upState = make_unique<INTRO>();
}

void CSpace::Verify()
{
	//for (unsigned short int wCntLoop = 0; wCntLoop < NUM_SPACE; ++wCntLoop)
	//{
	//	if (!m_pSpace[wCntLoop])
	//	{
	//		throw std::runtime_error("'CSpace::Verify()' was Failed of : " + wCntLoop);
	//	}
	//}

	//if (!m_upState)
	//{
	//	throw std::runtime_error("CSpace::Verify()' was Failed : " + static_cast<std::string>(typeid(m_upState).name()));
	//}
}

void CSpace::Control()
{
	if (ImGui::Begin(u8"ãÛä‘ÇÃëÄçÏ"))
	{
		ImGui::DragFloat("Y", &Y_Axis_Offset, 1.0f);
		ImGui::DragFloat("Z", &Z_Axis_Distance, 1.0f);
	}

	ImGui::End();
}

bool CSpace::Update()
{
#ifdef _DEBUG
	Verify();
#if 0 
	Control();
#endif
#endif // _DEBUG

	decltype(this) pSpace = this;

	return STATE::UpdateState(pSpace);
}