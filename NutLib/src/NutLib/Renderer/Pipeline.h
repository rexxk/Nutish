#pragma once

#include "Shader.h"


namespace Nut
{

	class Pipeline
	{
	public:
		struct BufferLayoutItem
		{

			ShaderLayoutDescriptor LayoutDescriptor;

			uint32_t Offset = 0;
			bool Normalized = false;

			//			BufferLayoutItem(const std::string& name, LayoutType type, bool normalized = false)
			//				: Name(name), Type(type), Normalized(normalized)
			BufferLayoutItem(const ShaderLayoutDescriptor& descriptor)
				: LayoutDescriptor(descriptor)
			{

			}

		};

		struct BufferLayout
		{
			std::vector<BufferLayoutItem> m_Items;

			uint32_t Stride = 0;
		};


	public:
		static Ref<Pipeline> Create(Ref<Shader> shader);

		virtual ~Pipeline() {}

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void SetBufferLayout() = 0;

		virtual Ref<Shader> GetShader() = 0;

	};

}
