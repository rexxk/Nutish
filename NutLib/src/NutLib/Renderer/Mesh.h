#pragma once

#include "NutLib/Scene/Object.h"
#include "Pipeline.h"


namespace Nut
{


	class Mesh : public Object
	{
	public:
		Mesh(Ref<Scene> scene);
		virtual ~Mesh();

	private:
		Ref<Pipeline::BufferLayout> m_BufferLayout;

		std::vector<uint8_t> m_VertexData;
		std::vector<uint32_t> m_IndexData;
	};


}
