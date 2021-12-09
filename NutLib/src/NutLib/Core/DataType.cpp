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
			case DataType::Double: return 4;
			case DataType::Vec2: return 4 * 2;
			case DataType::Vec3: return 4 * 3;
			case DataType::Vec4: return 4 * 4;
			case DataType::Matrix3x3: return 4 * 3 * 3;
			case DataType::Matrix4x4: return 4 * 4 * 4;

			case DataType::Texture2D: return 4;
			case DataType::TextureCube: return 4;

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
			case DataType::Double: return 1;
			case DataType::Vec2: return 2;
			case DataType::Vec3: return 3;
			case DataType::Vec4: return 4;

			case DataType::Matrix3x3: return 1;
			case DataType::Matrix4x4: return 1;

			case DataType::Texture2D: return 1;
			case DataType::TextureCube: return 1;
		}

		return 0;
	}

	DataType StringToDataType(const std::string& type)
	{
		if (type == "bool") return DataType::Bool;
		if (type == "int") return DataType::Int;
		if (type == "uint") return DataType::UInt;
		if (type == "float") return DataType::Float;
		if (type == "double") return DataType::Double;

		if (type == "vec2") return DataType::Vec2;
		if (type == "vec3") return DataType::Vec3;
		if (type == "vec4") return DataType::Vec4;

		if (type == "mat3") return DataType::Matrix3x3;
		if (type == "mat4") return DataType::Matrix4x4;

		if (type == "sampler2D") return DataType::Texture2D;
		if (type == "samplerCube") return DataType::TextureCube;

		if (type == "custom") return DataType::CustomType;

//		LOG_CORE_WARN("Unknown datatype: {0}", type.c_str());

		return DataType::Unknown;
	}

	std::string DataTypeToString(DataType type)
	{
		switch (type)
		{
			case DataType::Bool: return "bool";
			case DataType::Int: return "int";
			case DataType::UInt: return "uint";
			case DataType::Float: return "float";
			case DataType::Double: return "double";

			case DataType::Vec2: return "vec2";
			case DataType::Vec3: return "vec3";
			case DataType::Vec4: return "vec4";

			case DataType::Matrix3x3: return "mat3";
			case DataType::Matrix4x4: return "mat4";

			case DataType::Texture2D: return "sampler2D";
			case DataType::TextureCube: return "samplerCube";

			case DataType::CustomType: return "custom";
		}

		return "<unknown datatype>";
	}
}
