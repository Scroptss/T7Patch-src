#include "framework.h"
#include "CampaignFixes.h"

namespace campaign_fixes
{
    namespace
    {
        constexpr std::uint32_t DamageOverrideFunction = 0x56F9AB2E;
        constexpr std::uint32_t InfectionZombiesNamespace = 0xB0A87E94;

        constexpr std::uint8_t CompiledScriptMagicPrefix[] =
        {
            0x80, 0x47, 0x53, 0x43, 0x0D, 0x0A, 0x00
        };

        // Decrypted 0x1B bytecode: self.numhits === 5.
        constexpr std::uint8_t OriginalCondition1B[] =
        {
            0xD9, 0xCA, 0xB7, 0x6E, 0xE2, 0x05, 0x28
        };

        // Decrypted 0x1B bytecode: self.health <= idamage.
        constexpr std::uint8_t LethalDamageCondition1B[] =
        {
            0x6B, 0xF1, 0x90, 0x3A, 0x5D, 0x08, 0x42
        };

        // Protected 0x1C bytecode from the stock campaign fastfile. Opcodes are
        // two-byte, per-site encodings; the replacement encodings were copied
        // from matching operations in this same script and verified with ACTS.
        constexpr std::uint8_t OriginalCondition1C[] =
        {
            0xD9, 0xCA, 0xB7, 0x6E, // self.numhits field hash
            0xA2, 0x03, 0x05, 0x34, // GetByte 5
            0x0D, 0x12              // SuperEqual
        };

        constexpr std::uint8_t LethalDamageCondition1C[] =
        {
            0x6B, 0xF1, 0x90, 0x3A, // self.health field hash
            0xBB, 0x09, 0x08, 0x51, // EvalLocalVariableCached idamage
            0x53, 0x0E              // LessThanOrEqualTo
        };

        struct DamagePatch
        {
            std::uint8_t scriptVersion;
            std::uint32_t checksum;
            std::uint32_t minimumFunctionSize;
            std::uint32_t conditionOffset;
            const std::uint8_t* originalCondition;
            const std::uint8_t* replacementCondition;
            std::size_t conditionSize;
        };

        constexpr DamagePatch DamagePatch1B
        {
            0x1B, 0xC5BA252D, 0xD5, 0xB4,
            OriginalCondition1B, LethalDamageCondition1B, sizeof(OriginalCondition1B)
        };

        constexpr DamagePatch DamagePatch1C
        {
            0x1C, 0x6BA4A2E4, 0xFA, 0xC4,
            OriginalCondition1C, LethalDamageCondition1C, sizeof(OriginalCondition1C)
        };

        struct ScriptExport
        {
            std::uint32_t checksum;
            std::uint32_t bytecodeOffset;
            std::uint32_t functionName;
            std::uint32_t functionNamespace;
            std::uint32_t parameterFlags;
        };

        static_assert(sizeof(ScriptExport) == 0x14);

        using ScrGscObjLink = std::int64_t(__fastcall*)(int instance, char* scriptObject);

        ScrGscObjLink Scr_GscObjLink = nullptr;
        SRWLOCK PatchLock = SRWLOCK_INIT;

        const DamagePatch* GetDamagePatch(std::uint8_t scriptVersion)
        {
            if (scriptVersion == DamagePatch1B.scriptVersion)
                return &DamagePatch1B;
            if (scriptVersion == DamagePatch1C.scriptVersion)
                return &DamagePatch1C;
            return nullptr;
        }

        void PatchDemonWithinDamageOverride(std::uint8_t* buffer)
        {
            if (!buffer)
                return;

            AcquireSRWLockExclusive(&PatchLock);

            if (memcmp(buffer, CompiledScriptMagicPrefix,
                sizeof(CompiledScriptMagicPrefix)))
            {
                ReleaseSRWLockExclusive(&PatchLock);
                return;
            }

            const auto* damagePatch = GetDamagePatch(buffer[7]);
            if (!damagePatch)
            {
                ReleaseSRWLockExclusive(&PatchLock);
                return;
            }

            // The compiled-script header stores its complete buffer size here.
            const auto bufferSize = *reinterpret_cast<const std::uint32_t*>(buffer + 0x28);
            if (bufferSize < 0x40 || bufferSize > 64 * 1024 * 1024)
            {
                ReleaseSRWLockExclusive(&PatchLock);
                return;
            }

            const auto exportTableOffset = *reinterpret_cast<const std::uint32_t*>(buffer + 0x20);
            const auto exportCount = *reinterpret_cast<const std::uint16_t*>(buffer + 0x3A);
            const auto exportTableSize = static_cast<std::uint64_t>(exportCount) * sizeof(ScriptExport);

            if (exportCount > 4096 || exportTableOffset > bufferSize ||
                exportTableSize > static_cast<std::uint64_t>(bufferSize - exportTableOffset))
            {
                ReleaseSRWLockExclusive(&PatchLock);
                return;
            }

            auto* exports = reinterpret_cast<const ScriptExport*>(buffer + exportTableOffset);
            for (std::uint16_t index = 0; index < exportCount; ++index)
            {
                const auto& scriptExport = exports[index];
                if (scriptExport.functionName != DamageOverrideFunction ||
                    scriptExport.functionNamespace != InfectionZombiesNamespace ||
                    scriptExport.checksum != damagePatch->checksum)
                {
                    continue;
                }

                if (scriptExport.bytecodeOffset > bufferSize ||
                    damagePatch->minimumFunctionSize >
                        bufferSize - scriptExport.bytecodeOffset)
                {
                    break;
                }

                auto* bytecode = buffer + scriptExport.bytecodeOffset;
                auto* patchAddress = bytecode + damagePatch->conditionOffset;
                if (!memcmp(patchAddress, damagePatch->replacementCondition,
                    damagePatch->conditionSize))
                    break;

                if (memcmp(patchAddress, damagePatch->originalCondition,
                    damagePatch->conditionSize))
                    break;

                DWORD oldProtection = 0;
                if (VirtualProtect(patchAddress, damagePatch->conditionSize,
                    PAGE_EXECUTE_READWRITE, &oldProtection))
                {
                    memcpy(patchAddress, damagePatch->replacementCondition,
                        damagePatch->conditionSize);
                    FlushInstructionCache(GetCurrentProcess(), patchAddress,
                        damagePatch->conditionSize);

                    DWORD ignored = 0;
                    VirtualProtect(patchAddress, damagePatch->conditionSize,
                        oldProtection, &ignored);
                    OutputDebugStringA("T7 Patch: fixed Demon Within Realistic damage transition.\n");
                }
                break;
            }

            ReleaseSRWLockExclusive(&PatchLock);
        }

        std::int64_t __fastcall Scr_GscObjLinkHook(int instance, char* scriptObject)
        {
            // Some stock campaign scripts are handed directly to this linker and
            // never pass through a named DB_FindXAssetHeader lookup. Patch only
            // after linking so BO3 has finished resolving field identifiers.
            const auto result = Scr_GscObjLink(instance, scriptObject);
            PatchDemonWithinDamageOverride(reinterpret_cast<std::uint8_t*>(scriptObject));
            return result;
        }

    }

    void InstallHooks()
    {
        // February and September 2026 builds share this RVA (it is before the
        // September code-section deletion handled by GameBuild::translate_rva).
        MH_CreateHook(reinterpret_cast<void*>(REBASE(0x12CC320)),
            Scr_GscObjLinkHook, reinterpret_cast<void**>(&Scr_GscObjLink));
    }
}
