#pragma once

#include "DataType.h"



namespace Nut
{

	struct DataBufferLayoutItem
	{
		uint32_t Offset = 0;

		DataType Type;
		std::string Name;

		DataBufferLayoutItem(DataType type, const std::string& name)
			: Type(type), Name(name)
		{

		}

	};

	template<typename T>
	class DataBufferLayout
	{
	public:
		DataBufferLayout()
			: m_Stride(0)
		{
			m_Items.clear();
		}

		DataBufferLayout(std::initializer_list<T> items)
		{
			for (auto& item : items)
			{
				m_Items.emplace_back(item);
			}

			UpdateOffsets();
		}

		void UpdateOffsets()
		{
			uint32_t offset = 0;

			for (auto& item : m_Items)
			{
				item.Offset += offset;

				m_Stride += DataTypeSize(item.Type);
				offset = m_Stride;
			}
		}

		std::vector<T>& Items() { return m_Items; }

		uint32_t Stride() const { return m_Stride; }

	private:
		std::vector<T> m_Items;

		uint32_t m_Stride = 0;
	};


	template<typename T>
	class DataBuffer
	{
	public:
		DataBuffer(void* data, uint32_t count, DataBufferLayout<T> layout)
			: m_BufferLayout(layout), m_Count(count), m_Buffer(data)
		{
			m_Size = count * layout.Stride();

			LOG_CORE_TRACE("DataBuffer size: {0}", m_Size);
		}

		virtual ~DataBuffer()
		{

		}

	private:
		void* m_Buffer = nullptr;

		uint32_t m_Size = 0;
		uint32_t m_Count = 0;

		DataBufferLayout<T> m_BufferLayout;
	};


}
