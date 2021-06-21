#include "nutpch.h"
#include "Hash.h"



namespace Nut
{

	uint32_t Hash::GenerateFNVHash(const char* str)
	{
		constexpr uint32_t FNV_PRIME = 16777619u;
		constexpr uint32_t OFFSET_BASIS = 2166136261u;

		const size_t length = strlen(str) + 1;

		uint32_t hash = OFFSET_BASIS;

		for (size_t i = 0; i < length; i++)
		{
			hash ^= *str++;
			hash *= FNV_PRIME;
		}

		return hash;
	}

	uint32_t Hash::GenerateFNVHash(const std::string& str)
	{
		return GenerateFNVHash(str.c_str());
	}

}
