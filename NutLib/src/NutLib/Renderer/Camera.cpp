#include "nutpch.h"
#include "Camera.h"

#include "NutLib/Core/Application.h"

#include "NutLib/Event/Events.h"

#include "NutLib/Core/Input.h"

#include <glm/gtc/matrix_transform.hpp>


namespace Nut
{


	Camera::Camera()
		: m_Position(0.0f, 3.0f, 5.0f), m_Rotation(0.0f)
	{
		auto [width, height] = Application::Get().GetWindow()->GetWindowSize();

		m_ProjectionMatrix = glm::perspective(glm::radians(45.0f), static_cast<float>(width) / static_cast<float>(height), 0.1f, 1000.0f);

		Update();



		SubscribeToEvent<MouseButtonPressed>([&](MouseButtonPressed& e)
			{
				if (e.Button() == MiddleButton)
				{
					m_Position.x += e.X();
					m_Position.y += e.Y();
				}

				Update();

				return false;
			});

	}

	void Camera::Update()
	{
		m_ViewMatrix = glm::lookAt(m_Position, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	}

}
