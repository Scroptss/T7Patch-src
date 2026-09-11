#include "Arxan.h"

#include "GameBuild.h"

#include <Windows.h>

#include <algorithm>
#include <array>
#include <atomic>
#include <cstdint>
#include <cstring>
#include <iterator>
#include <mutex>
#include <string>
#include <vector>

namespace
{
    constexpr std::uint8_t kPatchKey = 0x69;
    constexpr std::uintptr_t kReadySignalRva = 0x1686E948;
    constexpr std::size_t kCrc1ExpectedCount = 259;
    constexpr std::size_t kCrc2ExpectedCount = 259;
    constexpr std::size_t kCrc3ExpectedCount = 847;

    constexpr std::array<std::uint8_t, 6> kEncryptedPatch1 = {
        0x21, 0x58, 0xA0, 0xF9, 0xF9, 0xF9
    };
    constexpr std::array<std::uint8_t, 8> kEncryptedPatch2 = {
        0x21, 0x58, 0xA0, 0xF9, 0xF9, 0xF9, 0xF9, 0xF9
    };
    constexpr std::array<std::uint8_t, 6> kEncryptedPatch3 = {
        0x21, 0x58, 0xA9, 0x21, 0x58, 0xBB
    };

    constexpr std::array<std::uint8_t, 6> kChecksumPattern1 = {
        0x8B, 0x0C, 0x8B, 0x33, 0x0C, 0x82
    };
    constexpr std::array<std::uint8_t, 8> kChecksumPattern2 = {
        0x8B, 0x0C, 0x8B, 0xF7, 0xD9, 0x03, 0x0C, 0x82
    };
    constexpr std::array<std::uint8_t, 8> kChecksumPattern3 = {
        0x8B, 0x04, 0x82, 0x8B, 0x14, 0x8B, 0x3B, 0xC2
    };

    struct SavedPatch
    {
        std::uint8_t* address = nullptr;
        std::uint8_t size = 0;
        std::array<std::uint8_t, 8> original{};
        std::array<std::uint8_t, 8> replacement{};
    };

    std::atomic_bool g_active = false;
    std::atomic_bool g_external = false;
    std::atomic_bool g_waitingForReadySignal = false;
    std::mutex g_mutex;
    std::vector<SavedPatch> g_patches;

    bool executable_protection(DWORD protection)
    {
        if ((protection & (PAGE_GUARD | PAGE_NOACCESS)) != 0) return false;
        protection &= 0xff;
        return protection == PAGE_EXECUTE || protection == PAGE_EXECUTE_READ ||
            protection == PAGE_EXECUTE_READWRITE || protection == PAGE_EXECUTE_WRITECOPY;
    }

    bool memory_range(const void* address, std::size_t size, bool requireExecutable)
    {
        if (!address || size == 0) return false;

        MEMORY_BASIC_INFORMATION information{};
        if (VirtualQuery(address, &information, sizeof(information)) != sizeof(information)) return false;
        if (information.State != MEM_COMMIT ||
            (information.Protect & (PAGE_GUARD | PAGE_NOACCESS)) != 0 ||
            (requireExecutable && !executable_protection(information.Protect)))
            return false;

        const auto start = reinterpret_cast<std::uintptr_t>(address);
        const auto regionStart = reinterpret_cast<std::uintptr_t>(information.BaseAddress);
        return start >= regionStart && size <= information.RegionSize - (start - regionStart);
    }

    bool ready_signal_set()
    {
        const auto* signal = reinterpret_cast<const std::uintptr_t*>(bo3::image_base() + kReadySignalRva);
        return memory_range(signal, sizeof(*signal), false) && *signal != 0;
    }

    bool write_bytes(std::uint8_t* address, const std::uint8_t* bytes, std::size_t size)
    {
        DWORD oldProtection = 0;
        if (!VirtualProtect(address, size, PAGE_EXECUTE_READWRITE, &oldProtection)) return false;

        std::memcpy(address, bytes, size);
        const BOOL flushed = FlushInstructionCache(GetCurrentProcess(), address, size);

        DWORD ignored = 0;
        const BOOL restored = VirtualProtect(address, size, oldProtection, &ignored);
        return flushed && restored;
    }

