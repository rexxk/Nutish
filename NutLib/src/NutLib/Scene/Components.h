#ifndef __NUTLIB_COMPONENTS_H
#define __NUTLIB_COMPONENTS_H

#include "NutLib/Core/Base.h"
//#include "Model.h"
#include "NutLib/Core/DataBuffer.h"

#include "NutLib/Renderer/Shader.h"

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
		Ref<DataBuffer<ShaderLayoutItem>> Vertices;
		uint32_t* Indices;

		MeshComponent(uint32_t* indices)
			: Indices(indices)
//		MeshComponent(/*Ref<DataBuffer<ShaderLayoutItem>> vertices, */std::vector<uint32_t> indices)
//			: /*Vertices(vertices), */Indices(indices)
		{
//			LOG_CORE_TRACE("MeshComponent data: {0}, {1}", data, bdata);

			LOG_CORE_TRACE("MeshComponent going indices..?");
		}

	};


}


#endif
