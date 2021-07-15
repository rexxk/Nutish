#ifndef __NUTLIB_LAYER_H
#define __NUTLIB_LAYER_H


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


#endif
