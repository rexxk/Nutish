#include "nutpch.h"
#include "Renderer.h"

#include "Platform/OpenGL/OpenGLRenderer.h"


namespace Nut
{

	void Renderer::Init()
	{
		switch (s_API)
		{
		case RendererAPIType::None: s_Renderer = nullptr; return;
		case RendererAPIType::OpenGL: s_Renderer = CreateScope<OpenGLRenderer>(); return;
		}

		s_Renderer->m_RenderData.BatchVertexBuffer = VertexBuffer::Create(nullptr, RenderData::MAX_VERTICES, BufferUsage::Dynamic);
		s_Renderer->m_RenderData.BatchIndexBuffer = IndexBuffer::Create(nullptr, RenderData::MAX_INDICES, BufferUsage::Dynamic);

	}

	void Renderer::BeginScene()
	{
		s_Renderer->m_RenderData.Statistics.DrawCalls = 0;

		s_Renderer->BeginSceneImplementation();
	}

	void Renderer::EndScene()
	{
		s_Renderer->EndSceneImplementation();
	}

	void Renderer::Present()
	{
		s_Renderer->PresentImplementation();
	}

	void Renderer::Submit(Ref<Model> model)
	{
		s_Renderer->SubmitImplementation(model);
	}

	void Renderer::Flush()
	{
		s_Renderer->FlushImplementation();
	}

}
