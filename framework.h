#pragma once

#define WIN32_LEAN_AND_MEAN
#include <ctype.h>
#include <iostream>
#include <Winternl.h>
#include <unordered_set>
#include <filesystem>
#include "functional"
#include "Arxan.h"
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <windows.h>
#include <intrin.h>
#include <cstdint>
#include <cstddef>
#include <TlHelp32.h>
#include "structs.h"
#include "minhook/include/MinHook.h"
#include "gscu_hashing.h"
#include "offsets.h"
#include "Protection.h"
#include "Hooks.h"

constexpr uint32_t fnv_base_32 = 0x4B9ACE2F;

inline uint32_t fnv1a(const char* key) {

	const char* data = key;
	uint32_t hash = 0x4B9ACE2F;
	while (*data)
	{
		hash ^= *data;
		hash *= 0x1000193;
		data++;
	}
	hash *= 0x1000193; // bo3 wtf lol
	return hash;

}

template <unsigned __int32 NUM>
struct canon_const_builtins
{
	static const unsigned __int32 value = NUM;
};

constexpr unsigned __int32 fnv1a_const(const char* input)
{
	const char* data = input;
	uint32_t hash = 0x4B9ACE2F;
	while (*data)
	{
		hash ^= *data;
		hash *= 0x01000193;
		data++;
	}
	hash *= 0x01000193; // bo3 wtf lol
	return hash;
}

#define FNV32(x) canon_const_builtins<fnv1a_const(x)>::value

inline bool is_in_array(std::string cmp1, std::vector<std::string> cmp2)
{
	for (auto& cmp : cmp2)
	{
		if (!strcmp(cmp1.data(), cmp.data()))
			return true;
	}
	return false;
}

inline bool is_in_number_array(int cmp1, std::vector<int> cmp2)
{
	for (auto cmp : cmp2)
	{
		if (cmp1 == cmp)
			return true;
	}
	return false;
}

inline std::string to_lower(std::string text)
{
	std::transform(text.begin(), text.end(), text.begin(), ::tolower);
	return text;
}

inline bool is_equal(const std::string& lhs, const std::string& rhs, const std::size_t count, const bool case_sensitive)
{
	auto left = lhs;
	auto right = rhs;

	if (count != std::string::npos)
	{
		if (lhs.size() > count)
			left.erase(count);

		if (rhs.size() > count)
			right.erase(count);
	}

	if (case_sensitive)
	{
		return left == right;
	}

	else
	{
		return to_lower(left) == to_lower(right);
	}
}

inline bool is_address_within_range(std::uintptr_t address, std::uintptr_t min, std::uintptr_t max)
{
	return (address >= min && address <= max);
}

inline std::vector<std::string> legit_packets = {
	"connectResponse",
	"statresponse",
	"LM",
	"disconnect",
	"loadoutResponse",
	"infoResponse",
	"statusResponse",
	"keyAuthorize",
	"error",
	"print",
	"fastrestart",
	"ping",
	"pinga",
	"steamAuthReq",
	"cfl"
};

#define CRASH_LOG_NAME "crashes.log"
#define PATCH_CONFIG_LOCATION "t7patch.conf"
#define ZBR_WINDOW_TEXT "Call of Duty: Black Ops III (community patch by serious)"
#define ZBR_VERSION_FULL "Patch 3.01 - by serious <3"
