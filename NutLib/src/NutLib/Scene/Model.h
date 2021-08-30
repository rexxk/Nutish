#pragma once

#include "Object.h"


namespace Nut
{


	class Model : public Object
	{
	public:
		static Ref<Model> Load(const std::string& filepath, Ref<Scene> scene);

		Model(Ref<Scene> scene, const std::string& tag = "model");
		virtual ~Model();


	private:



	};


}
