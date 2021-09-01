#include "nutpch.h"
#include "Buffer.h"

#include "Renderer.h"

#include "Platform/OpenGL/OpenGLBuffer.h"


namespace Nut
{

	Ref<VertexBuffer> VertexBuffer::Create(void* data, uint32_t size, BufferUsage usage)
	{
		switch (Renderer::API())
		{
		case RendererAPIType::OpenGL: return CreateRef<OpenGLVertexBuffer>(data, size, usage);

		}

		LOG_CORE_WARN("[VertexBuffer]: No api defined.");

		return nullptr;
	}



	Ref<IndexBuffer> IndexBuffer::Create(void* data, uint32_t count, BufferUsage usage)
	{
		switch (Renderer::API())
		{
		case RendererAPIType::OpenGL: return CreateRef<OpenGLIndexBuffer>(data, count, usage);

		}

		LOG_CORE_WARN("[IndexBuffer]: No api defined.");

		return nullptr;
	}

}
