#pragma once



namespace Nut
{

	enum class ShaderType
	{
		Unknown = -1,
		Vertex = 1,
		Fragment,
		Pixel,
		Geometry,
		Compute,
	};

	std::string ShaderTypeToString(ShaderType type);
	ShaderType StringToShaderType(const std::string& type);

	std::unordered_map<ShaderType, std::string> GetShaderSourcesFromFile(const std::string& shaderFile);


	



}
