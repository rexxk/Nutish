#include "MainWindow.h"

#include <iostream>


using namespace Nut;



MainWindow::MainWindow(const std::string& name)
	: Layer(name)
{

}

void MainWindow::OnAttach()
{
	m_Scene = CreateRef<Scene>();




	UUID eID = m_Scene->CreateEntity("Test entity");

	auto& tagView = View<TagComponent>::Get();

	for (auto& t : tagView)
	{
		LOG_TRACE("Tag: {0} (UUID: {1})", Entity::GetComponent<TagComponent>(t).Tag.c_str(), t);
	}


	Ref<VertexBuffer> vertexBuffer = VertexBuffer::Create(nullptr, 0);
	Ref<IndexBuffer> indexBuffer = IndexBuffer::Create(nullptr, 0);

	
}

void MainWindow::OnDetach()
{
//	LOG_TRACE("Detaching layer");
}

void MainWindow::OnUpdate(Timestep ts)
{
//	LOG_TRACE("Timestep: {0}", (double)ts);


}

void MainWindow::OnRender()
{

	Renderer::BeginScene();



	Renderer::EndScene();

}
