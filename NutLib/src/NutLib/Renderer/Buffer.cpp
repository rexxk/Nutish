#include "nutpch.h"
#include "Buffer.h"

#include "Renderer.h"

#include "Platform/OpenGL/OpenGLBuffer.h"


namespace Nut
{

	Ref<VertexBuffer> VertexBuffer::Create(void* data, uint32_t size)
	{
		switch (Renderer::API())
		{
		case RendererAPIType::OpenGL: return CreateRef<OpenGLVertexBuffer>(data, size);

		}

		LOG_CORE_WARN("[VertexBuffer]: No api defined.");

		return nullptr;
	}



	Ref<IndexBuffer> IndexBuffer::Create(void* data, uint32_t count)
	{
		switch (Renderer::API())
		{
		case RendererAPIType::OpenGL: return CreateRef<OpenGLIndexBuffer>(data, count);

		}

		LOG_CORE_WARN("[IndexBuffer]: No api defined.");

		return nullptr;
	}

}
