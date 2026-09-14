#include "framework.h"
#include "LuaSymbols.h"

#include <charconv>
#include <limits>
#include <string_view>

namespace lua_symbols
{
    namespace
    {
        constexpr std::uintptr_t LuaStateManagerErrorRva = 0x1F05CE0;
        constexpr std::uintptr_t HksiLuaGetInfoRva = 0x1D41500;
        constexpr std::uintptr_t HksObjectToStringRva = 0x1D3F2F0;
        constexpr std::uintptr_t LuaPushLStringRva = 0x0A18430;
        constexpr std::uintptr_t ActiveModNameRva = 0x1670DD84;

        constexpr std::size_t LuaStateTopOffset = 0x48;
        constexpr std::size_t LuaStateBaseOffset = 0x50;
        constexpr std::size_t HksObjectSize = 0x10;
        constexpr std::size_t MaximumModNameLength = 64;
        constexpr std::uintmax_t MaximumDatabaseFileSize = 16 * 1024 * 1024;
        constexpr std::size_t MaximumDatabaseEntries = 1'000'000;
        constexpr std::size_t MaximumStackTraceSize = 1024 * 1024;
        constexpr wchar_t StockSymbolDatabaseName[] = L"t7patch.stockluadb";

        struct SymbolRecord
        {
            std::string source;
            std::uint32_t sourceLine;
            std::string functionName;

            bool operator==(const SymbolRecord&) const = default;
        };

        struct StockSymbolRecord
        {
            std::string source;
            std::string functionName;

            bool operator==(const StockSymbolRecord&) const = default;
        };

        struct StockPrototypeRecord
        {
            std::uint32_t codeOffset;
            std::string source;
            std::string functionName;
        };

        using SymbolMap = std::unordered_map<std::uint64_t, SymbolRecord>;
        using StockSymbolMap = std::unordered_map<std::uint32_t, StockSymbolRecord>;
        using StockPrototypeMap = std::unordered_map<std::uint32_t,
            std::vector<StockPrototypeRecord>>;

        struct StockDatabases
        {
            StockSymbolMap symbols;
            StockPrototypeMap prototypes;
        };

        using LuaStateManagerError = void(__fastcall*)(const char* error, std::int64_t* luaState);
        using HksiLuaGetInfo = int(__fastcall*)(std::int64_t* luaState,
            const char* what, void* debugRecord);
        using HksObjectToString = const char*(__fastcall*)(std::int64_t* luaState,
            std::int64_t object, std::size_t* length);
        using LuaPushLString = void(__fastcall*)(std::int64_t* luaState,
            const char* value, std::size_t length);

        LuaStateManagerError OriginalLuaStateManagerError = nullptr;
        HksiLuaGetInfo OriginalHksiLuaGetInfo = nullptr;
        SRWLOCK SymbolLock = SRWLOCK_INIT;

        class ExclusiveLock
        {
        public:
            explicit ExclusiveLock(SRWLOCK& lock) : lock_(lock)
            {
                AcquireSRWLockExclusive(&lock_);
            }

            ~ExclusiveLock()
            {
                ReleaseSRWLockExclusive(&lock_);
            }

            ExclusiveLock(const ExclusiveLock&) = delete;
            ExclusiveLock& operator=(const ExclusiveLock&) = delete;

        private:
            SRWLOCK& lock_;
        };

        std::uint64_t MakeSymbolKey(std::uint32_t functionHash,
            std::uint32_t instructionIndex)
        {
            return (static_cast<std::uint64_t>(functionHash) << 32) |
                instructionIndex;
        }

        bool ParseUInt32(std::string_view value, std::uint32_t& result)
        {
            if (value.empty())
                return false;

            const auto parsed = std::from_chars(value.data(),
                value.data() + value.size(), result);
            return parsed.ec == std::errc{} && parsed.ptr == value.data() + value.size();
        }

        bool IsSafeModName(std::string_view name)
        {
            if (name.empty() || name.size() >= MaximumModNameLength)
                return false;

            for (const auto character : name)
            {
                const auto lower = character >= 'a' && character <= 'z';
                const auto upper = character >= 'A' && character <= 'Z';
                const auto digit = character >= '0' && character <= '9';
                if (!lower && !upper && !digit && character != '_' && character != '-')
                    return false;
            }
            return true;
        }

