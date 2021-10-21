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

		auto& renderData = Renderer::GetRenderData();

		renderData.InstanceMap.clear();
		renderData.MeshObjects.clear();

	}

	void OpenGLRenderer::EndSceneImplementation()
	{
		// Create instance buffer etc before drawing.

		auto& renderData = Renderer::GetRenderData();

		for (auto& meshBuffer : renderData.MeshObjects)
		{
			for (auto& objects : meshBuffer.second)
			{
				auto& instances = renderData.InstanceMap[objects->ObjectID()];

				if (static_cast<uint32_t>(instances.size()) != objects->InstanceCount())
				{
					objects->InstanceBuffer()->UpdateData(instances, BufferUsage::Dynamic);
					objects->SetInstanceCount(static_cast<uint32_t>(instances.size()));
				}
			}
		}
	}

	void OpenGLRenderer::SubmitImplementation(Ref<MeshAsset> mesh, const glm::mat4& transform)
	{
		// Submit sends the data to the render structure.

		auto& renderData = Renderer::GetRenderData();

		for (auto& submesh : mesh->Submeshes())
		{
			renderData.InstanceMap[submesh.GetMeshObject()->ObjectID()].push_back(transform);

			bool meshExists = false;

			for (auto& object : renderData.MeshObjects[submesh.GetMeshObject()->ObjectID()])
			{
				if (object->ObjectID() == submesh.GetMeshObject()->ObjectID())
					meshExists = true;
			}

			if (!meshExists)
				renderData.MeshObjects[submesh.GetMeshObject()->ObjectID()].push_back(submesh.GetMeshObject());
		}

	}

	void OpenGLRenderer::DrawImplementation()
	{
		auto& renderData = Renderer::GetRenderData();

		for (auto& meshObject : renderData.MeshObjects)
		{

			for (auto& object : meshObject.second)
			{
				object->Bind();

				RenderThread::Submit([=]()
					{
						glDrawElementsInstanced(GL_TRIANGLES, object->GetIndexCount(), GL_UNSIGNED_INT, nullptr, object->InstanceCount()); // static_cast<GLsizei>(instances.size()));

					});

				object->Unbind();

			}
		}


	}


}
