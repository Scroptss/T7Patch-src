#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <cstdint>

namespace bo3
{
    enum class Build
    {
        Unknown,
        February2026,
        September2026
    };

    inline constexpr std::uint32_t February2026TimeDateStamp = 0x693D731E;
    inline constexpr std::uint32_t February2026RetailImageSize = 0x1D74AC00;
    inline constexpr std::uint32_t February2026DumpImageSize = 0x1D74B000;
    inline constexpr std::uint32_t September2026TimeDateStamp = 0x6A7B6355;
    inline constexpr std::uint32_t September2026RetailImageSize = 0x1D75BC00;
    inline constexpr std::uint32_t September2026DumpImageSize = 0x1D75C000;

    // The September update removed 0x6C0 bytes from this later part of the
    // primary code section. The supplied February-to-September dump mapping
    // confirms the same delta for every T7 Patch code address in this range.
    inline constexpr std::uintptr_t SeptemberShiftStartRva = 0x1D29C20;
    inline constexpr std::uintptr_t PrimaryCodeEndRva = 0x2EFF000;
    inline constexpr std::uintptr_t SeptemberCodeDelta = 0x6C0;

    inline std::uintptr_t image_base()
    {
        return reinterpret_cast<std::uintptr_t>(GetModuleHandleW(nullptr));
    }

    inline Build detect_build()
    {
        const auto base = image_base();
        if (!base) return Build::Unknown;

        const auto* dosHeader = reinterpret_cast<const IMAGE_DOS_HEADER*>(base);
        if (dosHeader->e_magic != IMAGE_DOS_SIGNATURE || dosHeader->e_lfanew <= 0)
            return Build::Unknown;

        const auto* ntHeaders = reinterpret_cast<const IMAGE_NT_HEADERS64*>(
            base + static_cast<std::uintptr_t>(dosHeader->e_lfanew));
        if (ntHeaders->Signature != IMAGE_NT_SIGNATURE ||
            ntHeaders->FileHeader.Machine != IMAGE_FILE_MACHINE_AMD64)
            return Build::Unknown;

        const auto timestamp = ntHeaders->FileHeader.TimeDateStamp;
        const auto imageSize = ntHeaders->OptionalHeader.SizeOfImage;
        // IDA's reconstructed memory images round SizeOfImage to the next
        // page, while the retail executable retains its 0x200 alignment.
        // Accept both fingerprints; timestamp and RVA layout are identical.
        if (timestamp == September2026TimeDateStamp &&
            (imageSize == September2026RetailImageSize ||
                imageSize == September2026DumpImageSize))
            return Build::September2026;
        if (timestamp == February2026TimeDateStamp &&
            (imageSize == February2026RetailImageSize ||
                imageSize == February2026DumpImageSize))
            return Build::February2026;
        return Build::Unknown;
    }

    inline Build current_build()
    {
        static const Build build = detect_build();
        return build;
    }

    inline bool supported_build()
    {
        return current_build() != Build::Unknown;
    }

    constexpr std::uintptr_t translate_rva(std::uintptr_t rva, Build build)
    {
        if (build == Build::September2026 &&
            rva >= SeptemberShiftStartRva && rva < PrimaryCodeEndRva)
            return rva - SeptemberCodeDelta;
        return rva;
    }

    inline std::uintptr_t address(std::uintptr_t februaryRva)
    {
        return image_base() + translate_rva(februaryRva, current_build());
    }

    static_assert(translate_rva(0x1DECFD0, Build::September2026) == 0x1DEC910);
    static_assert(translate_rva(0x226B0A0, Build::September2026) == 0x226A9E0);
    static_assert(translate_rva(0x1686E948, Build::September2026) == 0x1686E948);
    static_assert(translate_rva(0x1DECFD0, Build::February2026) == 0x1DECFD0);
}
