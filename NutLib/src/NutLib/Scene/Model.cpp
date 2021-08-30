#include "nutpch.h"
#include "Model.h"

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"


namespace Nut
{

	Ref<Model> Model::Load(const std::string& filepath, Ref<Scene> scene)
	{
		Assimp::Importer importer;

		const aiScene* aiscene = importer.ReadFile(filepath, aiProcess_OptimizeMeshes);

		return CreateRef<Model>(scene, aiscene->mMeshes[0]->mName.C_Str());
	}



	Model::Model(Ref<Scene> scene, const std::string& tag)
		: Object(scene, tag)
	{

	}

	Model::~Model()
	{

	}

}
