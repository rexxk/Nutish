#include "nutpch.h"
#include "Model.h"

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

#include "NutLib/Renderer/Renderer.h"


namespace Nut
{


	std::string aiLightTypeToString(aiLightSourceType type)
	{
		switch (type)
		{
			case aiLightSource_AMBIENT: return "ambient";
			case aiLightSource_SPOT: return "spot";
			case aiLightSource_AREA: return "area";
			case aiLightSource_DIRECTIONAL: return "directional";
			case aiLightSource_POINT: return "point";
		}
		return "undefined";
	}



	Entity Model::Load(const std::string& filepath, Ref<Scene> scene, Ref<Pipeline> pipeline)
	{
		Assimp::Importer importer;

		const aiScene* aiscene = importer.ReadFile(filepath, aiProcess_OptimizeMeshes | aiProcess_Triangulate | aiProcess_TransformUVCoords | aiProcess_EmbedTextures);

//		Ref<Model> newModel = CreateRef<Model>(scene);

		Entity entity(scene->GetRegistry(), "Model"); // = newModel->GetEntity();
		ECS::EntitySystem::GetComponent<TagComponent>(entity.ID()).Tag = aiscene->mName.C_Str();
		entity.AddComponent<TransformComponent>();
		entity.AddComponent<MeshComponent>();

		auto& meshAsset = ECS::EntitySystem::GetComponent<MeshComponent>(entity.ID()).Mesh;
		meshAsset = CreateRef<MeshAsset>(pipeline, scene);

		if (!aiscene)
		{
			LOG_CORE_ERROR("Model::Load: Unable to load mesh file {0}", filepath.c_str());
			return Entity();
		}

		auto shader = pipeline->GetShader();

		// Load meshes

		if (aiscene->HasMeshes())
		{
			for (uint32_t i = 0; i < aiscene->mNumMeshes; i++)
			{
				aiMesh* aimesh = aiscene->mMeshes[i];

				DataBuffer<ShaderLayoutItem> dataBuffer(aiscene->mMeshes[i]->mNumVertices, shader->GetShaderLayout());

				for (auto& item : shader->GetShaderLayout().Items())
				{

					if (item.Slot == ShaderLayoutItem::ShaderSlot::Vertex)
					{
//						LOG_CORE_TRACE("Model loader: loading vertices (num vertices: {0})", aiscene->mMeshes[i]->mNumVertices);
						for (uint32_t j = 0; j < aimesh->mNumVertices; j++)
						{
							auto& vertex = aimesh->mVertices[j];
							dataBuffer.SetPosition(ShaderLayoutItem::ShaderSlot::Vertex, j, glm::vec3(vertex.x, vertex.y, vertex.z));
						}
					}

					if (item.Slot == ShaderLayoutItem::ShaderSlot::TexCoord)
					{

						if (aiscene->mMeshes[i]->HasTextureCoords(0))
						{
//							LOG_CORE_TRACE("Model loader: loading texture coordinates");

							for (uint32_t j = 0; j < aimesh->mNumVertices; j++)
							{
								auto& textureCoord = aimesh->mTextureCoords[0];
								dataBuffer.SetPosition(ShaderLayoutItem::ShaderSlot::TexCoord, j, glm::vec2(textureCoord->x, textureCoord->y));
							}
						}
						else
						{
							for (uint32_t j = 0; j < aimesh->mNumVertices; j++)
							{
								auto& textureCoord = aimesh->mTextureCoords[j];
								dataBuffer.SetPosition(ShaderLayoutItem::ShaderSlot::TexCoord, j, glm::vec2(0.0f, 0.0f));
							}
						}
					
					}

					if (item.Slot == ShaderLayoutItem::ShaderSlot::Normal)
					{

						if (aiscene->mMeshes[i]->HasNormals())
						{
//							LOG_CORE_TRACE("Model loader: loading normal coordinates");

							for (uint32_t j = 0; j < aimesh->mNumVertices; j++)
							{
								auto& normal = aimesh->mNormals[j];
								dataBuffer.SetPosition(ShaderLayoutItem::ShaderSlot::Normal, j, glm::vec3(normal.x, normal.y, normal.z));

							}
						}
					}

					if (item.Slot == ShaderLayoutItem::ShaderSlot::Color)
					{
//						LOG_CORE_TRACE("Model loader: loading colors");
						
						for (uint32_t j = 0; j < aimesh->mNumVertices; j++)
						{
							auto& color = aimesh->mColors[j];
							dataBuffer.SetPosition(ShaderLayoutItem::ShaderSlot::Color, j, glm::vec4(color->r, color->g, color->b, color->a));
						}
					}
				}

				std::vector<uint32_t> indexBuffer;

				for (uint32_t j = 0; j < aimesh->mNumFaces; j++)
				{
					for (uint32_t k = 0; k < aimesh->mFaces[j].mNumIndices; k++)
					{
						indexBuffer.push_back(aimesh->mFaces[j].mIndices[k]);
					}
				}

//				LOG_CORE_TRACE("Added submesh: {0}", aiscene->mMeshes[i]->mName.C_Str());
				meshAsset->AddSubmesh(dataBuffer, indexBuffer); // = CreateRef<MeshAsset>(newModel->m_ID, (DataBuffer<ShaderLayoutItem>(vertices.data(), 4, m_BasicShader->GetShaderLayout()), indices, m_BasicPipeline, m_Scene);
			}
		}

		// Load materials

		if (aiscene->HasMaterials())
		{
			std::vector<aiMaterial> materials;

			for (uint32_t i = 0; i < aiscene->mNumMaterials; i++)
			{
				aiMaterial* material = aiscene->mMaterials[i];

				for (uint32_t j = 0; j < material->mNumProperties; j++)
				{
					auto prop = material->mProperties[j];

					LOG_CORE_TRACE("Loading material {0}: {1}  (prop: {2})", i, material->GetName().C_Str(), prop->mKey.C_Str());

					if (prop->mKey == aiString("$clr.diffuse"))
					{
						aiColor3D diffuse;
						material->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse);
						LOG_CORE_TRACE("Found diffuse color: {0},{1},{2}", diffuse.r, diffuse.g, diffuse.b);
					}
					if (prop->mKey == aiString("$clr.emissive"))
					{
						aiColor3D emissive;
						material->Get(AI_MATKEY_COLOR_EMISSIVE, emissive);
						LOG_CORE_TRACE("Found emissive color: {0},{1},{2}", emissive.r, emissive.g, emissive.b);
					}
					if (prop->mKey == aiString("$clr.ambient"))
					{
						aiColor3D ambient;
						material->Get(AI_MATKEY_COLOR_AMBIENT, ambient);
						LOG_CORE_TRACE("Found ambient color: {0},{1},{2}", ambient.r, ambient.g, ambient.b);
					}
					if (prop->mKey == aiString("$clr.transparent"))
					{
						aiColor3D transparent;
						material->Get(AI_MATKEY_COLOR_TRANSPARENT, transparent);
						LOG_CORE_TRACE("Found transparent color: {0},{1},{2}", transparent.r, transparent.g, transparent.b);
					}
					if (prop->mKey == aiString("$raw.AmbientColor|file"))
					{
						aiString path;
						material->GetTexture(aiTextureType_AMBIENT, 0, &path);
						LOG_CORE_TRACE("AmbientColor file found: {0}", path.C_Str());
					}
					if (prop->mKey == aiString("$raw.DiffuseColor|file"))
					{
						aiString path;
						material->GetTexture(aiTextureType_DIFFUSE, 0, &path);
						LOG_CORE_TRACE("DiffuseColor file found: {0}", path.C_Str());
					}
					if (prop->mKey == aiString("$tex.file"))
					{
//						aiString path;
//						material->GetTexture(aiTextureType_NONE, 0, &path);
//						LOG_CORE_TRACE("Texture file found: {0}", path.C_Str());
					}
	
//					"$raw.ambient"

				}


			}
		}

		// Load textures

		if (aiscene->HasTextures())
		{
			for (uint32_t i = 0; i < aiscene->mNumTextures; i++)
			{
				aiTexture* texture = aiscene->mTextures[i];

				LOG_CORE_TRACE("Texture {0}: {1}", i, texture->mFilename.C_Str());
			}
		}
		else
		{
			LOG_CORE_TRACE("No textures in file");
		}

		// Load lights

		if (aiscene->HasLights())
		{
			LOG_CORE_TRACE("Loading {0} lights", aiscene->mNumLights);

			for (uint32_t i = 0; i < aiscene->mNumLights; i++)
			{
				LOG_CORE_TRACE(" - light {0}: {1} ({2})", i, aiscene->mLights[i]->mName.C_Str(), aiLightTypeToString(aiscene->mLights[i]->mType).c_str());
			}
		}

		// Load Materials

		if (aiscene->HasMaterials())
		{
			LOG_CORE_TRACE("Loading {0} materials", aiscene->mNumMaterials);

			for (uint32_t i = 0; i < aiscene->mNumMaterials; i++)
			{
				LOG_CORE_TRACE(" - material {0}: {1}", i, aiscene->mMaterials[i]->GetName().C_Str());
			}
		}

		return entity;
	}



}
