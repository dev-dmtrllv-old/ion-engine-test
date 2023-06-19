#pragma once

#include "pch.hpp"

namespace ion
{
	using Hash = std::size_t;

	namespace Hasher
	{
		constexpr Hash hash(const char* str)
		{
			std::size_t hash = 5381;
			int c;

			while ((c = *str++))
				hash = ((hash << 5) + hash) * 33 + c;

			return hash;
		}

		template<typename T>
		constexpr Hash hashType()
		{
			return hash(typeid(T).name());
		}

		constexpr bool equals(Hash& hashStr, const char* str) { return hash(str) == hashStr; }
	};
};