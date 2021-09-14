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
		Double,
		Vec2,
		Vec3,
		Vec4,
		Matrix3x3,
		Matrix4x4,

		Texture2D,
		TextureCube,
	};


	uint32_t DataTypeSize(DataType type);
	uint32_t DataTypeCount(DataType type);
	DataType StringToDataType(const std::string& type);
	std::string DataTypeToString(DataType type);

}