    template <std::size_t PatternSize>
    std::vector<std::uint8_t*> find_pattern_in_executable_image(
        const std::array<std::uint8_t, PatternSize>& pattern)
    {
        std::vector<std::uint8_t*> matches;
        const auto moduleBase = bo3::image_base();
        const auto* dosHeader = reinterpret_cast<const IMAGE_DOS_HEADER*>(moduleBase);
        const auto* ntHeaders = reinterpret_cast<const IMAGE_NT_HEADERS64*>(
            moduleBase + static_cast<std::uintptr_t>(dosHeader->e_lfanew));
        const auto imageSize = static_cast<std::uintptr_t>(ntHeaders->OptionalHeader.SizeOfImage);
        const auto* sections = reinterpret_cast<const IMAGE_SECTION_HEADER*>(
            reinterpret_cast<const std::uint8_t*>(&ntHeaders->OptionalHeader) +
            ntHeaders->FileHeader.SizeOfOptionalHeader);

        for (std::uint16_t index = 0; index < ntHeaders->FileHeader.NumberOfSections; ++index)
        {
            const auto& section = sections[index];
            if ((section.Characteristics & IMAGE_SCN_MEM_EXECUTE) == 0 ||
                section.VirtualAddress >= imageSize)
                continue;

            const auto sectionStart = moduleBase + section.VirtualAddress;
            const auto remainingImage = imageSize - section.VirtualAddress;
            const auto sectionSize = (std::min)(
                static_cast<std::uintptr_t>(section.Misc.VirtualSize), remainingImage);
            const auto sectionEnd = sectionStart + sectionSize;
            auto cursor = sectionStart;

            while (cursor < sectionEnd)
            {
                MEMORY_BASIC_INFORMATION information{};
                if (VirtualQuery(reinterpret_cast<const void*>(cursor), &information,
                    sizeof(information)) != sizeof(information))
                    break;

                const auto regionStart = (std::max)(
                    cursor, reinterpret_cast<std::uintptr_t>(information.BaseAddress));
                const auto queriedRegionEnd = reinterpret_cast<std::uintptr_t>(information.BaseAddress) +
                    information.RegionSize;
                const auto regionEnd = (std::min)(sectionEnd, queriedRegionEnd);
                if (regionEnd <= cursor) break;

                if (information.State == MEM_COMMIT &&
                    executable_protection(information.Protect) &&
                    regionEnd - regionStart >= PatternSize)
                {
                    for (auto address = regionStart; address <= regionEnd - PatternSize; ++address)
                    {
                        auto* bytes = reinterpret_cast<std::uint8_t*>(address);
                        if (std::memcmp(bytes, pattern.data(), PatternSize) == 0)
                            matches.push_back(bytes);
                    }
                }

                cursor = regionEnd;
            }
        }

        return matches;
    }

    template <std::size_t PatchSize>
    std::array<std::uint8_t, PatchSize> decrypt_patch(
        const std::array<std::uint8_t, PatchSize>& encryptedPatch)
    {
        std::array<std::uint8_t, PatchSize> replacement{};
        for (std::size_t index = 0; index < PatchSize; ++index)
            replacement[index] = encryptedPatch[index] ^ kPatchKey;
        return replacement;
    }

    bool external_code_patches_installed()
    {
        const auto replacement1 = decrypt_patch(kEncryptedPatch1);
        const auto replacement2 = decrypt_patch(kEncryptedPatch2);
        const auto replacement3 = decrypt_patch(kEncryptedPatch3);

        // The first six bytes of replacement2 are replacement1.
        return find_pattern_in_executable_image(replacement1).size() >=
                kCrc1ExpectedCount + kCrc2ExpectedCount &&
            find_pattern_in_executable_image(replacement2).size() >= kCrc2ExpectedCount &&
            find_pattern_in_executable_image(replacement3).size() >= kCrc3ExpectedCount;
    }

