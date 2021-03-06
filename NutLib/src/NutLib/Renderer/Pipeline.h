#pragma once

#include "Shader.h"

#include "NutLib/Core/UUID.h"

#include "NutLib/Core/DataBuffer.h"
#include "Buffer.h"

namespace Nut
{



	class Pipeline
	{
	public:
		static Ref<Pipeline> Create(Ref<Shader> shader);

		virtual ~Pipeline() {}

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void SetBufferLayout() = 0;
		virtual void SetInstanceLayout() = 0;

		virtual Ref<Shader> GetShader() = 0;

	};

}
