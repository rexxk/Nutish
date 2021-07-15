#include "MainWindow.h"

#include <iostream>

#include <glad/glad.h>


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


	float vertices[] =
	{
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.0f, 0.5f, 0.0f,
	};

	uint32_t indices[] =
	{
		0, 1, 2,
	};

	m_TriangleVA = OpenGLVertexArray::Create();
	m_TriangleVA->Bind();

	m_TriangleVB = VertexBuffer::Create(vertices, sizeof(vertices));
	m_TriangleIB = IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));

	m_TriangleVA->AttachVertexBuffer(m_TriangleVB);
	m_TriangleVA->AttachIndexBuffer(m_TriangleIB);

	m_TriangleVA->SetBufferLayout();

	Ref<VertexBuffer> vertexBuffer = VertexBuffer::Create(nullptr, 0);
	Ref<IndexBuffer> indexBuffer = IndexBuffer::Create(nullptr, 0);

	m_BasicShader = Shader::Create("assets/shaders/Basic.shader");

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

	m_BasicShader->Bind();

	m_TriangleVB->Bind();
	m_TriangleIB->Bind();

	
	GLsizei indexCount = m_TriangleIB->GetIndexCount();

	RenderCommandQueue::Submit([=]()
		{
			glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
		});


	Renderer::EndScene();


}
