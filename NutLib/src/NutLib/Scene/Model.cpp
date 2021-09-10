#include "nutpch.h"
#include "Model.h"

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

#include "NutLib/Renderer/Renderer.h"


namespace Nut
{

	Ref<Model> Model::Load(const std::string& filepath, Ref<Scene> scene, Ref<Shader> shader)
	{
		Assimp::Importer importer;

		const aiScene* aiscene = importer.ReadFile(filepath, aiProcess_OptimizeMeshes);

		if (!aiscene)
		{
			LOG_CORE_ERROR("Model::Load: Unable to load mesh file {0}", filepath.c_str());
			return nullptr;
		}

		auto shaderData = shader->GetShaderLayout();

		if (shaderData.find(ShaderLayoutDescriptor::Slot::Vertex) != shaderData.end())
		{
			LOG_CORE_TRACE("Model loader: loading vertices");
		}

		if (shaderData.find(ShaderLayoutDescriptor::Slot::TexCoord) != shaderData.end())
		{
			LOG_CORE_TRACE("Model loader: loading texture coordinates");
		}

		if (shaderData.find(ShaderLayoutDescriptor::Slot::Normal) != shaderData.end())
		{
			LOG_CORE_TRACE("Model loader: loading normals");
		}

		if (shaderData.find(ShaderLayoutDescriptor::Slot::Color) != shaderData.end())
		{
			LOG_CORE_TRACE("Model loader: loading colors");
		}

		Ref<Model> newModel = CreateRef<Model>(scene);
		Entity::GetComponent<TagComponent>(newModel->ID()).Tag = aiscene->mName.C_Str();
		Entity::AddComponent<TransformComponent>(newModel->m_ID);

		return newModel;
//		return CreateRef<Model>(scene, aiscene->mMeshes[0]->mName.C_Str());
	}



	Model::Model(Ref<Scene> scene, const std::string& tag)
		: Object(scene, tag)
	{

	}

	Model::~Model()
	{

	}



}
