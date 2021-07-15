#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <NutLib.h>



class MainWindow : public Nut::Layer
{
public:
	MainWindow(const std::string& name);

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	virtual void OnUpdate(Nut::Timestep ts) override;
	virtual void OnRender() override;

private:

	Ref<Nut::Scene> m_Scene;

	Ref<Nut::Shader> m_BasicShader;
};


#endif
