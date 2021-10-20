#ifndef __NUTLIB_OPENGL_RENDERER_H
#define __NUTLIB_OPENGL_RENDERER_H


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

		virtual void SubmitImplementation(Ref<MeshAsset> mesh, const glm::mat4& transform) override;
		virtual void DrawImplementation() override;
	};


}


#endif
