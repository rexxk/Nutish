#include "nutpch.h"
#include "Mesh.h"



namespace Nut
{


	Mesh::Mesh(Ref<Scene> scene)
	{
		m_Entity = scene->CreateEntity("mesh");
	}

	Mesh::~Mesh()
	{

	}

}
