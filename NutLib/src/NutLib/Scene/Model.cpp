#include "nutpch.h"
#include "Model.h"

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

#include "NutLib/Renderer/Renderer.h"


namespace Nut
{

	Ref<Model> Model::Load(const std::string& filepath, Ref<Scene> scene, Ref<Pipeline> pipeline)
	{
		Assimp::Importer importer;

		const aiScene* aiscene = importer.ReadFile(filepath, aiProcess_OptimizeMeshes);

		Ref<Model> newModel = CreateRef<Model>(scene);
		Entity::GetComponent<TagComponent>(newModel->ID()).Tag = aiscene->mName.C_Str();
		Entity::AddComponent<TransformComponent>(newModel->m_ID);

		Entity::AddComponent<MeshComponent>(newModel->m_ID);
		auto& meshAsset = Entity::GetComponent<MeshComponent>(newModel->m_ID).Mesh;
		meshAsset = CreateRef<MeshAsset>(pipeline, scene);

		if (!aiscene)
		{
			LOG_CORE_ERROR("Model::Load: Unable to load mesh file {0}", filepath.c_str());
			return nullptr;
		}

		auto shader = pipeline->GetShader();

		if (aiscene->HasMeshes())
		{
			for (uint32_t i = 0; i < aiscene->mNumMeshes; i++)
			{
				DataBuffer<ShaderLayoutItem> dataBuffer(aiscene->mMeshes[i]->mNumVertices, shader->GetShaderLayout());

				for (auto& item : shader->GetShaderLayout().Items())
				{

					if (item.Slot == ShaderLayoutItem::ShaderSlot::Vertex)
					{
						LOG_CORE_TRACE("Model loader: loading vertices");
						for (uint32_t j = 0; j < aiscene->mMeshes[i]->mNumVertices; j++)
						{
							auto& vertex = aiscene->mMeshes[i]->mVertices[j];
							dataBuffer.SetPosition(ShaderLayoutItem::ShaderSlot::Vertex, j, glm::vec3(vertex.x, vertex.y, vertex.z));

						}

					}

					if (item.Slot == ShaderLayoutItem::ShaderSlot::TexCoord)
					{

						if (aiscene->mMeshes[i]->HasTextureCoords(0))
						{
							LOG_CORE_TRACE("Model loader: loading texture coordinates");

							for (uint32_t j = 0; j < aiscene->mMeshes[i]->mNumVertices; j++)
							{
								auto& textureCoord = aiscene->mMeshes[i]->mTextureCoords[j];
//								dataBuffer.SetPosition(ShaderLayoutItem::ShaderSlot::TexCoord, j, glm::vec2(textureCoord->x, textureCoord->y));
							}
						}
						else
						{
							for (uint32_t j = 0; j < aiscene->mMeshes[i]->mNumVertices; j++)
							{
								auto& textureCoord = aiscene->mMeshes[i]->mTextureCoords[j];
								dataBuffer.SetPosition(ShaderLayoutItem::ShaderSlot::TexCoord, j, glm::vec2(0.0f, 0.0f));
							}
						}
					
					}

					if (item.Slot == ShaderLayoutItem::ShaderSlot::Normal)
					{

						if (aiscene->mMeshes[i]->HasNormals())
						{
							LOG_CORE_TRACE("Model loader: loading normal coordinates");

							for (uint32_t j = 0; j < aiscene->mMeshes[i]->mNumVertices; j++)
							{
								auto& normal = aiscene->mMeshes[i]->mNormals[j];
								dataBuffer.SetPosition(ShaderLayoutItem::ShaderSlot::Normal, j, glm::vec3(normal.x, normal.y, normal.z));

							}
						}
					}

					if (item.Slot == ShaderLayoutItem::ShaderSlot::Color)
					{
						LOG_CORE_TRACE("Model loader: loading colors");
						
						for (uint32_t j = 0; j < aiscene->mMeshes[i]->mNumVertices; j++)
						{
							auto& color = aiscene->mMeshes[i]->mColors[j];
							dataBuffer.SetPosition(ShaderLayoutItem::ShaderSlot::Color, j, glm::vec4(color->r, color->g, color->b, color->a));
						}
					}
				}

				std::vector<uint32_t> indexBuffer;

				for (uint32_t j = 0; j < aiscene->mMeshes[i]->mNumFaces; j++)
				{
					for (uint32_t k = 0; k < aiscene->mMeshes[i]->mFaces[j].mNumIndices; k++)
					{
						indexBuffer.push_back(aiscene->mMeshes[i]->mFaces[j].mIndices[k]);
					}
				}

				LOG_CORE_TRACE("Added submesh: {0}", aiscene->mMeshes[i]->mName.C_Str());
				meshAsset->AddSubmesh(dataBuffer, indexBuffer); // = CreateRef<MeshAsset>(newModel->m_ID, (DataBuffer<ShaderLayoutItem>(vertices.data(), 4, m_BasicShader->GetShaderLayout()), indices, m_BasicPipeline, m_Scene);

			}

		}





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
