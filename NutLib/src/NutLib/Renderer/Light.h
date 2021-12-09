#pragma once

#include <glm/glm.hpp>


namespace Nut
{


	enum class LightType
	{
		Unknown = 0,
		Ambient,
		Area,
		Directional,
		Point,
		Spot,
	};



	struct LightProperties
	{
		LightType Type = LightType::Unknown;

		glm::vec3 Position = glm::vec3(0.0f);
		glm::vec3 Direction = glm::vec3(0.0f);

		glm::vec3 Ambient = glm::vec3(0.0f);
		glm::vec3 Diffuse = glm::vec3(0.0f);
		glm::vec3 Specular = glm::vec3(0.0f);

		float AttenuationConstant = 0.0f;
		float AttenuationLinear = 0.0f;
		float AttenuationQuadratic = 0.0f;

		float AngleInnerCone = 0.0f;
		float AngleOuterCone = 0.0f;
	};


	class Light
	{
	public:
		Light();
		Light(const LightProperties& props);

		LightProperties& GetProperties() { return m_Properties; }

	private:

		LightProperties m_Properties;

	};


}
