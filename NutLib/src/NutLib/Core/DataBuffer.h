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
		DataBuffer()
		{
//			LOG_CORE_TRACE("DataBuffer() default: {0}", (uint64_t)m_Buffer);
		}

		DataBuffer(uint32_t count, DataBufferLayout<T> layout)
			: m_Count(count)
		{
			m_Size = count * layout.Stride();
			m_Buffer = new uint8_t[m_Size];

//			LOG_CORE_TRACE("DataBuffer() new: {0} (count: {1}, size: {2}, stride: {3}", (uint64_t)m_Buffer, count, m_Size, layout.Stride());
		}

		DataBuffer(void* data, uint32_t count, DataBufferLayout<T> layout)
			: m_BufferLayout(layout), m_Count(count)
		{
			m_Size = count * layout.Stride();
			m_Buffer = new uint8_t[m_Size];

			memcpy(m_Buffer, data, m_Size);

//			LOG_CORE_TRACE("DataBuffer() data: {0} (count: {1}, size: {2}, stride: {3}", (uint64_t)m_Buffer, count, m_Size, layout.Stride());
		}

		DataBuffer(DataBuffer& rhs)
		{
//			LOG_CORE_TRACE("DataBuffer() copy constructor");
		}

		DataBuffer(DataBuffer&& rhs)
		{
//			LOG_CORE_TRACE("DataBuffer() move constructor");
		}

//		DataBuffer(DataBuffer& rhs)
		DataBuffer& operator=(const DataBuffer& rhs)
		{
//			LOG_CORE_TRACE("DataBuffer copying");
			m_Size = rhs.m_Size;
			m_Count = rhs.m_Count;
			m_BufferLayout = rhs.m_BufferLayout;

			m_Buffer = rhs.m_Buffer;

//			LOG_CORE_TRACE("DataBuffer done copying, new databuffer: {0} (count: {1}, size: {2}, stride: {3}", (uint64_t)m_Buffer, m_Count, m_Size, m_BufferLayout.Stride());

			return *this;
		}

		DataBuffer& operator=(DataBuffer& rhs)
		{
//			LOG_CORE_TRACE("DataBuffer copying, nonconst");

			m_Size = rhs.m_Size;
			m_Count = rhs.m_Count;
			m_BufferLayout = rhs.m_BufferLayout;

			m_Buffer = rhs.m_Buffer;

//			LOG_CORE_TRACE("DataBuffer done copying, new databuffer: {0} (count: {1}, size: {2}, stride: {3}", (uint64_t)m_Buffer, m_Count, m_Size, m_BufferLayout.Stride());

			return *this;
		}

		DataBuffer& operator=(DataBuffer&& rhs)
		{
//			LOG_CORE_TRACE("Move operator!");

			m_Size = rhs.m_Size;
			m_Count = rhs.m_Count;
			m_BufferLayout = rhs.m_BufferLayout;

			m_Buffer = std::move(rhs.m_Buffer);
			rhs.m_Buffer = nullptr;

//			LOG_CORE_TRACE("Move operator done, new databuffer: {0}, old {1}", (uint64_t)m_Buffer, (uint64_t)rhs.m_Buffer);

			return *this;
		}

		virtual ~DataBuffer()
		{
			if (m_Buffer != nullptr)
			{
//				LOG_CORE_TRACE("Wants to delete databuffer: {0}", (uint64_t)m_Buffer);

				delete[] m_Buffer;
				m_Buffer = nullptr;

//				LOG_CORE_TRACE("Deleting databuffer: {0}", (uint64_t)m_Buffer);
			}
		}


		void* Data() { return m_Buffer; }

		uint32_t Count() { return m_Count; }
		uint32_t Size() { return m_Size; }

	private:
		void* m_Buffer = nullptr;

		uint32_t m_Size = 0;
		uint32_t m_Count = 0;

		DataBufferLayout<T> m_BufferLayout;
	};


}