    template <std::size_t PatternSize, std::size_t PatchSize>
    bool collect_pattern_patches(
        const std::array<std::uint8_t, PatternSize>& pattern,
        const std::array<std::uint8_t, PatchSize>& encryptedPatch,
        std::size_t expectedCount,
        std::string& message)
    {
        static_assert(PatchSize <= PatternSize);
        const auto replacement = decrypt_patch(encryptedPatch);
        const auto matches = find_pattern_in_executable_image(pattern);
        if (matches.size() != expectedCount)
        {
            message = "Arxan checksum pattern count mismatch: expected " +
                std::to_string(expectedCount) + ", found " + std::to_string(matches.size());
            return false;
        }

        for (auto* address : matches)
        {
            if (!memory_range(address, PatchSize, true))
            {
                message = "An Arxan checksum pattern was outside executable memory";
                return false;
            }

            SavedPatch patch{};
            patch.address = address;
            patch.size = static_cast<std::uint8_t>(PatchSize);
            std::memcpy(patch.original.data(), address, PatchSize);
            std::memcpy(patch.replacement.data(), replacement.data(), PatchSize);
            g_patches.push_back(patch);
        }
        return true;
    }

    void rollback_applied(std::size_t applied)
    {
        while (applied > 0)
        {
            --applied;
            auto& patch = g_patches[applied];
            write_bytes(patch.address, patch.original.data(), patch.size);
        }
        g_patches.clear();
    }
}

namespace arxan_bypass
{
    bool install(std::string& message)
    {
        std::lock_guard lock(g_mutex);
        if (g_active.load(std::memory_order_acquire))
        {
            message = "Arxan integrity bypass is active";
            return true;
        }

        if (!bo3::supported_build())
        {
            message = "This BO3 executable does not match a supported T7 Patch build";
            return false;
        }

        // Reuse a complete bypass installed by another module without taking
        // ownership of bytes that module must eventually restore.
        if (external_code_patches_installed())
        {
            g_external.store(true, std::memory_order_release);
            g_waitingForReadySignal.store(false, std::memory_order_release);
            g_active.store(true, std::memory_order_release);
            message = "Existing Arxan integrity bypass detected";
            return true;
        }

        g_patches.clear();
        g_patches.reserve(kCrc1ExpectedCount + kCrc2ExpectedCount + kCrc3ExpectedCount);
        if (!collect_pattern_patches(kChecksumPattern1, kEncryptedPatch1,
                kCrc1ExpectedCount, message) ||
            !collect_pattern_patches(kChecksumPattern2, kEncryptedPatch2,
                kCrc2ExpectedCount, message) ||
            !collect_pattern_patches(kChecksumPattern3, kEncryptedPatch3,
                kCrc3ExpectedCount, message))
        {
            g_patches.clear();
            return false;
        }

        std::size_t applied = 0;
        for (; applied < g_patches.size(); ++applied)
        {
            auto& patch = g_patches[applied];
            if (write_bytes(patch.address, patch.replacement.data(), patch.size)) continue;

            write_bytes(patch.address, patch.original.data(), patch.size);
            rollback_applied(applied);
            message = "Could not apply the complete Arxan integrity bypass";
            return false;
        }

        g_external.store(false, std::memory_order_release);
        g_waitingForReadySignal.store(true, std::memory_order_release);
        g_active.store(true, std::memory_order_release);
        message = "Arxan integrity code-patch bypass is active";
        return true;
    }

    void maintain()
    {
        if (!g_active.load(std::memory_order_acquire) ||
            g_external.load(std::memory_order_acquire) ||
            !g_waitingForReadySignal.load(std::memory_order_acquire) ||
            !ready_signal_set())
            return;

        std::lock_guard lock(g_mutex);
        if (!g_active.load(std::memory_order_acquire) ||
            !g_waitingForReadySignal.load(std::memory_order_acquire))
            return;

        for (auto& patch : g_patches)
        {
            if (std::memcmp(patch.address, patch.replacement.data(), patch.size) == 0) continue;
            if (!write_bytes(patch.address, patch.replacement.data(), patch.size)) return;
        }
        g_waitingForReadySignal.store(false, std::memory_order_release);
    }

    bool active()
    {
        return g_active.load(std::memory_order_acquire);
    }
}
