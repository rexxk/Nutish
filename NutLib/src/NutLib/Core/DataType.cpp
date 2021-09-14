#include "nutpch.h"
#include "DataType.h"



namespace Nut
{

	uint32_t DataTypeSize(DataType type)
	{
		switch (type)
		{
			case DataType::Bool: return 1;
			case DataType::Int: return 4;
			case DataType::UInt: return 4;
			case DataType::Float: return 4;
			case DataType::Vec2: return 4 * 2;
			case DataType::Vec3: return 4 * 3;
			case DataType::Vec4: return 4 * 4;
			case DataType::Matrix3x3: return 4 * 3 * 3;
			case DataType::Matrix4x4: return 4 * 4 * 4;
		}

		return 1;
	}

	uint32_t DataTypeCount(DataType type)
	{
		switch (type)
		{
			case DataType::Bool: return 1;
			case DataType::Int: return 1;
			case DataType::UInt: return 1;
			case DataType::Float: return 1;
			case DataType::Vec2: return 2;
			case DataType::Vec3: return 3;
			case DataType::Vec4: return 4;
		}

		return 0;
	}

	DataType StringToDataType(const std::string& type)
	{
		if (type == "bool") return DataType::Bool;
		if (type == "int") return DataType::Int;
		if (type == "uint") return DataType::UInt;
		if (type == "float") return DataType::Float;
		//		if (type == "double") return DataType::Double;
		if (type == "vec2") return DataType::Vec2;
		if (type == "vec3") return DataType::Vec3;
		if (type == "vec4") return DataType::Vec4;

		LOG_CORE_WARN("Unknown layout type in shader");

		return DataType::Unknown;
	}

}