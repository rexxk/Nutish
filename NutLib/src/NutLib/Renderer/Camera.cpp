#include "nutpch.h"
#include "Camera.h"

#include "NutLib/Core/Application.h"

#include "NutLib/Event/Events.h"

#include "NutLib/Core/Input.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>


namespace Nut
{

	Ref<Camera> Camera::Create(const glm::vec3& position)
	{
		return CreateRef<Camera>(position);
	}


	Camera::Camera(const glm::vec3& position)
		: m_Position(position), m_Rotation(0.0f), m_LookAt(0.0f)
	{
		auto [width, height] = Application::Get().GetWindow()->GetWindowSize();
		m_ProjectionMatrix = glm::perspective(glm::radians(45.0f), static_cast<float>(width) / static_cast<float>(height), 0.1f, 1000.0f);

		m_Distance = glm::distance(m_Position, m_LookAt);

		CalculateViewMatrix();

		SubscribeToEvent<WindowResizedEvent>([&](WindowResizedEvent& e)
			{
				if (e.Width() == 0 || e.Height() == 0)
					return false;

				m_ProjectionMatrix = glm::perspective(glm::radians(45.0f), static_cast<float>(e.Width()) / static_cast<float>(e.Height()), 0.1f, 1000.0f);

				CalculateViewMatrix();

				return false;
			});

		SubscribeToEvent<MouseScrolledEvent>([&](MouseScrolledEvent& e)
			{
				float direction = static_cast<float>(e.Direction());

//				LOG_CORE_TRACE("Mouse scrolled: {0}", direction);

				float distance = m_Distance * 0.2f;
				distance = std::max(distance, 0.0f);
				float zoomSpeed = distance * distance;
				zoomSpeed = std::min(zoomSpeed, 100.0f);

				if (distance < zoomSpeed)
					zoomSpeed /= 10;

				m_Distance = std::max(0.5f, m_Distance -= direction * zoomSpeed);

				LOG_CORE_TRACE("ZoomSpeed: {0}", zoomSpeed);

				CalculateViewMatrix();

				return false;
			});

		SubscribeToEvent<MouseMovedEvent>([&](MouseMovedEvent& e)
			{
				auto [dX, dY] = Input::MouseDelta();

				if (Input::IsMouseButtonPressed(MiddleButton))
				{
					// Pan
					m_LookAt += -GetRightDirection() * static_cast<float>(dX) * m_Distance * m_MouseSpeed;
					m_LookAt += GetUpDirection() * static_cast<float>(dY) * m_Distance * m_MouseSpeed;
				}

				if (Input::IsMouseButtonPressed(RightButton))
				{
					// Rotate
					float yawSign = GetUpDirection().y < 0 ? -1.0f : 1.0f;
					m_Yaw += yawSign * static_cast<float>(dX) * m_MouseSpeed;
					m_Pitch += static_cast<float>(dY) * m_MouseSpeed;
				}

				CalculateViewMatrix();

				return false;
			});

	}

	void Camera::CalculateViewMatrix()
	{
		m_Position = CalculatePosition();
		m_Rotation = glm::eulerAngles(GetOrientation() * (180.0f / glm::pi<float>()));

		m_ViewMatrix = glm::translate(glm::mat4(1.0f), m_Position) * glm::toMat4(GetOrientation());
		m_ViewMatrix = glm::inverse(m_ViewMatrix);

//		m_ViewMatrix = glm::lookAt(m_Position, m_LookAt, glm::vec3(0.0f, 1.0f, 0.0f));
	}

	glm::vec3 Camera::CalculatePosition()
	{
		return m_LookAt - GetForwardDirection() * m_Distance;
	}

	glm::quat Camera::GetOrientation()
	{
		return glm::quat(glm::vec3(-m_Pitch, -m_Yaw, 0.0f));
	}

	glm::vec3 Camera::GetUpDirection()
	{
		return glm::rotate(GetOrientation(), glm::vec3(0.0f, 1.0f, 0.0f));
	}

	glm::vec3 Camera::GetRightDirection()
	{
		return glm::rotate(GetOrientation(), glm::vec3(1.0f, 0.0f, 0.0f));
	}

	glm::vec3 Camera::GetForwardDirection()
	{
		return glm::rotate(GetOrientation(), glm::vec3(0.0f, 0.0f, -1.0f));
	}

}
