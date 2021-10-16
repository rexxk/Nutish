#ifndef __NUTLIB_COMPONENTS_H
#define __NUTLIB_COMPONENTS_H

#include "NutLib/Core/Base.h"
//#include "Model.h"
#include "NutLib/Core/DataBuffer.h"

#include "NutLib/Renderer/Pipeline.h"
#include "NutLib/Renderer/Shader.h"

#include "NutLib/Asset/MeshAsset.h"

#include <vector>

namespace Nut
{
	class Model;

	struct TagComponent
	{
		std::string Tag;

		TagComponent()
			: Tag("<none>")
		{

		}

		TagComponent(const std::string& tag)
			: Tag(tag)
		{

		}

	};


	struct TransformComponent
	{

		TransformComponent()
		{

		}

	};


	struct ModelComponent
	{
		Ref<Model> GraphicsModel;

		ModelComponent(Ref<Model> model)
			: GraphicsModel(model)
		{

		}

	};

	struct MeshComponent
	{
//		DataBuffer<ShaderLayoutItem> Vertices;
//		std::vector<uint32_t> Indices;

		MeshAsset AssetData;
		Ref<Pipeline> Pipeline;

		MeshComponent()
			: AssetData()
		{

		}

//		MeshComponent(const std::vector<uint32_t>& indices)
//			: Indices(indices)
//		{

//		MeshComponent(uint32_t* indices)
//			: Indices(indices)
		MeshComponent(DataBuffer<ShaderLayoutItem> vertices, const std::vector<uint32_t>& indices)
			: AssetData(vertices, indices)
		{

		}

	};


}


#endif
