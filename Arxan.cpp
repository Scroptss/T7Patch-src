#include "Arxan.h"

void PatchAddress(uint8_t* address, const uint8_t* patch, size_t patchSize)
{
	DWORD oldProtect;

	VirtualProtect(address, patchSize, PAGE_EXECUTE_READWRITE, &oldProtect);

	for (size_t i = 0; i < patchSize; i++)
		address[i] = patch[i];

	FlushInstructionCache(GetCurrentProcess(), address, patchSize);

	DWORD dummy;
	VirtualProtect(address, patchSize, oldProtect, &dummy);
}


template <size_t N>
struct ObfuscatedPatch {
	std::array<uint8_t, N> data;
	static constexpr uint8_t key = 0x69;

	constexpr ObfuscatedPatch(const uint8_t(&input)[N]) : data{} {
		for (size_t i = 0; i < N; ++i) {
			data[i] = input[i] ^ key;
		}
	}

	void Apply(uint8_t* address, void (*patchFunc)(uint8_t*, const uint8_t*, size_t)) const {
		uint8_t decrypted[N];
		for (size_t i = 0; i < N; ++i) {
			decrypted[i] = data[i] ^ key;
		}
		patchFunc(address, decrypted, N);

		memset(decrypted, 0, N);
	}
};

static constexpr ObfuscatedPatch patch_crc_1({ 0x48, 0x31, 0xC9, 0x90, 0x90, 0x90 });
static constexpr ObfuscatedPatch patch_crc_2({ 0x48, 0x31, 0xC9, 0x90, 0x90, 0x90, 0x90, 0x90 });
static constexpr ObfuscatedPatch patch_crc_3({ 0x48, 0x31, 0xC0, 0x48, 0x31, 0xD2 });

void PatchChecksumComparisons_Precomputed()
{
	for (auto rva : crc_patch_1)
		patch_crc_1.Apply((uint8_t*)(REBASE(rva)), PatchAddress);

	for (auto rva : crc_patch_2)
		patch_crc_2.Apply((uint8_t*)(REBASE(rva)), PatchAddress);

	for (auto rva : crc_patch_3)
		patch_crc_3.Apply((uint8_t*)(REBASE(rva)), PatchAddress);
}