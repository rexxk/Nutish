#ifndef __NUTLIB_SCENE_H
#define __NUTLIB_SCENE_H


#include "NutLib/Scene/Entity.h"
#include "NutLib/Scene/Components.h"


namespace Nut
{

	class Scene
	{
	public:
		Scene();
		virtual ~Scene();

		Entity CreateEntity(const std::string& tag);
		void DeleteEntity(const Entity& entity);

		void Draw();


		Ref<ECS::Registry> GetRegistry() { return m_Registry; }

	private:

		Ref<ECS::Registry> m_Registry;
		Ref<ECS::EntitySystem> m_EntitySystem;

	};

}


#endif
