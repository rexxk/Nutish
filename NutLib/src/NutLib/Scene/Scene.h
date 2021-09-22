#ifndef __NUTLIB_SCENE_H
#define __NUTLIB_SCENE_H

#include "Entity.h"



namespace Nut
{

	class Scene
	{
	public:
		Scene();
		virtual ~Scene();

		UUID CreateEntity(const std::string& tag);
		void DeleteEntity(UUID id);

		void Draw();


	private:

		Ref<Registry> m_Registry;
		Ref<Entity> m_Entity;

	};

}


#endif
