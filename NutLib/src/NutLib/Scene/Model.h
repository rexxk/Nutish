#pragma once

#include "NutLib/Renderer/Pipeline.h"
#include "NutLib/Renderer/Mesh.h"

#include "NutLib/Scene/Entity.h"



struct aiNode;


namespace Nut
{


	class Model
	{
	public:
		static Entity Load(const std::string& filepath, Ref<Scene> scene, Ref<Pipeline> pipeline);


	private:
		static glm::mat4 FindMatrix(const std::string& name, aiNode* node, aiNode** returnNode);
	};


}
