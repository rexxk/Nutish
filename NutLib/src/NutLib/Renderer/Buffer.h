#ifndef __NUTLIB_BUFFER_H
#define __NUTLIB_BUFFER_H


namespace Nut
{

	enum class BufferUsage
	{
		Static,
		Dynamic,
		Stream,
	};


	class VertexBuffer
	{
	public:
		static Ref<VertexBuffer> Create(void* data, uint32_t size, BufferUsage usage = BufferUsage::Static);

		virtual ~VertexBuffer() {}

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void SetData(void* data, uint32_t size) = 0;

		virtual RendererID ID() const = 0;
	};


	class IndexBuffer
	{
	public:
		static Ref<IndexBuffer> Create(void* data, uint32_t count, BufferUsage usage = BufferUsage::Static);

		virtual ~IndexBuffer() {}

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void SetData(void* data, uint32_t size) = 0;

		virtual uint32_t GetIndexCount() const = 0;

		virtual RendererID ID() const = 0;

	};

}


#endif
