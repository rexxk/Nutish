#include "nutpch.h"
#include "StringHelpers.h"


namespace Nut
{
	std::vector<std::string> Tokenize(const std::string& str, char delimiter)
	{
		std::vector<std::string> tokens;

		size_t pos = 0;
		size_t nextPos = 0;

		while ((nextPos = str.find_first_of(delimiter, pos)) != std::string::npos)
		{
			std::string token = str.substr(pos, nextPos - pos);
			tokens.emplace_back(token);

			pos = nextPos + 1;
		}

		if (pos > 0)
		{
			std::string token = str.substr(pos);
			tokens.emplace_back(token);
		}

		return tokens;
	}

}