        std::string GetActiveModName()
        {
            const auto* name = reinterpret_cast<const char*>(REBASE(ActiveModNameRva));
            const auto length = strnlen_s(name, MaximumModNameLength);
            if (length == 0 || length >= MaximumModNameLength)
                return {};

            const std::string result(name, length);
            return IsSafeModName(result) ? result : std::string{};
        }

        std::filesystem::path GetGameDirectory()
        {
            std::wstring executablePath(32768, L'\0');
            const auto length = GetModuleFileNameW(nullptr, executablePath.data(),
                static_cast<DWORD>(executablePath.size()));
            if (length == 0 || length >= executablePath.size())
                return {};

            executablePath.resize(length);
            return std::filesystem::path(executablePath).parent_path();
        }

        std::string DisplaySource(std::string source)
        {
            if (!source.empty() && source.front() == '@')
                source.erase(source.begin());
            std::replace(source.begin(), source.end(), '\\', '/');
            return source;
        }

        bool ParseSymbolLine(std::string_view line, std::uint64_t& key,
            SymbolRecord& record)
        {
            if (!line.empty() && line.back() == '\r')
                line.remove_suffix(1);

            std::size_t commas[4]{};
            auto searchFrom = std::size_t{};
            for (auto& comma : commas)
            {
                comma = line.find(',', searchFrom);
                if (comma == std::string_view::npos)
                    return false;
                searchFrom = comma + 1;
            }

            std::uint32_t functionHash = 0;
            std::uint32_t instructionIndex = 0;
            std::uint32_t sourceLine = 0;
            if (!ParseUInt32(line.substr(0, commas[0]), functionHash) ||
                !ParseUInt32(line.substr(commas[0] + 1,
                    commas[1] - commas[0] - 1), instructionIndex) ||
                !ParseUInt32(line.substr(commas[2] + 1,
                    commas[3] - commas[2] - 1), sourceLine))
            {
                return false;
            }

            auto source = DisplaySource(std::string(line.substr(commas[1] + 1,
                commas[2] - commas[1] - 1)));
            if (source.empty())
                return false;

            key = MakeSymbolKey(functionHash, instructionIndex);
            record = SymbolRecord
            {
                std::move(source),
                sourceLine,
                std::string(line.substr(commas[3] + 1))
            };
            return true;
        }

        void LoadSymbolDatabase(const std::filesystem::path& path,
            SymbolMap& symbols, std::unordered_set<std::uint64_t>& ambiguousKeys)
        {
            std::error_code error;
            const auto fileSize = std::filesystem::file_size(path, error);
            if (error || fileSize == 0 || fileSize > MaximumDatabaseFileSize)
                return;

            std::ifstream database(path, std::ios::binary);
            if (!database)
                return;

            std::string line;
            while (symbols.size() < MaximumDatabaseEntries && std::getline(database, line))
            {
                std::uint64_t key = 0;
                SymbolRecord record;
                if (!ParseSymbolLine(line, key, record) || ambiguousKeys.contains(key))
                    continue;

                const auto existing = symbols.find(key);
                if (existing == symbols.end())
                {
                    symbols.emplace(key, std::move(record));
                }
                else if (!(existing->second == record))
                {
                    symbols.erase(existing);
                    ambiguousKeys.emplace(key);
                }
            }
        }

        SymbolMap LoadSymbolsForActiveMod()
        {
            SymbolMap symbols;
            const auto modName = GetActiveModName();
            const auto gameDirectory = GetGameDirectory();
            if (modName.empty() || gameDirectory.empty())
                return symbols;

            const auto modDirectory = gameDirectory / L"mods" /
                std::filesystem::path(modName);
            std::error_code error;
            if (!std::filesystem::is_directory(modDirectory, error) || error)
                return symbols;

            std::unordered_set<std::uint64_t> ambiguousKeys;
            std::filesystem::recursive_directory_iterator iterator(modDirectory,
                std::filesystem::directory_options::skip_permission_denied, error);
            const std::filesystem::recursive_directory_iterator end;

            while (!error && iterator != end && symbols.size() < MaximumDatabaseEntries)
            {
                if (iterator->is_regular_file(error) && !error &&
                    _wcsicmp(iterator->path().extension().c_str(), L".luacallstackdb") == 0)
                {
                    LoadSymbolDatabase(iterator->path(), symbols, ambiguousKeys);
                }

                error.clear();
                iterator.increment(error);
            }
            return symbols;
        }

