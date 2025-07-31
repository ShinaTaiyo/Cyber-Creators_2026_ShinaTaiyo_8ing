//============================================================================
// 
// レジストリクラステンプレート、定義用ファイル [registry.tpp]
// Author : 福田歩希
// 
//============================================================================

#pragma once

#include "registry.hpp"

template <typename T> CRegistry<T>::CRegistry() :
	m_fpCreate(nullptr),
	m_mDynamicResource{},
	m_mPathMemory{}
{}

template <typename T> CRegistry<T>::~CRegistry()
{}

template <typename T> bool CRegistry<T>::Initialize(const std::string& path, std::function<T(const std::string&)>&& fpCreate)
{
	m_fpCreate = std::move(fpCreate);

	//const JSON& Json = hws::SafeOpenJsonFile("Data\\JSON\\INITIALIZE\\" + static_cast<std::string>(typeid(T).raw_name()) + ".json");
	const JSON& Json = hws::SafeOpenJsonFile(path);

	decltype(Json["element"].size()) MaxSize = Json["element"].size();

	for (decltype(MaxSize) CntIdx = 0; CntIdx < MaxSize; ++CntIdx)
	{
		m_mDynamicResource.insert(std::make_pair(Json["element"][CntIdx][0], m_fpCreate(Json["element"][CntIdx][1])));
		m_mPathMemory.insert(std::make_pair(Json["element"][CntIdx][1], Json["element"][CntIdx][0]));
	}
	
	return true;
}

template <typename T> void CRegistry<T>::Finalize(std::function<void(T&)> fpRelease)
{
	for (auto& it : m_mDynamicResource)
	{
		fpRelease(it.second);
	}

	m_mDynamicResource.clear();
}

template <typename T> T CRegistry<T>::BindAtKey(const std::string& key)
{
	const decltype(m_mDynamicResource)::iterator& it = m_mDynamicResource.find(key);

	if (it == m_mDynamicResource.end())
	{
		throw std::runtime_error("'BindAtKey' was Failed : " + key);
	}

	return it->second;
}

template <typename T> T CRegistry<T>::BindAtPath(const std::string& path)
{
	const decltype(m_mPathMemory)::iterator& it_key = m_mPathMemory.find(path);

	if (it_key == m_mPathMemory.end())
	{
		throw std::runtime_error("'BindAtPath was Failed : " + path);
	}

	const std::string& Key = m_mPathMemory.at(path);

	return 	BindAtKey(Key);
}

template <typename T> T CRegistry<T>::BindAtIndex(unsigned int index)
{
	unsigned int unElementIdx = 0;

	for (const auto& it : m_mDynamicResource)
	{
		if (index == unElementIdx)
		{
			return it.second;
		}

		++unElementIdx;
	}

	throw std::runtime_error("'BindAtIndex' was Failed" + index);
}

//template <typename T> T& CRegistry<T>::RefAtKey(const std::string& key)
//{
//const decltype(m_mDynamicResource)::iterator& it = m_mDynamicResource.find(key);
//
//if (it == m_mDynamicResource.end())
//{
//	throw std::runtime_error("'RefAtKey' was Failed");
//}
//
//return it->second;
//}
//
//template <typename T> T& CRegistry<T>::RefAtIndex(unsigned int index)
//{
//	unsigned int unElementIdx = 0;
//
//	for (const auto& pair : m_mDynamicResource)
//	{
//		if (index == unElementIdx)
//		{
//			return pair;
//		}
//
//		++unElementIdx;
//	}
//
//	throw std::runtime_error("'RefAtIndex' was Failed");
//}

template <typename T> bool CRegistry<T>::Register(const std::string& key, const std::string& path)
{
	const decltype(m_mDynamicResource)::iterator& it = m_mDynamicResource.find(key);

	if (it != m_mDynamicResource.end())
	{
		return false;
	}

	m_mDynamicResource.insert(std::make_pair(key, m_fpCreate(path)));

	return true;
}