#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <NutLib.h>

#include "Platform/OpenGL/OpenGLVertexArray.h"


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

	Ref<Nut::OpenGLVertexArray> m_TriangleVA;

	Ref<Nut::VertexBuffer> m_TriangleVB;
	Ref<Nut::IndexBuffer> m_TriangleIB;

	Ref<Nut::Texture2D> m_Texture;

};


#endif
