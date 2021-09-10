#pragma once

#include "Object.h"
#include "NutLib/Renderer/Shader.h"
#include "NutLib/Renderer/Mesh.h"

namespace Nut
{


	class Model : public Object
	{
	public:
		static Ref<Model> Load(const std::string& filepath, Ref<Scene> scene, Ref<Shader> shader);

		Model(Ref<Scene> scene, const std::string& tag = "model");
		virtual ~Model();

		uint32_t MeshCount() const { return static_cast<uint32_t>(m_Meshes.size()); }

		std::string Name() const { return m_Name; }

	private:
		UUID m_ID;

		std::vector<Mesh> m_Meshes;
		std::string m_Name;

	};


}
