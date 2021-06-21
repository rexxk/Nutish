#include "nutpch.h"
#include "LayerStack.h"


namespace Nut
{

	LayerStack::LayerStack()
		: m_LayerInsertCount(0)
	{

	}

	LayerStack::~LayerStack()
	{
		for (auto& layer : m_Layers)
		{
			layer->OnDetach();
		}

		m_Layers.clear();
	}


	void LayerStack::AttachLayer(const Ref<Layer>& layer)
	{
		if (std::find(m_Layers.begin(), m_Layers.begin() + m_LayerInsertCount, layer) == (m_Layers.begin() + m_LayerInsertCount))
		{
			m_Layers.push_back(layer);
			m_LayerInsertCount++;

			layer->OnAttach();
		}
	}

	void LayerStack::DetachLayer(const Ref<Layer>& layer)
	{
		auto loc = std::find(m_Layers.begin(), m_Layers.begin() + m_LayerInsertCount, layer);

		if (loc != (m_Layers.begin() + m_LayerInsertCount))
		{
			m_Layers.erase(loc);
			m_LayerInsertCount--;

			layer->OnDetach();
		}

	}

	void LayerStack::AttachOverlay(const Ref<Layer>& overlay)
	{
		if (std::find(m_Layers.begin() + m_LayerInsertCount, m_Layers.end(), overlay) == m_Layers.end())
		{
			m_Layers.push_back(overlay);

			overlay->OnAttach();
		}
	}

	void LayerStack::DetachOverlay(const Ref<Layer>& overlay)
	{
		auto loc = std::find(m_Layers.begin() + m_LayerInsertCount, m_Layers.end(), overlay);

		if (loc != m_Layers.end())
		{
			m_Layers.erase(loc);

			overlay->OnDetach();
		}

	}


}
