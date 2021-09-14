#pragma once


namespace Nut
{


	enum class DataType
	{
		Unknown = -1,
		Bool = 1,
		Int,
		UInt,
		Float,
		Vec2,
		Vec3,
		Vec4,
		Matrix3x3,
		Matrix4x4,
	};


	uint32_t DataTypeSize(DataType type);
	uint32_t DataTypeCount(DataType type);
	DataType StringToDataType(const std::string& type);

}
