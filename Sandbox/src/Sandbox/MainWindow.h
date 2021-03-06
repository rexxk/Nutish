#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <NutLib.h>



class MainWindow : public Nut::Layer
{
public:
	MainWindow(const std::string& name);
	virtual ~MainWindow();

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	virtual void OnUpdate(Nut::Timestep ts) override;
	virtual void OnRender() override;

private:

	Ref<Nut::Scene> m_Scene;

	Ref<Nut::Shader> m_BasicShader;
	Ref<Nut::Pipeline> m_BasicPipeline;

	Ref<Nut::VertexBuffer> m_TriangleVB;
	Ref<Nut::IndexBuffer> m_TriangleIB;

	Ref<Nut::Texture2D> m_Texture;
	Ref<Nut::Texture2D> m_GradientTexture;

	Nut::Entity m_Model;

	Nut::Entity m_AmbientLight;

	Nut::Entity m_TestEntity;
	Nut::Entity m_TestEntity2;

	Ref<Nut::Camera> m_Camera;

	glm::vec3 m_LightDirection;


	uint32_t m_VisibleLights = 0;
};


#endif
