#pragma once


#include <glm/glm.hpp>


namespace Nut
{


	class Camera
	{
	public:
		static Ref<Camera> Create(const glm::vec3& position);

		Camera(const glm::vec3& position);


		const glm::mat4&& GetViewProjectionMatrix() const { return m_ProjectionMatrix * m_ViewMatrix; }

		const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
		const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }

		const glm::vec3& GetPosition() const { return m_Position; }

	private:
		void CalculateViewMatrix();
		glm::vec3 CalculatePosition();

		glm::quat GetOrientation();
		
		glm::vec3 GetUpDirection();
		glm::vec3 GetRightDirection();
		glm::vec3 GetForwardDirection();

	private:
		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ProjectionMatrix;

		glm::vec3 m_Position;
		glm::vec3 m_Rotation;
		glm::vec3 m_LookAt;

		float m_Yaw = 0.0f;
		float m_Pitch = 0.0f;
		float m_Distance;

		float m_Exposure = 0.8f;

		float m_MouseSpeed = 0.01f;
	};


}
