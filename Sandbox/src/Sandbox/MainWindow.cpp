#include "MainWindow.h"

#include <iostream>

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


using namespace Nut;



MainWindow::MainWindow(const std::string& name)
	: Layer(name), m_LightDirection(glm::vec3(0.0f))
{
	SubscribeToEvent<KeyReleasedEvent>([&](KeyReleasedEvent& event)
		{
			if (event.Key() == KEY_SPACE)
			{
				LOG_TRACE("Space pressed - reloading shader");

				m_BasicShader->Reload();
			}

			if (event.Key() == KEY_SUBTRACT)
			{
				LOG_TRACE("Subtract pressed");

				if (m_VisibleLights != 0)
					m_VisibleLights--;
			}

			if (event.Key() == KEY_ADD)
			{
				LOG_TRACE("Add pressed");

				auto [lights] = ECS::EntitySystem::GetView<LightComponent>();

				if (m_VisibleLights < static_cast<uint32_t>(lights.size()))
					m_VisibleLights++;
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


	std::vector<float> vertices =
	{
		// Vertex, TexCoord, Normal
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		-0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
	};

	std::vector<uint32_t> indices = { 0, 1, 2, 2, 3, 0 };

	//	{
	m_TestEntity = m_Scene->CreateEntity("Test entity");
	m_TestEntity.AddComponent<MeshComponent>();
	//		Entity::AddComponent<MeshComponent>(m_TestEntity);
	m_TestEntity.AddComponent<TransformComponent>();
	//		Entity::AddComponent<TransformComponent>(m_TestEntity);

	auto& transform = m_TestEntity.GetComponent<TransformComponent>().Transform;

	//		auto& transform = ECS::EntitySystem::GetComponent<TransformComponent>(m_TestEntity.ID()).Transform;
	transform = glm::translate(glm::mat4(1.0f), glm::vec3(0.25f, 0.0f, 0.0f));

	auto& triangleMesh = m_TestEntity.GetComponent<MeshComponent>();
	//		auto& triangleMesh = ECS::EntitySystem::GetComponent<MeshComponent>(m_TestEntity.ID());
	//		triangleMesh.Mesh = MeshAsset(DataBuffer<ShaderLayoutItem>(vertices.data(), 4, m_BasicShader->GetShaderLayout()), indices, m_BasicPipeline, m_Scene);
	triangleMesh.Mesh = CreateRef<MeshAsset>(DataBuffer<ShaderLayoutItem>(vertices.data(), 4, m_BasicShader->GetShaderLayout()), indices, m_BasicPipeline, m_Scene);

	//	}




	//	auto& tagView = View<TagComponent>::Get();

	//	for (auto& t : tagView)
	//	{
	//		LOG_TRACE("Tag: {0} (UUID: {1})", Entity::GetComponent<TagComponent>(t).Tag.c_str(), t);
	//	}





	//	triangleMesh.AssetData.SetVertexData(DataBuffer<ShaderLayoutItem>(vertices.data(), 4, m_BasicShader->GetShaderLayout()));
	//	triangleMesh.AssetData.SetIndexData({0, 1, 2, 2, 3, 0});

	std::vector<float> vertices2 =
	{
		// Vertex, TexCoord
		0.5f, 0.5f, 0.0f, 0.0f, 0.0f,
		1.5f, 0.5f, 0.0f, 1.0f, 0.0f,
		1.5f, 1.5f, 0.0f, 1.0f, 1.0f,
		0.5f, 1.5f, 0.0f, 0.0f, 1.0f,
	};

	{
		m_TestEntity2 = m_Scene->CreateEntity("Test entity 2");
		//		ECS::EntitySystem::AddComponent<MeshComponent>(m_TestEntity2.ID(), triangleMesh);
		m_TestEntity2.AddComponent<MeshComponent>(triangleMesh);
		//		Entity::AddComponent<MeshComponent>(m_TestEntity2, triangleMesh);
		//		Entity::AddComponent<MeshComponent>(m_TestEntity2);
		//		auto& mesh = Entity::GetComponent<MeshComponent>(m_TestEntity2).Mesh;
		//		mesh = CreateRef<MeshAsset>(DataBuffer<ShaderLayoutItem>(vertices.data(), 4, m_BasicShader->GetShaderLayout()), indices, m_BasicPipeline, m_Scene);

		m_TestEntity2.AddComponent<TransformComponent>();
		auto& t2 = m_TestEntity2.GetComponent<TransformComponent>().Transform;
		//		ECS::EntitySystem::AddComponent<TransformComponent>(m_TestEntity2.ID());
		//		auto& t2 = ECS::EntitySystem::GetComponent<TransformComponent>(m_TestEntity2.ID()).Transform;
		t2 = glm::translate(glm::mat4(1.0f), glm::vec3(-0.5f, 0.5f, 0.0f));
	}

	//	auto& t2 = Entity::GetComponent<TransformComponent>(m_TestEntity2).Transform;
//	t2 = glm::translate(glm::mat4(1.0f), glm::vec3(-0.5f, 0.5f, 0.0f));

//	auto& mesh2 = Entity::GetComponent<MeshComponent>(m_TestEntity2);

//	mesh2.Mesh = CreateRef<MeshAsset>(DataBuffer<ShaderLayoutItem>(vertices2.data(), 4, m_BasicShader->GetShaderLayout()), indices, m_BasicPipeline, m_Scene);

//	mesh2.AssetData = MeshAsset(DataBuffer<ShaderLayoutItem>(vertices2.data(), 4, m_BasicShader->GetShaderLayout()), { 0, 1, 2, 2, 3, 0 });

//	mesh2.Vertices = DataBuffer<ShaderLayoutItem>(vertices2.data(), 4, m_BasicShader->GetShaderLayout());
//	mesh2.Indices = { 0, 1, 2, 2, 3, 0 };

//	mesh2.Pipeline = m_BasicPipeline;

//	uint8_t texData[4] = { 128, 0, 128, 255 };
//	m_Texture = Texture2D::Create("assets/textures/texture.png");

//	m_TriangleVB = VertexBuffer::Create(vertices, sizeof(vertices));
//	m_TriangleIB = IndexBuffer::Create(indices.data(), static_cast<uint32_t>(indices.size()));
//	m_TriangleVB = VertexBuffer::Create(triangleMesh.Vertices.Data(), triangleMesh.Vertices.Size());
//	m_TriangleIB = IndexBuffer::Create(triangleMesh.Indices.data(), static_cast<uint32_t>(triangleMesh.Indices.size()));

//	m_BasicPipeline->SetBufferLayout();

	LOG_TRACE("Available shaders:");

	auto shaderList = ShaderStore::List();

	for (auto& shaderName : shaderList)
	{
		LOG_TRACE(" - {0}", shaderName.c_str());
	}

	//	m_Texture = Texture2D::Create("assets/textures/texture.png");
	m_Texture = Texture2D::Create("assets/textures/v90.png");
	m_GradientTexture = Texture2D::Create("assets/textures/texture2.png");

	//	m_Model = Model::Load("assets/models/rubik.fbx", m_Scene, m_BasicPipeline);
	//	m_Model = Model::Load("assets/models/cube.obj", m_Scene, m_BasicPipeline);
	m_Model = Model::Load("assets/models/sponza.fbx", m_Scene, m_BasicPipeline);
	//	m_Model = Model::Load("assets/models/Mandalorian.fbx", m_Scene, m_BasicPipeline);
	//	m_Model = Model::Load("assets/models/Anna_FBX.FBX", m_Scene, m_BasicPipeline);
	//	m_Model = Model::Load("assets/models/Anna_FBX_2.FBX", m_Scene, m_BasicPipeline);
	//	m_Model = Model::Load("assets/models/a599e.fbx", m_Scene, m_BasicPipeline);
	//	m_Model = Model::Load("assets/models/sponza/Sponza.gltf", m_Scene, m_BasicPipeline);
	//	m_Model = Model::Load("assets/models/stormtrooper/silly_dancing.fbx", m_Scene, m_BasicPipeline);

	//	LOG_TRACE("m_Rectangle id: {0}, tag: {1}", m_Rectangle->ID(), Entity::GetComponent<TagComponent>(m_Rectangle->ID()).Tag.c_str());

	LightProperties ambientProps;
	ambientProps.Type = LightType::Ambient;
	ambientProps.Ambient = glm::vec3(0.2f, 0.1f, 0.1f);


	Light ambientLight(ambientProps);

	m_AmbientLight.AddComponent<LightComponent>(ambientLight);

	m_Camera = Camera::Create({ 0.0f, 3.0f, 150.0f });

	m_LightDirection = glm::vec3(-0.5f, 0.5f, -0.5f);


	auto [lights] = ECS::EntitySystem::GetView<LightComponent>();
	m_VisibleLights = lights.size();
}

void MainWindow::OnDetach()
{
//	LOG_TRACE("Detaching layer");
}

void MainWindow::OnUpdate(Timestep ts)
{
//	LOG_TRACE("Timestep: {0}", (double)ts);


	m_BasicShader->Bind();

	// Setup lights
	auto [lightIDs] = ECS::EntitySystem::GetView<LightComponent>();

//	m_BasicShader->Set("u_LightCount", (int)lightIDs.size());
	m_BasicShader->Set("u_LightCount", (int)m_VisibleLights);

	uint32_t count = 0;

	for (auto lightID : lightIDs)
	{
		auto& light = ECS::EntitySystem::GetComponent<LightComponent>(lightID).LightSource;
		auto props = light.GetProperties();
		// Put light sources into .. some lightmap for the renderer?

		auto& transform = ECS::EntitySystem::GetComponent<TransformComponent>(lightID).Transform;

		props.Position = glm::vec3(transform * glm::vec4(props.Position, 1.0f));
		props.Direction = glm::vec3(transform * glm::vec4(props.Direction, 0.0f));

		std::string uniformName = "u_Lights[" + std::to_string(count++) + "].";

		m_BasicShader->Set(uniformName + "LightType", (int)props.Type);
		m_BasicShader->Set(uniformName + "Direction", props.Direction);
		m_BasicShader->Set(uniformName + "Position", props.Position);

		m_BasicShader->Set(uniformName + "Ambient", props.Ambient);
		m_BasicShader->Set(uniformName + "Diffuse", props.Diffuse);
		m_BasicShader->Set(uniformName + "Specular", props.Specular);

//		m_BasicShader->Set(uniformName + "AttenuationConstant", props.AttenuationConstant);
//		m_BasicShader->Set(uniformName + "AttenuationLinear", props.AttenuationLinear);
//		m_BasicShader->Set(uniformName + "AttenuationQuadratic", props.AttenuationQuadratic);

		m_BasicShader->Set(uniformName + "AttenuationConstant", 1.0f);
		m_BasicShader->Set(uniformName + "AttenuationLinear", 0.003f);
		m_BasicShader->Set(uniformName + "AttenuationQuadratic", 0.0001f);

		m_BasicShader->Set(uniformName + "AngleInnerCone", props.AngleInnerCone);
		m_BasicShader->Set(uniformName + "AngleOuterCone", props.AngleOuterCone);
	}


//	m_BasicShader->Set("u_Texture1", 5);
//	m_BasicShader->Set("u_Texture2", 3);
//	m_BasicShader->Set("u_LightDirection", m_LightDirection);

	m_Texture->Bind(5);
	m_GradientTexture->Bind(3);


	//	m_Texture->Bind(0);


//	auto orthoMatrix = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);
//	m_BasicShader->Set("u_ViewProjection", orthoMatrix);
	
	m_BasicShader->Set("u_ViewProjection", m_Camera->GetViewProjectionMatrix());


//	m_Texture->Bind(m_Texture->ID());

}

void MainWindow::OnRender()
{

//	LOG_TRACE("OnRender");

	Renderer::BeginScene();

//	GLsizei indexCount = m_TriangleVA->GetIndexBuffer()->GetIndexCount();
//	GLsizei indexCount = m_TriangleIB->GetIndexCount();

//	Renderer::Submit(m_Rectangle);

//	m_BasicPipeline->Bind();

	m_Scene->Draw();

//	RenderThread::Submit([=]()
//		{
//			glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
//		});

//	m_BasicPipeline->Flush();

	Renderer::EndScene();

	Renderer::Draw();



}
