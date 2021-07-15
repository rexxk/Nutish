#include "nutpch.h"
#include "ShaderFile.h"


namespace Nut
{

	std::string ShaderTypeToString(ShaderType type)
	{
		switch (type)
		{
			case ShaderType::Vertex: return "vertex";
			case ShaderType::Pixel: return "pixel";
			case ShaderType::Fragment: return "fragment";
			case ShaderType::Geometry: return "geometry";
			case ShaderType::Compute: return "compute";
		}

		return std::string();
	}

	ShaderType StringToShaderType(const std::string& type)
	{
		if (type == "vertex") return ShaderType::Vertex;
		if (type == "pixel") return ShaderType::Pixel;
		if (type == "fragment") return ShaderType::Fragment;
		if (type == "geometry") return ShaderType::Geometry;
		if (type == "compute") return ShaderType::Compute;

		return ShaderType::Unknown;
	}

	std::unordered_map<ShaderType, std::string> GetShaderSourcesFromFile(const std::string& shaderFile)
	{
		std::unordered_map<ShaderType, std::string> shaderSources;

		std::ifstream fileStream(shaderFile, std::ios::in);

		NUT_CORE_ASSERT(fileStream, "ShaderFile: Unable to load shader file");

		fileStream.seekg(0, std::ios::end);
		size_t fileLength = fileStream.tellg();
		fileStream.seekg(0, std::ios::beg);

		std::string fileData;
		fileData.resize(fileLength);

		fileStream.read(fileData.data(), fileLength);

		fileStream.close();


		std::string tokenString = "#shadertype";
		size_t tokenStringLength = std::strlen(tokenString.c_str());

		size_t position = 0;


		while ((position = fileData.find(tokenString, position)) != std::string::npos)
		{
			size_t lineEndPos = fileData.find_first_of('\n', position);
			
			size_t tokenPos = position + tokenStringLength + 1;
			std::string shaderType = fileData.substr(tokenPos, lineEndPos - tokenPos);

			position = lineEndPos;

			size_t offset;

			if ((offset = fileData.find(tokenString, position)) != std::string::npos)
			{
				shaderSources[StringToShaderType(shaderType)] = fileData.substr(lineEndPos + 1, offset - (lineEndPos + 1));
			}
			else
			{
				shaderSources[StringToShaderType(shaderType)] = fileData.substr(lineEndPos + 1);
			}

		}

		return shaderSources;
	}


}


