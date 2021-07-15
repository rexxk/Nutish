#ifndef __NUTLIB_BUFFER_H
#define __NUTLIB_BUFFER_H


namespace Nut
{

	class VertexBuffer
	{
	public:
		static Ref<VertexBuffer> Create(void* data, uint32_t size);

		virtual ~VertexBuffer() {}

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual RendererID ID() const = 0;
	};


	class IndexBuffer
	{
	public:
		static Ref<IndexBuffer> Create(void* data, uint32_t count);

		virtual ~IndexBuffer() {}

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual uint32_t GetIndexCount() const = 0;

		virtual RendererID ID() const = 0;

	};

}


#endif
