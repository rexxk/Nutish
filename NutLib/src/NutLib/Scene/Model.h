#pragma once

#include "NutLib/Renderer/Pipeline.h"
#include "NutLib/Renderer/Mesh.h"

#include "NutLib/Scene/Entity.h"

namespace Nut
{


	class Model
	{
	public:
		static Entity Load(const std::string& filepath, Ref<Scene> scene, Ref<Pipeline> pipeline);

	};


}
