//============================================================================
// 
// レジストリクラステンプレート、ヘッダファイル [registry.hpp]
// Author : 福田歩希
// 
//============================================================================

#pragma once

template <typename T> class CRegistry
{
public:

	CRegistry();
	~CRegistry();

	bool Initialize(const std::string& path, std::function<T(const std::string&)>&& fpCreate);
	void Finalize(std::function<void(T&)> fpRelease);

	T BindAtKey(const std::string& key);
	T BindAtPath(const std::string& path);
	T BindAtIndex(unsigned int index);
	//T& RefAtKey(const std::string& key);
	//T& RefAtIndex(unsigned int index);

	bool Register(const std::string& key, const std::string& path);

	inline std::map<std::string, T>& RefDynamicResource() { return m_mDynamicResource; }

private:

	std::function<T(const std::string&)> m_fpCreate;
	std::map<std::string, T> m_mDynamicResource;
	std::map<std::string, std::string> m_mPathMemory;
};

#include "registry.tpp"