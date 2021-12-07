#ifndef __NUTLIB_COMPONENTS_H
#define __NUTLIB_COMPONENTS_H

#include "NutLib/Core/Base.h"
//#include "Model.h"
#include "NutLib/Core/DataBuffer.h"

#include "NutLib/Renderer/Pipeline.h"
#include "NutLib/Renderer/Shader.h"

//#include "NutLib/Asset/MeshSource.h"
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
		Ref<MeshAsset> Mesh;


		MeshComponent()
		{

		}

		MeshComponent(MeshComponent& component)
		{
			Mesh = component.Mesh;
		}

//		MeshComponent(MeshComponent&& component)
//		{
//			Mesh = component.Mesh;
//		}
		
	};


}


#endif