        StockDatabases LoadStockDatabases()
        {
            StockDatabases databases;
            const auto gameDirectory = GetGameDirectory();
            if (gameDirectory.empty())
                return databases;

            const auto databasePath = gameDirectory / StockSymbolDatabaseName;
            std::error_code error;
            const auto fileSize = std::filesystem::file_size(databasePath, error);
            if (error || fileSize == 0 || fileSize > MaximumDatabaseFileSize)
                return databases;

            std::ifstream database(databasePath, std::ios::binary);
            if (!database)
                return databases;

            std::unordered_set<std::uint32_t> ambiguousHashes;
            std::string line;
            std::size_t entries = 0;
            while (entries < MaximumDatabaseEntries && std::getline(database, line))
            {
                if (!line.empty() && line.back() == '\r')
                    line.pop_back();
                if (line.empty() || line.front() == '#')
                    continue;

                const auto firstComma = line.find(',');
                if (firstComma == std::string::npos || firstComma == 0 ||
                    firstComma + 1 >= line.size())
                    continue;

                std::uint32_t functionHash = 0;
                if (!ParseUInt32(std::string_view(line).substr(0, firstComma), functionHash) ||
                    functionHash == 0 || ambiguousHashes.contains(functionHash))
                {
                    continue;
                }

                const auto secondComma = line.find(',', firstComma + 1);
                const auto sourceEnd = secondComma == std::string::npos ?
                    line.size() : secondComma;
                auto source = DisplaySource(line.substr(firstComma + 1,
                    sourceEnd - firstComma - 1));
                if (source.empty() || source.size() > 4096)
                    continue;

                std::uint32_t codeOffset = 0;
                auto functionName = std::string{};
                auto hasPrototypeOffset = false;
                if (secondComma != std::string::npos)
                {
                    const auto thirdComma = line.find(',', secondComma + 1);
                    if (thirdComma != std::string::npos &&
                        ParseUInt32(std::string_view(line).substr(secondComma + 1,
                            thirdComma - secondComma - 1), codeOffset))
                    {
                        hasPrototypeOffset = codeOffset != 0;
                        functionName = line.substr(thirdComma + 1);
                    }
                    else
                    {
                        // Version 2 stored the function name directly after
                        // the source path. Version 1 had no second comma.
                        functionName = line.substr(secondComma + 1);
                    }
                }
                if (functionName.size() > 4096)
                    continue;

                StockSymbolRecord record
                {
                    std::move(source),
                    std::move(functionName)
                };

                if (hasPrototypeOffset && !record.functionName.empty())
                {
                    databases.prototypes[functionHash].push_back(
                        StockPrototypeRecord
                        {
                            codeOffset,
                            record.source,
                            record.functionName
                        });
                }

                const auto existing = databases.symbols.find(functionHash);
                if (existing == databases.symbols.end())
                {
                    databases.symbols.emplace(functionHash, std::move(record));
                }
                else if (existing->second.source != record.source)
                {
                    databases.symbols.erase(existing);
                    databases.prototypes.erase(functionHash);
                    ambiguousHashes.emplace(functionHash);
                }
                else if (existing->second.functionName != record.functionName)
                {
                    // The hash identifies this source but is shared by more
                    // than one closure. Live prototype matching may still
                    // recover the exact name; the text-only fallback may not.
                    existing->second.functionName.clear();
                }
                ++entries;
            }
            return databases;
        }

        const StockDatabases& GetStockDatabases()
        {
            static const auto databases = LoadStockDatabases();
            return databases;
        }

