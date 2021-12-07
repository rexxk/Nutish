#pragma once

#include "NutLib/Scene/Scene.h"

#include "Pipeline.h"


namespace Nut
{


	class Mesh
	{
	public:
		Mesh(Ref<Scene> scene);
		virtual ~Mesh();

	private:
		Ref<DataBufferLayout<DataBufferLayoutItem>> m_BufferLayout;

		std::vector<uint8_t> m_VertexData;
		std::vector<uint32_t> m_IndexData;

		Entity m_Entity;
	};


}
