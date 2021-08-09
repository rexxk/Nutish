#pragma once

#include "Shader.h"


namespace Nut
{

	class ShaderStore
	{
	public:
		static void Add(const std::string& name, Ref<Shader> shader);

		static Ref<Shader> Get(const std::string& name);

		static std::vector<std::string> List();

	private:
		static inline ShaderStore* s_Instance = nullptr;

		std::unordered_map<std::string, Ref<Shader>> m_ShaderStore;

	};

}
