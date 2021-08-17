#ifndef __NUTLIB_COMPONENTS_H
#define __NUTLIB_COMPONENTS_H



namespace Nut
{

	struct TagComponent
	{
		std::string Tag;

		TagComponent()
			: Tag("<none>")
		{

		}

		TagComponent(const std::string& tag)
			: Tag(tag)
		{

		}

	};


	struct TransformComponent
	{

		TransformComponent()
		{

		}

	};


	struct MeshComponent
	{


		MeshComponent()
		{

		}

	};


}


#endif
