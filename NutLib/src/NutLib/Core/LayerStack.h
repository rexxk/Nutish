#pragma once

#include "Layer.h"


namespace Nut
{

	class LayerStack
	{
	public:
		LayerStack();
		~LayerStack();

		void AttachLayer(const Ref<Layer>& layer);
		void DetachLayer(const Ref<Layer>& layer);

		void AttachOverlay(const Ref<Layer>& overlay);
		void DetachOverlay(const Ref<Layer>& overlay);

		std::vector<Ref<Layer>>::iterator begin() { return m_Layers.begin(); }
		std::vector<Ref<Layer>>::iterator end() { return m_Layers.end(); }

	private:
		std::vector<Ref<Layer>> m_Layers;

		uint32_t m_LayerInsertCount = 0;
	};


}
