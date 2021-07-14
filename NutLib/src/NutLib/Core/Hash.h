#ifndef __NUTLIB_HASH_H
#define __NUTLIB_HASH_H



namespace Nut
{

	class Hash
	{
	public:
		static uint32_t GenerateFNVHash(const char* str);
		static uint32_t GenerateFNVHash(const std::string& string);
	};

}


#endif
