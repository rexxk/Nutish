#include "nutpch.h"
#include "OpenGLRenderer.h"
#include "OpenGLBuffer.h"

#include "NutLib/Core/Application.h"

#include "NutLib/Renderer/RenderThread.h"


#include <glad/glad.h>


namespace Nut
{

	OpenGLRenderer::OpenGLRenderer()
	{
		LOG_CORE_TRACE("Creating OpenGL Renderer");
	}

	OpenGLRenderer::~OpenGLRenderer()
	{

	}

	void OpenGLRenderer::BeginSceneImplementation()
	{
		RenderThread::Submit([]()
			{
				glClearColor(1.0f, 0.0f, 1.0f, 1.0f);
				glClear(GL_COLOR_BUFFER_BIT);
			});

	}

	void OpenGLRenderer::EndSceneImplementation()
	{

	}

	void OpenGLRenderer::SubmitImplementation(Ref<MeshAsset> mesh, const glm::mat4& transform)
	{
		// Submit sends the data to the render structure.

		for (auto& submesh : mesh->Submeshes())
		{
			auto& renderData = Renderer::GetRenderData();

			renderData.InstanceMap[submesh.ID()].push_back(transform);
			renderData.MeshBuffers[mesh->GetPipeline()].push_back(submesh.GetMeshBuffers());
		}

	}

	void OpenGLRenderer::DrawImplementation()
	{
		auto& renderData = Renderer::GetRenderData();

		for (auto& meshBuffer : renderData.MeshBuffers)
		{
			Ref<Pipeline> pipeline = meshBuffer.first;

			pipeline->Bind();
			pipeline->SetBufferLayout();


			for (auto& buffers : meshBuffer.second)
			{
				buffers.VertexBuffer->Bind();
				buffers.IndexBuffer->Bind();

				RenderThread::Submit([=]()
					{
						glDrawElements(GL_TRIANGLES, buffers.IndexBuffer->GetIndexCount(), GL_UNSIGNED_INT, nullptr);
					});

			}
		}




		renderData.InstanceMap.clear();
		renderData.MeshBuffers.clear();
	}


}
