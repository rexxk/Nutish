#pragma once


#include <glm/glm.hpp>


namespace Nut
{


	class Camera
	{
	public:
		Camera();

		void Update();




		glm::mat4 GetViewProjection() const
		{
			return m_ProjectionMatrix * m_ViewMatrix;
		}


	private:
		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ProjectionMatrix;

		glm::vec3 m_Position;
		glm::vec3 m_Rotation;

	};


}
