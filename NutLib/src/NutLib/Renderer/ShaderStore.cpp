#include "nutpch.h"
#include "ShaderStore.h"


namespace Nut
{




	void ShaderStore::Add(const std::string& name, Ref<Shader> shader)
	{
		if (!s_Instance)
			s_Instance = new ShaderStore;

		s_Instance->m_ShaderStore[name] = shader;
	}

	Ref<Shader> ShaderStore::Get(const std::string& name)
	{
		if (s_Instance)
		{
			if (s_Instance->m_ShaderStore.find(name) != s_Instance->m_ShaderStore.end())
				return s_Instance->m_ShaderStore[name];
		}

		return nullptr;
	}

	std::vector<std::string> ShaderStore::List()
	{
		std::vector<std::string> shaderList;

		for (auto& shader : s_Instance->m_ShaderStore)
		{
			shaderList.emplace_back(shader.first);
		}

		return shaderList;
	}

}
