#pragma once

#include "Timestep.h"


namespace Nut
{

	class Layer
	{
	public:
		Layer(const std::string& name = "layer");
		virtual ~Layer();

		virtual void OnAttach() {}
		virtual void OnDetach() {}

		virtual void OnUpdate(Timestep ts) {}
		virtual void OnRender() {}


	protected:
		std::string m_LayerName;
	};

}