        bool TryGetLivePrototype(std::int64_t* luaState, void* debugRecord,
            std::uint32_t& functionHash, std::uintptr_t& instructions)
        {
            // Keep structured exception handling isolated from C++ objects.
            // A malformed third-party Lua VM must fall back to the stock trace
            // instead of turning symbol recovery into a game crash.
            __try
            {
                if (!luaState || !debugRecord)
                    return false;

                const auto state = reinterpret_cast<std::uintptr_t>(luaState);
                const auto debug = reinterpret_cast<std::uintptr_t>(debugRecord);
                const auto records = *reinterpret_cast<const std::uintptr_t*>(state + 0x18);
                const auto current = *reinterpret_cast<const std::uintptr_t*>(state + 0x28);
                const auto level = *reinterpret_cast<const std::int32_t*>(debug + 0x240);
                const auto isTailCall = *reinterpret_cast<const std::int32_t*>(debug + 0x244);

                if (!records || current < records || level < 0 || isTailCall)
                    return false;

                const auto callStackBytes = current - records;
                if (callStackBytes % 24 != 0)
                    return false;

                const auto currentLevel = callStackBytes / 24;
                if (currentLevel > 4096 || static_cast<std::uintptr_t>(level) > currentLevel)
                    return false;

                const auto functionBase = static_cast<std::uintptr_t>(level) == currentLevel ?
                    *reinterpret_cast<const std::uintptr_t*>(state + 0x50) :
                    *reinterpret_cast<const std::uintptr_t*>(records +
                        24 * (static_cast<std::uintptr_t>(level) + 1));
                if (functionBase < HksObjectSize)
                    return false;

                const auto typeBits = *reinterpret_cast<const std::uintptr_t*>(
                    functionBase - HksObjectSize);
                if ((typeBits & 0xF) != 9)
                    return false;

                const auto closure = *reinterpret_cast<const std::uintptr_t*>(
                    functionBase - sizeof(std::uintptr_t));
                if (!closure)
                    return false;

                const auto method = *reinterpret_cast<const std::uintptr_t*>(closure + 0x10);
                if (!method)
                    return false;

                const auto instructionCount = *reinterpret_cast<const std::uint32_t*>(
                    method + 0x20);
                const auto instructionData = *reinterpret_cast<const std::uintptr_t*>(
                    method + 0x28);
                const auto hash = *reinterpret_cast<const std::uint32_t*>(method + 0x10);
                if (!hash || !instructionData || instructionCount == 0 ||
                    instructionCount > MaximumDatabaseEntries)
                {
                    return false;
                }

                functionHash = hash;
                instructions = instructionData;
                return true;
            }
            __except (EXCEPTION_EXECUTE_HANDLER)
            {
                return false;
            }
        }

        bool HasLuaBytecodeHeader(std::uintptr_t address)
        {
            __try
            {
                return address != 0 &&
                    *reinterpret_cast<const std::uint32_t*>(address) == 0x61754C1B;
            }
            __except (EXCEPTION_EXECUTE_HANDLER)
            {
                return false;
            }
        }

        const char* ResolveLiveFunctionName(std::uint32_t functionHash,
            std::uintptr_t instructions)
        {
            const auto& prototypes = GetStockDatabases().prototypes;
            const auto candidates = prototypes.find(functionHash);
            if (candidates == prototypes.end())
                return nullptr;

            const StockPrototypeRecord* match = nullptr;
            for (const auto& candidate : candidates->second)
            {
                if (candidate.codeOffset > MaximumDatabaseFileSize ||
                    instructions < candidate.codeOffset)
                {
                    continue;
                }

                const auto bytecodeBase = instructions - candidate.codeOffset;
                if (!HasLuaBytecodeHeader(bytecodeBase))
                    continue;

                if (match && (match->codeOffset != candidate.codeOffset ||
                    match->functionName != candidate.functionName))
                {
                    return nullptr;
                }
                match = &candidate;
            }

            return match && !match->functionName.empty() ?
                match->functionName.c_str() : nullptr;
        }

        int __fastcall HksiLuaGetInfoHook(std::int64_t* luaState,
            const char* what, void* debugRecord)
        {
            const auto result = OriginalHksiLuaGetInfo(luaState, what, debugRecord);
            if (!result || !debugRecord)
                return result;

            const auto debug = reinterpret_cast<std::uintptr_t>(debugRecord);
            const auto currentName = *reinterpret_cast<const char* const*>(debug + 0x8);
            if (!currentName || strcmp(currentName, "(*stripped)") != 0)
                return result;

            std::uint32_t functionHash = 0;
            std::uintptr_t instructions = 0;
            if (TryGetLivePrototype(luaState, debugRecord, functionHash, instructions))
            {
                if (const auto resolvedName = ResolveLiveFunctionName(functionHash,
                    instructions))
                {
                    *reinterpret_cast<const char**>(debug + 0x8) = resolvedName;
                }
            }
            return result;
        }

