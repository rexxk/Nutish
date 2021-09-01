#include "MainWindow.h"

#include <iostream>

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


using namespace Nut;



MainWindow::MainWindow(const std::string& name)
	: Layer(name)
{
	SubscribeToEvent<KeyReleasedEvent>([&](KeyReleasedEvent& event)
		{
			if (event.Key() == KEY_SPACE)
			{
				LOG_TRACE("Space pressed - reloading shader");

				m_BasicShader->Reload();
			}

			return false;
		});

}

MainWindow::~MainWindow()
{
	LOG_TRACE("MainWindow destructor");
}

void MainWindow::OnAttach()
{
	m_Scene = CreateRef<Scene>();

	ShaderStore::Add("Basic", Shader::Create("assets/shaders/Basic.shader"));

	m_BasicShader = ShaderStore::Get("Basic");

	m_BasicPipeline = Pipeline::Create(m_BasicShader);
	m_BasicPipeline->Bind();


	UUID eID = m_Scene->CreateEntity("Test entity");

	auto& tagView = View<TagComponent>::Get();

	for (auto& t : tagView)
	{
		LOG_TRACE("Tag: {0} (UUID: {1})", Entity::GetComponent<TagComponent>(t).Tag.c_str(), t);
	}


	float vertices[] =
	{
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
		-0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
	};

	uint32_t indices[] =
	{
		0, 1, 2, 2, 3, 0,
	};

//	uint8_t texData[4] = { 128, 0, 128, 255 };
//	m_Texture = Texture2D::Create("assets/textures/texture.png");

	m_TriangleVB = VertexBuffer::Create(vertices, sizeof(vertices));
	m_TriangleIB = IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));

	m_BasicPipeline->SetBufferLayout();

//	Ref<VertexBuffer> vertexBuffer = VertexBuffer::Create(nullptr, 0);
//	Ref<IndexBuffer> indexBuffer = IndexBuffer::Create(nullptr, 0);

	LOG_TRACE("Available shaders:");

	auto shaderList = ShaderStore::List();

	for (auto& shaderName : shaderList)
	{
		LOG_TRACE(" - {0}", shaderName.c_str());
	}

	m_Texture = Texture2D::Create("assets/textures/texture.png");
	m_GradientTexture = Texture2D::Create("assets/textures/texture2.png");

//	m_Texture->Bind(0);

//	m_Texture = Texture2D::Create(texData, 1, 1);

//	m_Rectangle = CreateRef<Model>(m_Scene, "Rectangle");

//	m_Rectangle = Model::Load("assets/models/rubik.fbx", m_Scene);
	m_Rectangle = Model::Load("assets/models/cube.obj", m_Scene);

	LOG_TRACE("m_Rectangle id: {0}, tag: {1}", m_Rectangle->ID(), Entity::GetComponent<TagComponent>(m_Rectangle->ID()).Tag.c_str());

}

void MainWindow::OnDetach()
{
//	LOG_TRACE("Detaching layer");
}

void MainWindow::OnUpdate(Timestep ts)
{
//	LOG_TRACE("Timestep: {0}", (double)ts);

	m_BasicShader->Bind();
	m_BasicShader->Set("u_Texture1", 5);
	m_BasicShader->Set("u_Texture2", 3);
	m_Texture->Bind(5);
	m_GradientTexture->Bind(3);


	//	m_Texture->Bind(0);


//	auto orthoMatrix = glm::ortho(0.0f, 1280.0f, 720.0f, 0.0f, 0.0f, 1.0f);
	auto orthoMatrix = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);
	m_BasicShader->Set("u_ViewProjection", orthoMatrix);
	

//	m_Texture->Bind(m_Texture->ID());

}

void MainWindow::OnRender()
{

	Renderer::BeginScene();

//	m_TriangleVB->Bind();
//	m_TriangleIB->Bind();

//	m_TriangleVA->Bind();
	
//	GLsizei indexCount = m_TriangleVA->GetIndexBuffer()->GetIndexCount();
	GLsizei indexCount = m_TriangleIB->GetIndexCount();

	Renderer::Submit(m_Rectangle);

	RenderCommandQueue::Submit([=]()
		{
			glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
		});


	Renderer::EndScene();


}
