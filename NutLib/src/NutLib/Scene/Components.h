#ifndef __NUTLIB_COMPONENTS_H
#define __NUTLIB_COMPONENTS_H

#include "NutLib/Core/Base.h"
//#include "Model.h"
#include "NutLib/Core/DataBuffer.h"

#include "NutLib/Renderer/Pipeline.h"
#include "NutLib/Renderer/Shader.h"

#include "NutLib/Asset/MeshSource.h"

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
		glm::mat4 Transform;

		TransformComponent()
			: Transform(glm::mat4(1.0f))
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

		Ref<MeshSource> MeshData;
		Ref<Pipeline> Pipeline;
		
		UUID MeshID;

		MeshComponent()
			: MeshData()
		{

		}

//		MeshComponent(const std::vector<uint32_t>& indices)
//			: Indices(indices)
//		{

//		MeshComponent(uint32_t* indices)
//			: Indices(indices)
		MeshComponent(DataBuffer<ShaderLayoutItem> vertices, const std::vector<uint32_t>& indices)
			: MeshData(CreateRef<MeshSource>(vertices, indices))
		{

		}

		MeshComponent(MeshComponent& component)
		{
//			AssetData.SetVertexData(component.AssetData.Vertices());
//			AssetData.SetIndexData(component.AssetData.Indices());
			MeshData = component.MeshData;
			Pipeline = component.Pipeline;

			MeshID = component.MeshID;
		}

	};


}


#endif