        bool ParseFramePrefix(std::string_view line, std::size_t& prefixStart,
            std::size_t& prefixEnd, std::uint64_t& key)
        {
            prefixStart = 0;
            while (prefixStart < line.size() &&
                (line[prefixStart] == ' ' || line[prefixStart] == '\t'))
            {
                ++prefixStart;
            }

            auto cursor = prefixStart;
            while (cursor < line.size() && line[cursor] >= '0' && line[cursor] <= '9')
                ++cursor;
            if (cursor == prefixStart || cursor >= line.size() || line[cursor] != ':')
                return false;

            std::uint32_t functionHash = 0;
            if (!ParseUInt32(line.substr(prefixStart, cursor - prefixStart), functionHash))
                return false;

            const auto instructionStart = ++cursor;
            while (cursor < line.size() && line[cursor] >= '0' && line[cursor] <= '9')
                ++cursor;
            if (cursor == instructionStart || cursor >= line.size() || line[cursor] != ':')
                return false;

            std::uint32_t instructionIndex = 0;
            if (!ParseUInt32(line.substr(instructionStart,
                cursor - instructionStart), instructionIndex))
            {
                return false;
            }

            prefixEnd = cursor + 1;
            key = MakeSymbolKey(functionHash, instructionIndex);
            return true;
        }

        std::string RewriteStackTrace(std::string_view stackTrace,
            const SymbolMap& symbols, const StockSymbolMap& stockSymbols,
            std::size_t& resolvedFrames)
        {
            constexpr std::string_view StrippedFunction = "in function '(*stripped)'";

            std::string rewritten;
            rewritten.reserve(stackTrace.size());
            auto position = std::size_t{};

            while (position < stackTrace.size())
            {
                const auto newline = stackTrace.find('\n', position);
                const auto lineEnd = newline == std::string_view::npos ?
                    stackTrace.size() : newline;
                const auto line = stackTrace.substr(position, lineEnd - position);

                std::size_t prefixStart = 0;
                std::size_t prefixEnd = 0;
                std::uint64_t key = 0;
                const auto parsed = ParseFramePrefix(line, prefixStart, prefixEnd, key);
                const auto symbol = parsed ? symbols.find(key) : symbols.end();
                const auto functionHash = static_cast<std::uint32_t>(key >> 32);
                const auto instructionIndex = static_cast<std::uint32_t>(key);
                const auto stockSymbol = parsed && symbol == symbols.end() ?
                    stockSymbols.find(functionHash) : stockSymbols.end();
                if (symbol == symbols.end() && stockSymbol == stockSymbols.end())
                {
                    rewritten.append(line);
                }
                else if (symbol == symbols.end())
                {
                    rewritten.append(line.substr(0, prefixStart));
                    rewritten.append(stockSymbol->second.source);
                    rewritten.append(":pc");
                    rewritten.append(std::to_string(instructionIndex));
                    rewritten.push_back(':');

                    const auto remainder = line.substr(prefixEnd);
                    const auto stripped = remainder.find(StrippedFunction);
                    if (stripped == std::string_view::npos ||
                        stockSymbol->second.functionName.empty())
                    {
                        rewritten.append(remainder);
                    }
                    else
                    {
                        rewritten.append(remainder.substr(0, stripped));
                        rewritten.append("in function '");
                        rewritten.append(stockSymbol->second.functionName);
                        rewritten.push_back('\'');
                        rewritten.append(remainder.substr(stripped +
                            StrippedFunction.size()));
                    }
                    ++resolvedFrames;
                }
                else
                {
                    rewritten.append(line.substr(0, prefixStart));
                    rewritten.append(symbol->second.source);
                    rewritten.push_back(':');
                    rewritten.append(std::to_string(symbol->second.sourceLine));
                    rewritten.push_back(':');

                    const auto remainder = line.substr(prefixEnd);
                    const auto stripped = remainder.find(StrippedFunction);
                    if (stripped == std::string_view::npos ||
                        symbol->second.functionName.empty())
                    {
                        rewritten.append(remainder);
                    }
                    else
                    {
                        rewritten.append(remainder.substr(0, stripped));
                        rewritten.append("in function '");
                        rewritten.append(symbol->second.functionName);
                        rewritten.push_back('\'');
                        rewritten.append(remainder.substr(stripped + StrippedFunction.size()));
                    }
                    ++resolvedFrames;
                }

                if (newline == std::string_view::npos)
                    break;
                rewritten.push_back('\n');
                position = newline + 1;

                if (rewritten.size() > MaximumStackTraceSize)
                    return {};
            }
            return rewritten;
        }

