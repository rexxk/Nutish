#pragma once

#include <NutLib/Renderer/Renderer.h>


namespace Nut
{


	class OpenGLRenderer : public Renderer
	{
	public:
		OpenGLRenderer();
		virtual ~OpenGLRenderer();

	private:
		virtual void BeginSceneImplementation() override;
		virtual void EndSceneImplementation() override;

	};


}
