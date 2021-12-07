#pragma once

#include "NutLib/Renderer/Pipeline.h"
#include "NutLib/Renderer/Mesh.h"

#include "NutLib/Scene/Entity.h"

namespace Nut
{


	class Model
	{
	public:
		static Ref<Model> Load(const std::string& filepath, Ref<Scene> scene, Ref<Pipeline> pipeline);

		Model(Ref<Scene> scene, const std::string& tag = "model");
		virtual ~Model();

		uint32_t MeshCount() const { return static_cast<uint32_t>(m_Meshes.size()); }

		std::string Name() const { return m_Name; }

		Entity GetEntity() const { return m_Entity; }

	private:
		Entity m_Entity;

		std::vector<Mesh> m_Meshes;
		std::string m_Name;

	};


}