        bool ReplaceTopErrorObject(std::int64_t* luaState,
            const std::string& rewritten)
        {
            if (!luaState || rewritten.empty() || rewritten.size() > MaximumStackTraceSize)
                return false;

            const auto top = static_cast<std::uintptr_t>(luaState[LuaStateTopOffset / 8]);
            const auto base = static_cast<std::uintptr_t>(luaState[LuaStateBaseOffset / 8]);
            if (top < base + HksObjectSize || top - base > MaximumStackTraceSize)
                return false;

            const auto pushString = reinterpret_cast<LuaPushLString>(
                REBASE(LuaPushLStringRva));
            pushString(luaState, rewritten.data(), rewritten.size());

            const auto newTop = static_cast<std::uintptr_t>(
                luaState[LuaStateTopOffset / 8]);
            if (newTop != top + HksObjectSize)
            {
                luaState[LuaStateTopOffset / 8] = static_cast<std::int64_t>(top);
                return false;
            }

            memcpy(reinterpret_cast<void*>(top - HksObjectSize),
                reinterpret_cast<const void*>(top), HksObjectSize);
            luaState[LuaStateTopOffset / 8] = static_cast<std::int64_t>(top);
            return true;
        }

        void ResolveTopErrorStack(std::int64_t* luaState)
        {
            if (!luaState)
                return;

            const auto top = static_cast<std::uintptr_t>(luaState[LuaStateTopOffset / 8]);
            const auto base = static_cast<std::uintptr_t>(luaState[LuaStateBaseOffset / 8]);
            if (top < base + HksObjectSize || top - base > MaximumStackTraceSize)
                return;

            const auto objectToString = reinterpret_cast<HksObjectToString>(
                REBASE(HksObjectToStringRva));
            std::size_t stackLength = 0;
            const auto* stack = objectToString(luaState,
                static_cast<std::int64_t>(top - HksObjectSize), &stackLength);
            if (!stack || stackLength == 0 || stackLength > MaximumStackTraceSize)
                return;

            const ExclusiveLock symbolLock(SymbolLock);
            const auto symbols = LoadSymbolsForActiveMod();
            const auto& stockSymbols = GetStockDatabases().symbols;
            std::size_t resolvedFrames = 0;
            const auto rewritten = RewriteStackTrace(
                std::string_view(stack, stackLength), symbols, stockSymbols,
                resolvedFrames);
            if (resolvedFrames != 0 && !rewritten.empty() && rewritten !=
                std::string_view(stack, stackLength))
            {
                if (ReplaceTopErrorObject(luaState, rewritten))
                {
                    OutputDebugStringA("T7 Patch: resolved Lua stack symbols.\n");
                }
            }
        }

        void __fastcall LuaStateManagerErrorHook(const char* error,
            std::int64_t* luaState)
        {
            try
            {
                ResolveTopErrorStack(luaState);
            }
            catch (...)
            {
                OutputDebugStringA("T7 Patch: Lua symbol resolution failed; using the original stack trace.\n");
            }
            OriginalLuaStateManagerError(error, luaState);
        }
    }

    void InstallHooks()
    {
        MH_CreateHook(reinterpret_cast<void*>(REBASE(HksiLuaGetInfoRva)),
            HksiLuaGetInfoHook,
            reinterpret_cast<void**>(&OriginalHksiLuaGetInfo));
        MH_CreateHook(reinterpret_cast<void*>(REBASE(LuaStateManagerErrorRva)),
            LuaStateManagerErrorHook,
            reinterpret_cast<void**>(&OriginalLuaStateManagerError));
    }
}
